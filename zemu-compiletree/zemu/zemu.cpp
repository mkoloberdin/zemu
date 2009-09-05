#include <string.h>
#include <ctype.h>
#include "zemu.h"
#include "exceptions.h"
#include "bin.h"
#include "lib_wd1793/wd1793_chip.h"
#include "dirwork.h"
#include "font.h"
#include "dialog.h"
#include "graphics.h"
#include "sound/sdlwaveplayer.h"
#include "cpu_trace.h"
#include "tape/tape.h"
#include "labels.h"
#include "joystick_manager.h"		// [boo_boo]

#include "renderer/render_speccy.h"
#include "renderer/render_16c.h"
#include "renderer/render_multicolor.h"

#include "devs.h"

#include "snap_z80.h"
#include "snap_sna.h"
#include "sound/native.h"

#include "ftbos_font.h"
#include "font_64.h"

#define SNAP_FORMAT_Z80 0
#define SNAP_FORMAT_SNA 1

s_Sample mixBuffer[MIX_BUFFER_SIZE * 2];
WORD audioBuffer[MIX_BUFFER_SIZE * 2];
volatile int audioCnt;

unsigned turboMultiplier = 1;
unsigned turboMultiplierNx = 1;

bool isPaused = false;
bool isPausedNx = false;

bool joyOnKeyb = false;

Z80EX_CONTEXT *cpu;
unsigned long long clk, devClk, lastDevClk, devClkCounter;
s_Params params;
SDL_Surface *screen, *realScreen, *scrSurf[2];
int PITCH, REAL_PITCH;
bool drawFrame;
int frames;
C_Config config;
C_Font font, fixed;
bool disableSound = false;
bool doCopyAfSurfaces = false;

int videoSpec;
int actualWidth;
int actualHeight;

SDLWavePlayer *sdlWp;

char tempFolderName[MAX_PATH];

bool recordWav = false;
C_File wavFl;

//--------------------------------------------------------------------------------------------------------------

onReadByteFunc * devMapRead;
bool (** devMapWrite)(Z80EX_WORD, Z80EX_BYTE);
bool (** devMapInput)(Z80EX_WORD, Z80EX_BYTE&);
bool (** devMapOutput)(Z80EX_WORD, Z80EX_BYTE);

onReadByteFunc devMapRead_base[0x20000];
bool (* devMapWrite_base[0x10000])(Z80EX_WORD, Z80EX_BYTE);
bool (* devMapInput_base[0x10000])(Z80EX_WORD, Z80EX_BYTE&);
bool (* devMapOutput_base[0x10000])(Z80EX_WORD, Z80EX_BYTE);

onReadByteFunc devMapRead_trdos[0x20000];
bool (* devMapInput_trdos[0x10000])(Z80EX_WORD, Z80EX_BYTE&);
bool (* devMapOutput_trdos[0x10000])(Z80EX_WORD, Z80EX_BYTE);

//--------------------------------------------------------------------------------------------------------------

struct s_ReadItem
{
	onReadByteFunc (* check)(Z80EX_WORD, bool);
//	bool (* func)(Z80EX_WORD, bool, Z80EX_BYTE&);
};

struct s_WriteItem
{
	bool (* check)(Z80EX_WORD);
	bool (* func)(Z80EX_WORD, Z80EX_BYTE);
};

struct s_InputItem
{
	bool (* check)(Z80EX_WORD);
	bool (* func)(Z80EX_WORD, Z80EX_BYTE&);
};

typedef s_WriteItem s_OutputItem;

struct s_SdlItem
{
	int eventType;
	bool (* func)(SDL_Event&);
};

//--------------------------------------------------------------------------------------------------------------

s_ReadItem hnd_z80read[MAX_HANDLERS];
s_WriteItem hnd_z80write[MAX_HANDLERS];
s_InputItem hnd_z80input[MAX_HANDLERS];
s_OutputItem hnd_z80output[MAX_HANDLERS];
void (* hnd_frameStart[MAX_HANDLERS])(void);
void (* hnd_afterFrameRender[MAX_HANDLERS])(void);
s_SdlItem hnd_sdl[MAX_HANDLERS];
void (* hnd_reset[MAX_HANDLERS])(void);
C_SndRenderer* sndRenderers[MAX_HANDLERS];

int cnt_z80read = 0;
int cnt_z80write = 0;
int cnt_z80input = 0;
int cnt_z80output = 0;
int cnt_frameStart = 0;
int cnt_afterFrameRender = 0;
int cnt_sdl = 0;
int cnt_reset = 0;
int cnt_sndRenderers = 0;

void AttachZ80ReadHandler(onReadByteFunc (* check)(Z80EX_WORD, bool))
{
	if (cnt_z80read >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");

	s_ReadItem item;
	item.check = check;

	hnd_z80read[cnt_z80read++] = item;
}

void AttachZ80WriteHandler(bool (* check)(Z80EX_WORD), bool (* func)(Z80EX_WORD, Z80EX_BYTE))
{
	if (cnt_z80write >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");

	s_WriteItem item;
	item.check = check;
	item.func = func;

	hnd_z80write[cnt_z80write++] = item;
}

void AttachZ80InputHandler(bool (* check)(Z80EX_WORD), bool (* func)(Z80EX_WORD, Z80EX_BYTE&))
{
	if (cnt_z80input >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");

	s_InputItem item;
	item.check = check;
	item.func = func;

	hnd_z80input[cnt_z80input++] = item;
}

void AttachZ80OutputHandler(bool (* check)(Z80EX_WORD), bool (* func)(Z80EX_WORD, Z80EX_BYTE))
{
	if (cnt_z80output >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");

	s_OutputItem item;
	item.check = check;
	item.func = func;

	hnd_z80output[cnt_z80output++] = item;
}

void AttachFrameStartHandler(void (* func)(void))
{
	if (cnt_frameStart >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");
	hnd_frameStart[cnt_frameStart++] = func;
}

void AttachAfterFrameRenderHandler(void (* func)(void))
{
	if (cnt_afterFrameRender >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");
	hnd_afterFrameRender[cnt_afterFrameRender++] = func;
}

void AttachSDLHandler(int eventType, bool (* func)(SDL_Event&))
{
	if (cnt_sdl >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");

	s_SdlItem item;
	item.eventType = eventType;
	item.func = func;

	hnd_sdl[cnt_sdl++] = item;
}

void AttachResetHandler(void (* func)(void))
{
	if (cnt_reset >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");
	hnd_reset[cnt_reset++] = func;
}

void RegisterSndRenderer(C_SndRenderer *sndr)
{
	if (cnt_sndRenderers >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");
	sndRenderers[cnt_sndRenderers++] = sndr;
}

//--------------------------------------------------------------------------------------------------------------

C_Border dev_border;
C_ExtPort dev_extport;
C_Keyboard dev_keyboard;
C_TrDos dev_trdos;
C_MemoryManager dev_mman;
C_TsFm dev_tsfm;
C_Mouse dev_mouse;
C_Covox dev_covox;
C_KempstonStick dev_kempston; // [boo_boo]

C_Device* devs[] =
{
	&dev_border,
	&dev_extport,
	&dev_keyboard,
	&dev_trdos,
	&dev_mman,
	&dev_tsfm,
	&dev_mouse,
	&dev_covox,
	&dev_kempston, // [boo_boo]
	NULL
};

int colors_base[0x10] = {
	DRGB(  0,   0,   0),
	DRGB(  0,   0, 192),
	DRGB(192,   0,   0),
	DRGB(192,   0, 192),
	DRGB(  0, 192,   0),
	DRGB(  0, 192, 192),
	DRGB(192, 192,   0),
	DRGB(192, 192, 192),
	DRGB(  0,   0,   0),
	DRGB(  0,   0, 255),
	DRGB(255,   0,   0),
	DRGB(255,   0, 255),
	DRGB(  0, 255,   0),
	DRGB(  0, 255, 255),
	DRGB(255, 255,   0),
	DRGB(255, 255, 255)
};

int colors[0x10];

//--------------------------------------------------------------------------------------------------------------

int messageTimeout = 0;
char message[0x100];

void OutputText(char *str)
{
	int x, y;

	x = (WIDTH - font.StrLenPx(str)) / 2;
	y = HEIGHT - font.Height() - 4;

	font.PrintString(x, y, str);
}

void ShowMessage(void)
{
	if (messageTimeout <= 0) return;
	messageTimeout--;
	OutputText(message);
}

void SetMessage(const char *str)
{
	strcpy(message, str);
	messageTimeout = 50;
}

//--------------------------------------------------------------------------------------------------------------

void ResetSequence(void);

void StrToLower(char *str)
{
	while (*str) {
		*(str) = tolower(*str);
		str++;
	}
}

#define MAX_FILES 4096
#define MAX_FNAME 256

bool IsArchive(char *fname)
{
	char plgName[MAX_PATH];
	char ext[MAX_FNAME];

	if (!(*params.arcPluginsDir)) return false;
	strcpy(ext, C_DirWork::ExtractExt(fname));
	StrToLower(ext);

	strcpy(plgName, params.arcPluginsDir);
	strcat(plgName, ext);

	return C_File::FileExists(plgName);
}

void LoadNormalFile(char *fname, int drive)
{
	if (C_Tape::IsTapeFormat(fname)) {
		C_Tape::Insert(fname);
	} else if (!stricmp(C_DirWork::ExtractExt(fname), "z80")) {
		if (!load_z80_snap(fname, cpu, dev_mman, dev_border)) StrikeMessage("Error loading snapshot");
	} else if (!stricmp(C_DirWork::ExtractExt(fname), "sna")) {
		if (!load_sna_snap(fname, cpu, dev_mman, dev_border)) StrikeMessage("Error loading snapshot");
	} else {
		wd1793_load_dimage(fname, drive);
	}
}

void LoadArcFile(char *arcName, int drive)
{
	C_File fl;
	char plgName[MAX_PATH], res[MAX_PATH];
	char tmp[MAX_PATH], str[MAX_FNAME];
	char files[MAX_FILES][MAX_FNAME];
	int filesCount;

	strcpy(tmp, C_DirWork::ExtractExt(arcName));
	StrToLower(tmp);

	strcpy(plgName, params.arcPluginsDir);
	strcat(plgName, tmp);

	sprintf(tmp, "%s list %s %s/files.txt", plgName, arcName, tempFolderName);
	if (system(tmp) == -1) _DEBUG("system failed");

	sprintf(tmp, "%s/files.txt", tempFolderName);
	if (!C_File::FileExists(tmp)) return;

	fl.Read(tmp);
	for (filesCount = 0; !fl.Eof(); )
	{
		fl.GetS(str, sizeof(str));
		if (strcmp(str, "")) strcpy(files[filesCount++], str);
	}

	unlink(tmp);
	if (!filesCount) return;

	// currently load only first file
	sprintf(tmp, "%s extract %s %s %s", plgName, arcName, files[0], tempFolderName);
	if (system(tmp) == -1) _DEBUG("system failed");
	strcpy(tmp, C_DirWork::ExtractFileName(files[0]));

	// TODO: check if lresult strlen > sizeof(res)
	sprintf(res, "%s/%s", tempFolderName, tmp);

	LoadNormalFile(res, drive);
	unlink(res);
}

void TryNLoadFile(char *fname, int drive)
{
	#ifdef _WIN32
		char tname[MAX_PATH];

		strcpy(tname, fname);

		if (tname[0]=='/' && tname[2]=='/')
		{
			tname[0] = tname[1];
			tname[1] = ':';
		}
	#else
		char *tname = fname;
	#endif

	if (IsArchive(tname)) LoadArcFile(tname, drive);
	else LoadNormalFile(tname, drive);
}

void TryNLoadFile(char *fname)
{
	TryNLoadFile(fname, 0);
}

//--------------------------------------------------------------------------------------------------------------

void Action_Reset(void)
{
	isPaused = false;
	ResetSequence();
}

void Action_ResetTrDos(void)
{
	isPaused = false;
	ResetSequence();
	dev_mman.OnOutputByte(0x7FFD, 0x10);
	dev_trdos.Enable();
}

void Action_MaxSpeed(void)
{
	isPaused = false;
	params.maxSpeed = !params.maxSpeed;
	SetMessage(params.maxSpeed ? "MaxSpeed ON" : "MaxSpeed OFF");
}

void Action_QuickLoad(void)
{
	bool (* loadSnap)(const char *filename, Z80EX_CONTEXT *cpu, C_MemoryManager &mmgr, C_Border &border);

	isPaused = false;

	const char * snapName = (params.snapFormat==SNAP_FORMAT_SNA ? "snap.sna" : "snap.z80");
	loadSnap = (params.snapFormat==SNAP_FORMAT_SNA ? load_sna_snap : load_z80_snap);

	if (!loadSnap(snapName, cpu, dev_mman, dev_border)) SetMessage("Error loading snapshot");
	else SetMessage("Snapshot loaded");
}

void Action_QuickSave(void)
{
	void (* saveSnap)(const char *filename, Z80EX_CONTEXT *cpu, C_MemoryManager &mmgr, C_Border &border);

	isPaused = false;

	const char * snapName = (params.snapFormat==SNAP_FORMAT_SNA ? "snap.sna" : "snap.z80");
	saveSnap = (params.snapFormat==SNAP_FORMAT_SNA ? save_sna_snap : save_z80_snap);

	saveSnap(snapName, cpu, dev_mman, dev_border);
	SetMessage("Snapshot saved");
}

void Action_AntiFlicker(void)
{
	isPaused = false;
	params.antiFlicker = !params.antiFlicker;
	if (params.antiFlicker) doCopyAfSurfaces = true;
	SetMessage(params.antiFlicker ? "AntiFlicker ON" : "AntiFlicker OFF");
}

void Action_LoadFile(void)
{
	FileDialog();
}

void Action_Fullscreen(void)
{
	/* SDL_WM_ToggleFullscreen works only on X11 */

	videoSpec ^= SDL_FULLSCREEN;
	SDL_FreeSurface(realScreen);
	realScreen = SDL_SetVideoMode(actualWidth, actualHeight, 32, videoSpec);
	REAL_PITCH = realScreen->pitch / 4;

	if (!params.scale2x)
	{
		screen = realScreen;
		PITCH = REAL_PITCH;
	}
}

void Action_Debugger(void)
{
	isPaused = false;
	RunDebugger();
}

void Action_Turbo(void)
{
	isPaused = false;
	turboMultiplierNx *= 2;

	if (turboMultiplierNx > 4) turboMultiplierNx = 1;

	if (turboMultiplierNx < 2) {
		SetMessage("Turbo OFF");
	} else {
		char buf[0x10];
		sprintf(buf, "Turbo %dx", turboMultiplierNx);
		SetMessage(buf);
	}
}

void Action_AttributesHack(void)
{
	isPaused = false;
	attributesHack = !attributesHack;
	SetMessage(attributesHack ? "AttributesHack ON" : "AttributesHack OFF");
}

void Action_Pause(void)
{
	isPausedNx = !isPausedNx;
	SetMessage(isPausedNx ? "Pause ON" : "Pause OFF");
}

void Action_JoyOnKeyb(void)
{
	isPaused = false;
	joyOnKeyb = !joyOnKeyb;
	dev_kempston.joy_kbd = 0;
	SetMessage(joyOnKeyb ? "Kempston on keyboard ON" : "Kempston on keyboard OFF");
}

s_Action cfgActions[] =
{
	{"reset",			Action_Reset},
	{"reset_trdos",		Action_ResetTrDos},
	{"max_speed",		Action_MaxSpeed},
	{"quick_load",		Action_QuickLoad},
	{"quick_save",		Action_QuickSave},
	{"anti_flicker",	Action_AntiFlicker},
	{"load_file",		Action_LoadFile},
	{"fullscreen",		Action_Fullscreen},
	{"debugger",		Action_Debugger},
	{"turbo",			Action_Turbo},
	{"attrib_hack",		Action_AttributesHack},
	{"pause",			Action_Pause},
	{"joy_on_keyb",		Action_JoyOnKeyb},
	{"",				NULL}
};

//--------------------------------------------------------------------------------------------------------------

bool runDebuggerFlag = false;
bool breakpoints[0x10000];

WORD watches[MAX_WATCHES];
unsigned watchesCount = 0;

Z80EX_BYTE ReadByteDasm(Z80EX_WORD addr, void *userData)
{
    onReadByteFunc func = devMapRead[addr];
    return func(addr, false);
}

void WriteByteDasm(Z80EX_WORD addr, Z80EX_BYTE value)
{
	for (;;)
	{
		bool (* func)(Z80EX_WORD, Z80EX_BYTE) = devMapWrite[addr];

		if (func == NULL) return;
		if (func(addr, value)) return;
	}
}

Z80EX_BYTE ReadByte(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *userData)
{
	unsigned raddr = addr + (m1_state ? 0x10000 : 0);
    onReadByteFunc func = devMapRead[raddr];
    return func(addr, m1_state);
}

void WriteByte(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *userData)
{
	for (;;)
	{
		bool (* func)(Z80EX_WORD, Z80EX_BYTE) = devMapWrite[addr];

		if (func == NULL) return;
		if (func(addr, value)) return;
	}
}

Z80EX_BYTE InputByte(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *userData)
{
	Z80EX_BYTE retval;

	for (;;)
	{
		bool (* func)(Z80EX_WORD, Z80EX_BYTE&) = devMapInput[port];

		if (func == NULL) return 0xFF;
		if (func(port, retval)) return retval;
	}
}

void OutputByte(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *userData)
{
	for (;;)
	{
		bool (* func)(Z80EX_WORD, Z80EX_BYTE) = devMapOutput[port];

		if (func == NULL) return;
		if (func(port, value)) return;
	}
}

Z80EX_BYTE ReadIntVec(Z80EX_CONTEXT *cpu, void *userData)
{
	return 0xFF;
}

//--------------------------------------------------------------------------------------------------------------

void InitDevMapRead(onReadByteFunc * map)
{
	onReadByteFunc func;
	for (unsigned m1_state = 0; m1_state < 2; m1_state++)
	{
		for (unsigned addr = 0; addr < 0x10000; addr++)
		{
			map[addr + (m1_state ? 0x10000 : 0)] = NULL;

			for (int i = 0; i < cnt_z80read; i++)
			{
				if ((func = hnd_z80read[i].check(addr, m1_state)) != NULL)
				{
					map[addr + (m1_state ? 0x10000 : 0)] = func;
					break;
				}
			}
		}
	}
}

void InitDevMapWrite(bool (** map)(Z80EX_WORD, Z80EX_BYTE))
{
	for (unsigned addr = 0; addr < 0x10000; addr++)
	{
		map[addr] = NULL;

		for (int i = 0; i < cnt_z80write; i++)
		{
			if (hnd_z80write[i].check(addr))
			{
				map[addr] = hnd_z80write[i].func;
				break;
			}
		}
	}
}

void InitDevMapInput(bool (** map)(Z80EX_WORD, Z80EX_BYTE&))
{
	for (unsigned port = 0; port < 0x10000; port++)
	{
		map[port] = NULL;

		for (int i = 0; i < cnt_z80input; i++)
		{
			if (hnd_z80input[i].check(port))
			{
				map[port] = hnd_z80input[i].func;
				break;
			}
		}
	}
}

void InitDevMapOutput(bool (** map)(Z80EX_WORD, Z80EX_BYTE))
{
	for (unsigned port = 0; port < 0x10000; port++)
	{
		map[port] = NULL;

		for (int i = 0; i < cnt_z80output; i++)
		{
			if (hnd_z80output[i].check(port))
			{
				map[port] = hnd_z80output[i].func;
				break;
			}
		}
	}
}

void InitDevMaps(void)
{
	C_TrDos::trdos = true;

	InitDevMapRead(devMapRead_trdos);
	InitDevMapInput(devMapInput_trdos);
	InitDevMapOutput(devMapOutput_trdos);

	C_TrDos::trdos = false;

	InitDevMapRead(devMapRead_base);
	InitDevMapWrite(devMapWrite_base);
	InitDevMapInput(devMapInput_base);
	InitDevMapOutput(devMapOutput_base);

	devMapRead = devMapRead_base;
	devMapWrite = devMapWrite_base;
	devMapInput = devMapInput_base;
	devMapOutput = devMapOutput_base;
}

//--------------------------------------------------------------------------------------------------------------

void InitSurfaces(void)
{
	SDL_PixelFormat *fmt = screen->format;

	scrSurf[0] = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, 0);
	if (scrSurf[0] == NULL) StrikeError("Unable to create primary surface: %s\n", SDL_GetError());

	scrSurf[1] = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, 0);
	if (scrSurf[1] == NULL) StrikeError("Unable to create secondary surface: %s\n", SDL_GetError());
}

void InitFont(void)
{
	font.Init(ftbos_font_data);
	font.CopySym('-', '_');
	font.SetSymOff('_', 0, 4);
	font.SetSymOff('-', 0, 1);

	fixed.Init(font_64_data);
}

void InitAll(void)
{
	int i;
	for (i = 0; devs[i]; i++) devs[i]->Init();

	InitDevMaps();

	for (i = 0; i < 0x10; i++) colors[i] = colors_base[i];

	InitSurfaces();
	InitFont();
	FileDialogInit();
	C_Tape::Init();

	for (i = 0; i < 0x10000; i++) breakpoints[i] = false;

	cpu = z80ex_create(
			ReadByte, NULL,
			WriteByte, NULL,
			InputByte, NULL,
			OutputByte, NULL,
			ReadIntVec, NULL
		);
}

#define MAX_FRAME_TACTS 71680
#define INT_LENGTH 32
#define INT_BEGIN 68069

bool attributesHack = false;

// left_border        = 36
// horizontal_screen  = 128
// right_border       = 28
// horizontal_retrace = 32

// top_border         = 64
// vertical_screen    = 192
// bottom_border      = 48
// vertical_retrace   = 16

// ----------------------------------

void AntiFlicker(SDL_Surface *copyFrom, SDL_Surface *copyTo)
{
	int i, j;
	BYTE *s1, *s2, *sr;
	BYTE *s1w, *s2w, *srw;

	if (SDL_MUSTLOCK(screen)) {if (SDL_LockSurface(screen) < 0) return;}
	if (SDL_MUSTLOCK(scrSurf[0])) {if (SDL_LockSurface(scrSurf[0]) < 0) return;}
	if (SDL_MUSTLOCK(scrSurf[1])) {if (SDL_LockSurface(scrSurf[1]) < 0) return;}

	if (doCopyAfSurfaces)
	{
		s1 = (BYTE *)copyFrom->pixels;
		s2 = (BYTE *)copyTo->pixels;

		for (i = HEIGHT; i--;)
		{
			DWORD *s1dw = (DWORD *)s1;
			DWORD *s2dw = (DWORD *)s2;

			for (j = WIDTH; j--;) { *(s2dw++) = *(s1dw++); }

			s1 += copyFrom->pitch;
			s2 += copyTo->pitch;
		}

		doCopyAfSurfaces = false;
	}

	sr = (BYTE *)screen->pixels;
	s1 = (BYTE *)scrSurf[0]->pixels;
	s2 = (BYTE *)scrSurf[1]->pixels;

	for (i = HEIGHT; i--;)
	{
		srw = sr;
		s1w = s1;
		s2w = s2;

		for (j = WIDTH; j--;)
		{
			*srw = (BYTE)(((unsigned int)(*s1w) + (unsigned int)(*s2w)) >> 1);
			srw++; s1w++; s2w++;

			*srw = (BYTE)(((unsigned int)(*s1w) + (unsigned int)(*s2w)) >> 1);
			srw++; s1w++; s2w++;

			*srw = (BYTE)(((unsigned int)(*s1w) + (unsigned int)(*s2w)) >> 1);
			srw++; s1w++; s2w++;

			*(srw++) = 0; s1w++; s2w++;
		}

		sr += screen->pitch;
		s1 += scrSurf[0]->pitch;
		s2 += scrSurf[1]->pitch;
	}

	if (SDL_MUSTLOCK(scrSurf[1])) SDL_UnlockSurface(scrSurf[1]);
	if (SDL_MUSTLOCK(scrSurf[0])) SDL_UnlockSurface(scrSurf[0]);
	if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
}

unsigned long long actDevClkCounter = 0;
unsigned long long actClk = 0;

void InitActClk(void)
{
	actDevClkCounter = devClkCounter * (unsigned long long)turboMultiplier;
	actClk = clk * (unsigned long long)turboMultiplier;
}

inline void CpuCalcTacts(unsigned long cmdClk)
{
	if (turboMultiplier > 1)
	{
		actDevClkCounter += (unsigned long long)cmdClk;
		actClk += (unsigned long long)cmdClk;

		devClkCounter = (actDevClkCounter + (unsigned long long)(turboMultiplier-1)) / (unsigned long long)turboMultiplier;
		clk = (actClk + (unsigned long long)(turboMultiplier-1)) / (unsigned long long)turboMultiplier;
	}
	else
	{
		devClkCounter += (unsigned long long)cmdClk;
		clk += (unsigned long long)cmdClk;
	}

	devClk = clk;
	C_Tape::Process();

	if (runDebuggerFlag)
	{
		runDebuggerFlag = false;
		RunDebugger();
	}
	else
	if (breakpoints[z80ex_get_reg(cpu, regPC)])
	{
		RunDebugger();
	}
}

inline void DebugCpuCalcTacts(unsigned long cmdClk)
{
	if (turboMultiplier > 1)
	{
		actDevClkCounter += (unsigned long long)cmdClk;
		actClk += (unsigned long long)cmdClk;

		devClkCounter = (actDevClkCounter + (unsigned long long)(turboMultiplier-1)) / (unsigned long long)(turboMultiplier);
		clk = (actClk + (unsigned long long)(turboMultiplier-1)) / (unsigned long long)(turboMultiplier);
	}
	else
	{
		devClkCounter += (unsigned long long)cmdClk;
		clk += (unsigned long long)cmdClk;
	}

	devClk = clk;
	C_Tape::Process();
}

int (* DoCpuStep)(Z80EX_CONTEXT *cpu) = z80ex_step;
int (* DoCpuInt)(Z80EX_CONTEXT *cpu) = z80ex_int;

int TraceCpuStep(Z80EX_CONTEXT *cpu)
{
	CpuTrace_Log();
	cpuTrace_dT = z80ex_step(cpu);
	return cpuTrace_dT;
}

int TraceCpuInt(Z80EX_CONTEXT *cpu)
{
	// CpuTrace_Log();
	cpuTrace_dT = z80ex_int(cpu);
	return cpuTrace_dT;
}

inline void CpuStep(void)
{
	CpuCalcTacts(DoCpuStep(cpu));
}

inline void CpuInt(void)
{
	CpuCalcTacts(DoCpuInt(cpu));
}

inline void DebugCpuStep(void)
{
	DebugCpuCalcTacts(DoCpuStep(cpu));
}

inline void DebugCpuInt(void)
{
	DebugCpuCalcTacts(DoCpuInt(cpu));
}

void DebugStep(void)
{
	int cnt = 4;

	do
	{
		if (clk < MAX_FRAME_TACTS)
		{
			CpuStep();
			if (clk>=INT_BEGIN && clk<(INT_BEGIN+INT_LENGTH)) CpuInt();
		}
		else
		{
			lastDevClk = devClk;
			clk -= MAX_FRAME_TACTS;
			devClk = clk;

			InitActClk();
		}

		cnt--;
	} while (z80ex_last_op_type(cpu) && cnt>0);
}

void Render(void)
{
	SDL_Surface *surf;
	unsigned long lastClk;
	static int sn = 0;
	unsigned long (* render)(SDL_Surface*, int, unsigned long, unsigned long);

	if (params.antiFlicker)
	{
		surf = scrSurf[sn];
		sn = 1 - sn;
	} else surf = screen;

	if (SDL_MUSTLOCK(surf)) {if (SDL_LockSurface(surf) < 0) return;}
	int ptch = surf->pitch / 4;

	if (dev_extport.Is16Colors()) render = Render16c;
	else if (dev_extport.IsMulticolor()) render = RenderMulticolor;
//	else if (dev_extport.Is512x192()) render = Render512x192;
//	else if (dev_extport.Is384x304()) render = Render384x304;
	else render = RenderSpeccy;

	InitActClk();
	lastClk = 0;

	if (drawFrame)
	{
		while (clk < MAX_FRAME_TACTS)
		{
			CpuStep();
			lastClk = render(surf, ptch, lastClk, clk);

			if (clk>=INT_BEGIN && clk<(INT_BEGIN+INT_LENGTH)) CpuInt();
		}
	}
	else
	{
		while (clk < MAX_FRAME_TACTS)
		{
			CpuStep();
			if (clk>=INT_BEGIN && clk<(INT_BEGIN+INT_LENGTH)) CpuInt();
		}
	}

	lastDevClk = devClk;
	clk -= MAX_FRAME_TACTS;
	devClk = clk;

	if (SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
	if (params.antiFlicker && drawFrame) AntiFlicker(surf, scrSurf[sn]);
}

#include "images/floppy.h"
#include "images/floppy_read.h"
#include "images/floppy_write.h"
#include "images/turbo_off.h"
#include "images/turbo_on.h"

void DrawIndicators(void)
{
	char buf[0x100];

	DRIVE_STATE st = dev_trdos.GetIndicatorState();

	if (st == DS_READ) OutputGimpImage(8, 0, (s_GimpImage *) &img_floppyRead);
	else if (st == DS_WRITE) OutputGimpImage(8, 0, (s_GimpImage *) &img_floppyWrite);
	else if (params.showInactiveIcons) OutputGimpImage(8, 0, (s_GimpImage *) &img_floppy);

	if (params.maxSpeed) OutputGimpImage(32, 0, (s_GimpImage *) &img_turboOn);
	else if (params.showInactiveIcons) OutputGimpImage(32, 0, (s_GimpImage *) &img_turboOff); 

	if (C_Tape::IsActive())
	{
		sprintf(buf, "%d%%", C_Tape::GetPosPerc());

		int wdt = font.StrLenPx(buf);
		font.PrintString(WIDTH - 4 - wdt, 4, buf);
	}

	for (unsigned i = 0; i < watchesCount; i++)
	{
		Z80EX_BYTE val = ReadByteDasm(watches[i], NULL);
		sprintf(buf, "%04X:%02X", watches[i], val);

		int wdt = fixed.StrLenPx(buf);
		fixed.PrintString(WIDTH - 4 - wdt, 4 + fixed.Height() * (i+1), buf);
	}
}

void ResetSequence(void)
{
	int cnt = cnt_reset;
	void (** ptr)(void) = hnd_reset;

	z80ex_reset(cpu);

	while (cnt)
	{
		(*ptr)();

		ptr++;
		cnt--;
	}
}

void ScaleImage(void)
{
	if (!params.scale2x) return;

	if (SDL_MUSTLOCK(realScreen)) { if (SDL_LockSurface(realScreen) < 0) return; }

	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			if (SDL_MUSTLOCK(realScreen)) SDL_UnlockSurface(realScreen);
			return;
		}
	}

	if (params.scanlines)
	{
		for (int i = HEIGHT-1; i >= 0; i--)
		{
			int* line = (int*)screen->pixels + i * PITCH + WIDTH-1;
			int* lineA = (int*)realScreen->pixels + (i*2) * REAL_PITCH + (WIDTH*2-1);
			int* lineB = (int*)realScreen->pixels + (i*2+1) * REAL_PITCH + (WIDTH*2-1);

			for (int j = WIDTH; j--;)
			{
				int c = *(line--);
				int dc = (c & 0xFEFEFE) >> 1;

				*(lineA--) = c;
				*(lineA--) = c;
				*(lineB--) = dc;
				*(lineB--) = dc;
			}
		}
	}
	else
	{
		for (int i = HEIGHT-1; i >= 0; i--)
		{
			int* line = (int*)screen->pixels + i * PITCH + WIDTH-1;
			int* lineA = (int*)realScreen->pixels + (i*2) * REAL_PITCH + (WIDTH*2-1);
			int* lineB = (int*)realScreen->pixels + (i*2+1) * REAL_PITCH + (WIDTH*2-1);

			for (int j = WIDTH; j--;)
			{
				int c = *(line--);

				*(lineA--) = c;
				*(lineA--) = c;
				*(lineB--) = c;
				*(lineB--) = c;
			}
		}
	}

	if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
	if (SDL_MUSTLOCK(realScreen)) SDL_UnlockSurface(realScreen);
}

void Process(void)
{
	int key;
	SDL_Event event;
	int i;
	unsigned int btick;
	int frameSkip = 0;
	bool tapePrevActive = false;

	devClkCounter = 0;
	clk = 0;
	devClk = 0;
	lastDevClk = 0;
	frames = 0;
	params.maxSpeed = false;

	btick = SDL_GetTicks() + FRAME_WAIT_MS;

	//*/ unsigned long lastDivider = 0L;

	for (;;)
	{
		if (!isPaused)
		{
			i = cnt_frameStart;
			void (** ptr_frameStart)(void) = hnd_frameStart;

			while (i)
			{
				(*ptr_frameStart)();

				ptr_frameStart++;
				i--;
			}

			if (params.maxSpeed)
			{
				if (frameSkip > 0) {
					frameSkip--;
					drawFrame = false;
				} else {
					frameSkip = MAX_SPEED_FRAME_SKIP;
					drawFrame = true;
				}
			} else drawFrame = true;

			tapePrevActive = C_Tape::IsActive();

			Render();
			frames++;

			if (drawFrame)
			{
				DrawIndicators();
				ShowMessage();

				/*//
				char bbb[0x100]; sprintf(bbb, "%ld", lastDivider); font.PrintString(0, 0, bbb);
				for (i = 0; i < cnt_sndRenderers; i++) fixed.PrintString(16+i*4, 0, sndRenderers[i]->activeCnt ? "#" : "-");
				//*/

				ScaleImage();
				UpdateScreen();
			}

			if (!params.maxSpeed)
			{
				SDL_Delay(1);
				if (SDL_GetTicks() < btick) SDL_Delay(btick-SDL_GetTicks());
			}

			btick = SDL_GetTicks() + (params.sound ? 0 : FRAME_WAIT_MS);

			i = cnt_afterFrameRender;
			void (** ptr_afterFrameRender)(void) = hnd_afterFrameRender;

			while (i)
			{
				(*ptr_afterFrameRender)();

				ptr_afterFrameRender++;
				i--;
			}

			if (params.sound && !params.maxSpeed && cnt_sndRenderers)
			{
				unsigned minSamples = sndRenderers[0]->samples;
				unsigned maxSamples = sndRenderers[0]->samples;

				for (i = 1; i < cnt_sndRenderers; i++)
				{
					unsigned samples = sndRenderers[i]->samples;

					if (samples < minSamples) minSamples = samples;
					if (samples > maxSamples) maxSamples = samples;
				}

				if (minSamples)
				{
					s_Sample *p = mixBuffer;

					if (params.mixerMode & MIXER_SMART_MASK)
					{
						if (cnt_sndRenderers > 1)
						{
							for (i = minSamples; i--; p++)
							{
								p->left /= cnt_sndRenderers;
								p->right /= cnt_sndRenderers;
							}
						}
					}
					else
					{
						unsigned long divider = 0L;

						for (i = 0; i < cnt_sndRenderers; i++)
						{
							if (sndRenderers[i]->activeCnt)
							{
								divider++;
								sndRenderers[i]->activeCnt--;
							}
						}

						//*/ lastDivider = divider;

						if (divider)
						{
							p = mixBuffer;

							for (i = minSamples; i--; p++)
							{
								p->left /= divider;
								p->right /= divider;
							}
						}
					}

					p = mixBuffer;
					WORD* o = audioBuffer;

					if (params.mixerMode & MIXER_HALF_VOL_MASK)
					{
						for (i = minSamples; i--; p++)
						{
							*(o++) = (WORD)(p->left >> 1);
							*(o++) = (WORD)(p->right >> 1);
						}
					}
					else
					{
						for (i = minSamples; i--; p++)
						{
							*(o++) = (WORD)((long)p->left - 0x8000L);
							*(o++) = (WORD)((long)p->right - 0x8000L);
						}
					}

					if (recordWav)
					{
						o = audioBuffer;

						for (i = minSamples; i--;)
						{
							wavFl.PutWORD(*(o++));
							wavFl.PutWORD(*(o++));
						}
					}

					if (params.sdlSound) sdlWp->Write((BYTE*)audioBuffer, minSamples*sizeof(WORD)*2);
					else wav_play((BYTE*)audioBuffer, minSamples*sizeof(WORD)*2);
				}

				if (maxSamples > minSamples) {
					memmove(mixBuffer, &mixBuffer[minSamples], (maxSamples-minSamples) * sizeof(s_Sample));
				}

				memset(&mixBuffer[maxSamples-minSamples], 0, (MIX_BUFFER_SIZE) * sizeof(s_Sample));

				for (i = 0; i < cnt_sndRenderers; i++) {
					sndRenderers[i]->samples -= minSamples;
				}
			}
			else
			{
				for (i = 0; i < cnt_sndRenderers; i++) {
					sndRenderers[i]->samples = 0;
				}
			}
		}

		isPaused = isPausedNx;
		bool quitMode = false;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) exit(0);

			if (event.type == SDL_KEYUP)
			{
				key = event.key.keysym.sym;
				if (key == SDLK_ESCAPE) quitMode = true;
			}

			i = cnt_sdl;
			s_SdlItem * ptr_sdl = hnd_sdl;

			while (i)
			{
				if (ptr_sdl->eventType == event.type) {
					if (ptr_sdl->func(event)) {
						break;
					}
				}

				ptr_sdl++;
				i--;
			}
		}

		if (quitMode) {
			if (DlgConfirm("Are you really want to quit?")) {
				return;
			}
		}

		turboMultiplier = turboMultiplierNx;

		if (!isPaused && tapePrevActive && !C_Tape::IsActive())
		{
			if (params.maxSpeed)
			{
				SetMessage("Tape end : MaxSpeed OFF");
				params.maxSpeed = false;
			}
			else SetMessage("Tape end");
		}
	}
}

void InitAudio(void)
{
	if (params.sdlSound)
	{
		sdlWp = new SDLWavePlayer(SDLWAVE_CALLBACK_BUFFER_SIZE * params.sdlBufferSize);
		sdlWp->Init();
	}
	else
	{
		wav_start(params.soundParam, AUDIO_HW_BUFFER);
	}
}

void FreeAudio(void)
{
	if (params.sdlSound)
	{
		SDL_CloseAudio();
		delete sdlWp;
	}
	else
	{
		wav_stop();
	}
}

void UpdateScreen(void)
{
	if (params.useFlipSurface) SDL_Flip(realScreen);
	else SDL_UpdateRect(realScreen, 0, 0, 0, 0);
}

void FreeAll(void)
{
	int i;
	for (i = 0; devs[i]; i++) devs[i]->Close();

	C_Tape::Close();

	#ifdef OS_LINUX
		// TODO: do in in more portable way
		char cmd[MAX_PATH];
		strcpy(cmd, "rm -rf ");
		strcat(cmd, tempFolderName);
		if (system(cmd) == -1) _DEBUG("system failed");
	#endif

	if (videoSpec & SDL_FULLSCREEN)
	{
		videoSpec ^= SDL_FULLSCREEN;
		SDL_FreeSurface(realScreen);
		realScreen = SDL_SetVideoMode(actualWidth, actualHeight, 32, videoSpec);
	}

	SDL_FreeSurface(scrSurf[0]);
	SDL_FreeSurface(scrSurf[1]);
	SDL_FreeSurface(realScreen);

	if (params.scale2x) SDL_FreeSurface(screen);

	z80ex_destroy(cpu);
	if (params.sound) FreeAudio();
}

void ParseCmdLine(int argc, char *argv[])
{
	argv++;
	argc--;

	while (argc > 0)
	{
		if (!strcmp(*argv, "-l"))
		{
			if (argc > 1)
			{
				argv++;
				argc--;

				Labels_Load(*argv);
			}
		}
		else
		if (!strcmp(*argv, "-w"))
		{
			recordWav = true;
		}
		else
		{
			TryNLoadFile(*argv);
			return;
		}

		argv++;
		argc--;
	}
}

void OutputLogo(void)
{
	printf("                                     \n");
	printf("   $ww,.                             \n");
	printf("    `^$$$ww,.                        \n");
	printf("       `^$$$$$$                      \n");
	printf("         ,$$7'                       \n");
	printf("       _j$$'   __    __  _ _         \n");
	printf("     ,j$$7      /__ (-_ | ) ) (_|    \n");
	printf("    $$$$$$w.                         \n");
	printf("     `^^T$$$w,             rst'o6    \n");
	printf("           `^T$$                     \n");
	printf("                                     \n");
	printf(" restorer [ restorer.fct@gmail.com ] \n");
	printf(" boo_boo  [    boo_boo@inbox.ru    ] \n");
	printf("                                     \n");
	printf(" breeze (gfx)                        \n");
	printf("                                     \n");
	printf(" with help of SMT                    \n");
	printf("                                     \n");
}

void InitCfgName(char* cfg_fname)
{
	char* homePath = getenv("HOME");

	if (homePath)
	{
		#ifdef OS_LINUX
			// TODO: do in in more portable way
			char cmd[MAX_PATH];
			sprintf(cmd, "mkdir -p %s/.zemu", homePath);
			if (system(cmd) == -1) _DEBUG("system failed");
		#endif

		strcat(cfg_fname, homePath);
		strcat(cfg_fname, "/.zemu/config.xml");

		if (C_File::FileExists(cfg_fname)) return;
	}

	strcpy(cfg_fname, CONFIG_PATH);
	strcat(cfg_fname, "/config.xml");
}

int main(int argc, char *argv[])
{
	int spec;
	char *str;
	bool wd1793delay;

	OutputLogo();

	try
	{
		char cfg_fname[MAX_PATH] = "";
		InitCfgName(cfg_fname);

		if (C_File::FileExists(cfg_fname)) config.Load(cfg_fname);
		else StrikeError("Unable to load config from \"%s\"", cfg_fname);

		if (config.GetString("root/ArcPluginsDir", &str)) strcpy(params.arcPluginsDir, str);
		else *params.arcPluginsDir = 0;

		if (config.GetString("root/CpuTrace/Format", &str)) strcpy(params.cpuTraceFormat, str);
		else *params.cpuTraceFormat = 0;

		if (config.GetString("root/CpuTrace/FileName", &str)) strcpy(params.cpuTraceFileName, str);
		else *params.cpuTraceFileName = 0;

		if (!config.GetBool("root/UseFlipSurface", &params.useFlipSurface)) params.useFlipSurface = false;
		if (!config.GetBool("root/SoundEnable", &params.sound)) params.sound = false;
		if (!config.GetInt("root/MixerMode", &params.mixerMode)) params.mixerMode = 0;
		if (!config.GetBool("root/FullScreen", &params.fullscreen)) params.fullscreen = false;
		if (!config.GetBool("root/Scale2x", &params.scale2x)) params.scale2x = false;
		if (!config.GetBool("root/Scanlines", &params.scanlines)) params.scanlines = false;
		if (!config.GetBool("root/AntiFlicker", &params.antiFlicker)) params.antiFlicker = false;
		if (!config.GetBool("root/UseSdlSound", &params.sdlSound)) params.sdlSound = false;
		if (!config.GetBool("root/ShowInactiveIcons", &params.showInactiveIcons)) params.showInactiveIcons = true;
		if (!config.GetBool("root/CpuTrace/Enabled", &params.cpuTraceEnabled)) params.cpuTraceEnabled = false;

		if (config.GetString("root/SnapFormat", &str))
		{
			if (!strcasecmp(str, "sna")) params.snapFormat = SNAP_FORMAT_SNA;
			else params.snapFormat = SNAP_FORMAT_Z80;
		}
		else
		{
			params.snapFormat = SNAP_FORMAT_Z80;
		}

		if (config.GetString("root/Drives/A", &str)) wd1793_load_dimage(str, 0);
		if (config.GetString("root/Drives/B", &str)) wd1793_load_dimage(str, 1);
		if (config.GetString("root/Drives/C", &str)) wd1793_load_dimage(str, 2);
		if (config.GetString("root/Drives/D", &str)) wd1793_load_dimage(str, 3);

		if (config.GetBool("root/Drives/NoDelay", &wd1793delay)) wd1793_set_nodelay(wd1793delay);

		if (config.GetString("root/Drives/AppendBoot", &str)) {
			if (strcmp(str, "")) wd1793_set_appendboot(str);
		}

		if (!config.GetInt("root/MouseDiv", &params.mouseDiv)) params.mouseDiv = 1;
		else {
			if (params.mouseDiv <= 0) params.mouseDiv = 1;
			if (params.mouseDiv > 8) params.mouseDiv = 8;
		}

		if (!config.GetInt("root/SdlBufferSize", &params.sdlBufferSize)) params.sdlBufferSize = 4;

		#ifdef OS_LINUX
			if (!config.GetInt("root/OssFragNum", &params.soundParam)) params.soundParam = 8;
		#endif
		#ifdef OS_WINDOWS
			if (!config.GetInt("root/WqSize", &params.soundParam)) params.soundParam = 4;
		#endif

		spec = SDL_INIT_VIDEO;
		if (params.sound && params.sdlSound) spec |= SDL_INIT_AUDIO;
		if (SDL_Init(spec) < 0) StrikeError("Unable to init SDL: %s\n", SDL_GetError());

		atexit(SDL_Quit);

		videoSpec = SDL_SWSURFACE;
		if (params.useFlipSurface) videoSpec |= SDL_DOUBLEBUF;
		if (params.fullscreen) videoSpec |= SDL_FULLSCREEN;

		int actualWidth = WIDTH;
		int actualHeight = HEIGHT;

		if (params.scale2x)
		{
			actualWidth *= 2;
			actualHeight *= 2;
		}

		realScreen = SDL_SetVideoMode(actualWidth, actualHeight, 32, videoSpec);

		if (realScreen == NULL) StrikeError("Unable to set requested video mode: %s\n", SDL_GetError());
		REAL_PITCH = realScreen->pitch / 4;

		if (params.scale2x)
		{
			SDL_PixelFormat *fmt = realScreen->format;

			screen = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, 0);
			if (screen == NULL) StrikeError("Unable to create screen surface: %s\n", SDL_GetError());

			PITCH = screen->pitch / 4;
		}
		else
		{
			screen = realScreen;
			PITCH = REAL_PITCH;
		}

		SDL_WM_SetCaption("ZEmu", "ZEmu");
		SDL_ShowCursor(SDL_DISABLE);

		#ifdef OS_WINDOWS
			strcpy(tempFolderName, "./_temp");
		#endif
		#ifdef OS_LINUX
			strcpy(tempFolderName, "/tmp/zemu-XXXXXX");
			if (!mkdtemp(tempFolderName)) _DEBUG("mkdtemp failed");
		#endif

		atexit(FreeAll);

		if (params.cpuTraceEnabled)
		{
			DoCpuStep = TraceCpuStep;
			DoCpuInt = TraceCpuInt;
			CpuTrace_Init();
		}

		InitAll();
		ResetSequence();
		if (argc != 1) ParseCmdLine(argc, argv);
		if (params.sound) InitAudio();

		// [boo_boo]
		C_JoystickManager::Instance()->Init();
		// [/boo_boo]

		if (recordWav) {
			wavFl.Write("output.wav.tmp");
		}

		Process();

		if (recordWav)
		{
			wavFl.Close();

			long wavSz = C_File::FileSize("output.wav.tmp");

			wavFl.Write("output.wav");
			wavFl.PutDWORD(0x46464952);
			wavFl.PutDWORD(wavSz + 0x40 - 8);
			wavFl.PutDWORD(0x45564157);
			wavFl.PutDWORD(0x20746D66);
			wavFl.PutDWORD(0x10);
			wavFl.PutWORD(1);
			wavFl.PutWORD(2);
			wavFl.PutDWORD(44100);
			wavFl.PutDWORD(44100 * 4);
			wavFl.PutWORD(4);
			wavFl.PutWORD(16);
			wavFl.PutDWORD(0x61746164);
			wavFl.PutDWORD(wavSz);

			C_File wavTmp;

			wavTmp.Read("output.wav.tmp");
			while (!wavTmp.Eof()) wavFl.PutBYTE(wavTmp.GetBYTE());

			wavTmp.Close();
			wavFl.Close();

			unlink("output.wav.tmp");
		}

		if (params.cpuTraceEnabled) CpuTrace_Close();
	}
	catch (C_E &e)
	{
		StrikeError("Error %d: %s (%s)", e.exc, e.Descr(), e.param);
	}

	return 0;
}
