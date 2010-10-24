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

#include "ftbos_font.h"
#include "font_64.h"

#include "file.h"

#include <cctype>
#include <algorithm>

#define SNAP_FORMAT_Z80 0
#define SNAP_FORMAT_SNA 1

#ifndef Z80EX_ZAME_WRAPPER
	#define Z80EX_CONTEXT_PARAM Z80EX_CONTEXT *cpu,
#else
	#define Z80EX_CONTEXT_PARAM
#endif

unsigned turboMultiplier = 1;
unsigned turboMultiplierNx = 1;
bool unturbo = false;
bool unturboNx = false;

bool isPaused = false;
bool isPausedNx = false;

bool joyOnKeyb = false;

Z80EX_CONTEXT *cpu;
uint64_t clk, devClk, lastDevClk, devClkCounter;
s_Params params;
SDL_Surface *screen, *realScreen, *scrSurf[2];
int PITCH, REAL_PITCH;
bool drawFrame;
int frames;
CConfig config;
C_Font font, fixed_font;
bool disableSound = false;
bool doCopyAfSurfaces = false;

int videoSpec;
int actualWidth;
int actualHeight;

char tempFolderName[MAX_PATH];

bool recordWav = false;
const char *wavFileName = "output.wav"; // TODO: make configurable + full filepath

//--------------------------------------------------------------------------------------------------------------

ptrOnReadByteFunc * devMapRead;
bool (** devMapWrite)(Z80EX_WORD, Z80EX_BYTE);
bool (** devMapInput)(Z80EX_WORD, Z80EX_BYTE&);
bool (** devMapOutput)(Z80EX_WORD, Z80EX_BYTE);

ptrOnReadByteFunc devMapRead_base[0x20000];
bool (* devMapWrite_base[0x10000])(Z80EX_WORD, Z80EX_BYTE);
bool (* devMapInput_base[0x10000])(Z80EX_WORD, Z80EX_BYTE&);
bool (* devMapOutput_base[0x10000])(Z80EX_WORD, Z80EX_BYTE);

ptrOnReadByteFunc devMapRead_trdos[0x20000];
bool (* devMapInput_trdos[0x10000])(Z80EX_WORD, Z80EX_BYTE&);
bool (* devMapOutput_trdos[0x10000])(Z80EX_WORD, Z80EX_BYTE);

//--------------------------------------------------------------------------------------------------------------

struct s_ReadItem
{
	ptrOnReadByteFunc (* check)(Z80EX_WORD, bool);
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

int cnt_z80read = 0;
int cnt_z80write = 0;
int cnt_z80input = 0;
int cnt_z80output = 0;
int cnt_frameStart = 0;
int cnt_afterFrameRender = 0;
int cnt_sdl = 0;
int cnt_reset = 0;

void AttachZ80ReadHandler(ptrOnReadByteFunc (* check)(Z80EX_WORD, bool))
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

/*
 * Work in progress
 *
int c64_colors_base[0x10] = {
	DRGB(0x00, 0x00, 0x00),
	DRGB(0x35, 0x28, 0x79),
	DRGB(0x9A, 0x67, 0x59),
	DRGB(0x6F, 0x3D, 0x86),
	DRGB(0x58, 0x8D, 0x43),
	DRGB(0x70, 0xA4, 0xB2),
	DRGB(0xB8, 0xC7, 0x6F),
	DRGB(0x95, 0x95, 0x95),

	DRGB(0x00, 0x00, 0x00),
	DRGB(0x35, 0x28, 0x79),
	DRGB(0x9A, 0x67, 0x59),
	DRGB(0x6F, 0x3D, 0x86),
	DRGB(0x58, 0x8D, 0x43),
	DRGB(0x70, 0xA4, 0xB2),
	DRGB(0xB8, 0xC7, 0x6F),
	DRGB(0xFF, 0xFF, 0xFF)
};
*/

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

void LoadNormalFile(char *fname, int drive)
{
	if (C_Tape::IsTapeFormat(fname)) {
		C_Tape::Insert(fname);
	} else if (!stricmp(C_DirWork::ExtractExt(fname), "z80")) {
		if (load_z80_snap(fname, cpu, dev_mman, dev_border)) {
			dev_tsfm.OnReset();
		} else {
			StrikeMessage("Error loading snapshot");
		}
	} else if (!stricmp(C_DirWork::ExtractExt(fname), "sna")) {
		if (load_sna_snap(fname, cpu, dev_mman, dev_border)) {
			dev_tsfm.OnReset();
		} else {
			StrikeMessage("Error loading snapshot");
		}
	} else {
		wd1793_load_dimage(fname, drive);
	}
}

bool TryLoadArcFile(char *arcName, int drive)
{
	C_File fl;
	char res[MAX_PATH];
	char tmp[MAX_PATH], str[MAX_FNAME];
	char files[MAX_FILES][MAX_FNAME];
	int filesCount;
	string plugin_fn;

	strcpy(tmp, C_DirWork::ExtractExt(arcName));
	StrToLower(tmp);

	plugin_fn = config.FindDataFile("arc", tmp);

	if (plugin_fn.empty()) {
		return false;
	}

	sprintf(tmp, "%s list %s %s/files.txt", plugin_fn.c_str(), arcName, tempFolderName);
	if (system(tmp) == -1) DEBUG_MESSAGE("system failed");

	sprintf(tmp, "%s/files.txt", tempFolderName);
	if (!C_File::FileExists(tmp)) return true; // "true" here means ONLY that the file is an archive

	fl.Read(tmp);

	for (filesCount = 0; !fl.Eof(); )
	{
		fl.GetS(str, sizeof(str));
		if (strcmp(str, "")) strcpy(files[filesCount++], str);
	}

	unlink(tmp);
	if (!filesCount) return true; // "true" here means ONLY that the file is an archive

	// currently load only first file
	sprintf(tmp, "%s extract %s %s %s", plugin_fn.c_str(), arcName, files[0], tempFolderName);
	if (system(tmp) == -1) DEBUG_MESSAGE("system failed");
	strcpy(tmp, C_DirWork::ExtractFileName(files[0]));

	// TODO: check if lresult strlen > sizeof(res)
	sprintf(res, "%s/%s", tempFolderName, tmp);

	LoadNormalFile(res, drive);
	unlink(res);

	return true; // "true" here means ONLY that the file is an archive
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

	if (!TryLoadArcFile(tname, drive)) {
		LoadNormalFile(tname, drive);
	}
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
#ifdef __linux__
	SDL_WM_ToggleFullScreen(realScreen);
#else
	videoSpec ^= SDL_FULLSCREEN;
	SDL_FreeSurface(realScreen);
	realScreen = SDL_SetVideoMode(actualWidth, actualHeight, 32, videoSpec);
	REAL_PITCH = realScreen->pitch / 4;

	if (!params.scale2x)
	{
		screen = realScreen;
		PITCH = REAL_PITCH;
	}
#endif
}

void Action_Debugger(void)
{
	isPaused = false;
	RunDebugger();
}

void Action_Turbo(void)
{
	isPaused = false;

	if (unturboNx && turboMultiplierNx > 1)
	{
		turboMultiplierNx /= 2;
		unturboNx = (turboMultiplierNx > 1);
	}
	else
	{
		unturboNx = false;
		turboMultiplierNx = (turboMultiplierNx == 8 ? 1 : (turboMultiplierNx * 2));
	}

	DisplayTurboMessage();
}

void Action_UnTurbo(void)
{
	isPaused = false;

	if (!unturboNx && turboMultiplierNx > 1)
	{
		turboMultiplierNx /= 2;
	}
	else
	{
		turboMultiplierNx = (turboMultiplierNx == 256 ? 1 : (turboMultiplierNx * 2));
		unturboNx = (turboMultiplierNx > 1);
	}

	DisplayTurboMessage();
}

void DisplayTurboMessage(void)
{
	if (turboMultiplierNx < 2)
	{
		SetMessage("Turbo OFF");
	}
	else
	{
		char buf[0x10];
		sprintf(buf, (unturboNx ? "Slowness %dx" : "Turbo %dx"), turboMultiplierNx);
		SetMessage(buf);
	}
}

void Action_AttributesHack(void)
{
	isPaused = false;
	attributesHack = !attributesHack;
	SetMessage(attributesHack ? "AttributesHack ON" : "AttributesHack OFF");
}

void Action_ScreensHack(void)
{
	isPaused = false;
	screensHack = screensHack ^ 8;
	SetMessage(screensHack ? "ScreensHack ON" : "ScreensHack OFF");
}

void Action_FlashColor(void)
{
	isPaused = false;
	flashColor = !flashColor;
	SetMessage(flashColor ? "FlashColor ON" : "FlashColor OFF");
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
	{"unturbo",			Action_UnTurbo},
	{"attrib_hack",		Action_AttributesHack},
	{"screens_hack",	Action_ScreensHack},
	{"flash_color",		Action_FlashColor},
	{"pause",			Action_Pause},
	{"joy_on_keyb",		Action_JoyOnKeyb},
	{"",				NULL}
};

//--------------------------------------------------------------------------------------------------------------

bool runDebuggerFlag = false;
bool breakpoints[0x10000];

Z80EX_WORD watches[MAX_WATCHES];
unsigned watchesCount = 0;

Z80EX_BYTE ReadByteDasm(Z80EX_WORD addr, void *userData)
{
    ptrOnReadByteFunc func = devMapRead[addr];
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

Z80EX_BYTE ReadByte(Z80EX_CONTEXT_PARAM Z80EX_WORD addr, int m1_state, void *userData)
{
	unsigned raddr = addr + (m1_state ? 0x10000 : 0);
	ptrOnReadByteFunc func = devMapRead[raddr];
	return func(addr, m1_state);
}

void WriteByte(Z80EX_CONTEXT_PARAM Z80EX_WORD addr, Z80EX_BYTE value, void *userData)
{
	for (;;)
	{
		bool (* func)(Z80EX_WORD, Z80EX_BYTE) = devMapWrite[addr];

		if (func == NULL) return;
		if (func(addr, value)) return;
	}
}

Z80EX_BYTE InputByte(Z80EX_CONTEXT_PARAM Z80EX_WORD port, void *userData)
{
	Z80EX_BYTE retval;

	for (;;)
	{
		bool (* func)(Z80EX_WORD, Z80EX_BYTE&) = devMapInput[port];

		if (func == NULL) return 0xFF;
		if (func(port, retval)) return retval;
	}
}

void OutputByte(Z80EX_CONTEXT_PARAM Z80EX_WORD port, Z80EX_BYTE value, void *userData)
{
	for (;;)
	{
		bool (* func)(Z80EX_WORD, Z80EX_BYTE) = devMapOutput[port];

		if (func == NULL) return;
		if (func(port, value)) return;
	}
}

Z80EX_BYTE ReadIntVec(Z80EX_CONTEXT_PARAM void *userData)
{
	return 0xFF;
}

//--------------------------------------------------------------------------------------------------------------

void InitDevMapRead(ptrOnReadByteFunc * map)
{
	ptrOnReadByteFunc func;

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

	fixed_font.Init(font_64_data);
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
#define INT_BEGIN (68068+1)

bool attributesHack = false;
bool flashColor = false;
int screensHack = 0;

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
	uint8_t *s1, *s2, *sr;
	uint8_t *s1w, *s2w, *srw;

	if (SDL_MUSTLOCK(screen)) {if (SDL_LockSurface(screen) < 0) return;}
	if (SDL_MUSTLOCK(scrSurf[0])) {if (SDL_LockSurface(scrSurf[0]) < 0) return;}
	if (SDL_MUSTLOCK(scrSurf[1])) {if (SDL_LockSurface(scrSurf[1]) < 0) return;}

	if (doCopyAfSurfaces)
	{
		s1 = (uint8_t *)copyFrom->pixels;
		s2 = (uint8_t *)copyTo->pixels;

		for (i = HEIGHT; i--;)
		{
			uint32_t *s1dw = (uint32_t *)s1;
			uint32_t *s2dw = (uint32_t *)s2;

			for (j = WIDTH; j--;) { *(s2dw++) = *(s1dw++); }

			s1 += copyFrom->pitch;
			s2 += copyTo->pitch;
		}

		doCopyAfSurfaces = false;
	}

	sr = (uint8_t *)screen->pixels;
	s1 = (uint8_t *)scrSurf[0]->pixels;
	s2 = (uint8_t *)scrSurf[1]->pixels;

	for (i = HEIGHT; i--;)
	{
		srw = sr;
		s1w = s1;
		s2w = s2;

		for (j = WIDTH; j--;)
		{
			*srw = (uint8_t)(((unsigned int)(*s1w) + (unsigned int)(*s2w)) >> 1);
			srw++; s1w++; s2w++;

			*srw = (uint8_t)(((unsigned int)(*s1w) + (unsigned int)(*s2w)) >> 1);
			srw++; s1w++; s2w++;

			*srw = (uint8_t)(((unsigned int)(*s1w) + (unsigned int)(*s2w)) >> 1);
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

uint64_t actDevClkCounter = 0;
uint64_t actClk = 0;

void InitActClk(void)
{
	actDevClkCounter = devClkCounter * (uint64_t)turboMultiplier;
	actClk = clk * (uint64_t)turboMultiplier;
}

// TODO:
// CpuCalcTacts - pointer to function
// CpuCalcTacts_Normal
// CpuCalcTacts_Turbo
// CpuCalcTacts_Unturbo
// DebugCpuCalcTacts_Normal
// ...
// ...
// TODO: refactor

inline void CpuCalcTacts(unsigned long cmdClk)
{
	if (turboMultiplier < 2)
	{
		devClkCounter += (uint64_t)cmdClk;
		clk += (uint64_t)cmdClk;
	}
	else if (unturbo)
	{
		cmdClk *= turboMultiplier;
		devClkCounter += (uint64_t)cmdClk;
		clk += (uint64_t)cmdClk;
	}
	else
	{
		actDevClkCounter += (uint64_t)cmdClk;
		actClk += (uint64_t)cmdClk;

		devClkCounter = (actDevClkCounter + (uint64_t)(turboMultiplier-1)) / (uint64_t)turboMultiplier;
		clk = (actClk + (uint64_t)(turboMultiplier-1)) / (uint64_t)turboMultiplier;
	}

	devClk = clk;
	C_Tape::Process();

	if (runDebuggerFlag)
	{
		runDebuggerFlag = false;
		RunDebugger();
	}
	else if (breakpoints[z80ex_get_reg(cpu, regPC)])
	{
		RunDebugger();
	}
}

inline void DebugCpuCalcTacts(unsigned long cmdClk)
{
	if (turboMultiplier < 2)
	{
		devClkCounter += (uint64_t)cmdClk;
		clk += (uint64_t)cmdClk;
	}
	else if (unturbo)
	{
		cmdClk *= turboMultiplier;
		devClkCounter += (uint64_t)cmdClk;
		clk += (uint64_t)cmdClk;
	}
	else
	{
		actDevClkCounter += (uint64_t)cmdClk;
		actClk += (uint64_t)cmdClk;

		devClkCounter = (actDevClkCounter + (uint64_t)(turboMultiplier-1)) / (uint64_t)turboMultiplier;
		clk = (actClk + (uint64_t)(turboMultiplier-1)) / (uint64_t)turboMultiplier;
	}

	devClk = clk;
	C_Tape::Process();
}

int (* DoCpuStep)(Z80EX_CONTEXT *cpu) = z80ex_step;
int (* DoCpuInt)(Z80EX_CONTEXT *cpu) = z80ex_int;

int TraceCpuStep(Z80EX_CONTEXT *cpu)
{
	CpuTrace_Log();
	cpuTrace_intReq = 0;
	cpuTrace_dT = z80ex_step(cpu);
	return cpuTrace_dT;
}

int TraceCpuInt(Z80EX_CONTEXT *cpu)
{
	CpuTrace_Log();
	int dt = z80ex_int(cpu);
	cpuTrace_dT += dt;
	cpuTrace_intReq++;
	return dt;
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

		int wdt = fixed_font.StrLenPx(buf);
		fixed_font.PrintString(WIDTH - 4 - wdt, 4 + fixed_font.Height() * (i+1), buf);
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
				for (i = 0; i < cnt_sndRenderers; i++) fixed_font.PrintString(16+i*4, 0, sndRenderers[i]->activeCnt ? "#" : "-");
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

			SoundMixer.FlushFrame(SOUND_ENABLED);
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
			if (DlgConfirm("Do you really want to quit?")) {
				return;
			}
		}

		turboMultiplier = turboMultiplierNx;
		unturbo = unturboNx;

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
	if (params.sndBackend == SND_BACKEND_SDL) {
		SoundMixer.InitBackendSDL(params.sdlBufferSize);
	}
#ifndef _WIN32
	else if (params.sndBackend == SND_BACKEND_OSS) {
		SoundMixer.InitBackendOSS(params.soundParam);
	}
#else
	else if (params.sndBackend == SND_BACKEND_WIN32) {
		SoundMixer.InitBackendWin32(params.soundParam);
	}
#endif // !_WIN32
	SoundMixer.Init(&params.mixerMode, recordWav, wavFileName);
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

	#ifdef __linux__
		// TODO: do in in more portable way
		char cmd[MAX_PATH];
		strcpy(cmd, "rm -rf ");
		strcat(cmd, tempFolderName);
		if (system(cmd) == -1) DEBUG_MESSAGE("system() failed");
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

	if (CConfig::executableDir) {
		free(CConfig::executableDir);
	}
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
	printf("                                      \n");
	printf("   $ww,.                              \n");
	printf("    `^$$$ww,.                         \n");
	printf("       `^$$$$$$                       \n");
	printf("         ,$$7'                        \n");
	printf("       _j$$'   __    __  _ _          \n");
	printf("     ,j$$7      /__ (-_ | ) ) (_|     \n");
	printf("    $$$$$$w.                          \n");
	printf("     `^^T$$$w,             rst'o6     \n");
	printf("           `^T$$                      \n");
	printf("                                      \n");
	printf(" restorer   [ restorer.fct@gmail.com ]\n");
	printf(" boo_boo    [    boo_boo@inbox.ru    ]\n");
	printf(" mastermind [   zxmmind@gmail.com    ]\n");
	printf("                                      \n");
	printf(" breeze (gfx) [ breeze.fbn@gmail.com ]\n");
	printf("                                      \n");
	printf(" with help of SMT                     \n");
	printf("                                      \n");
}

int main(int argc, char *argv[])
{
	OutputLogo();

	if (argc > 0) {		// just for case
		CConfig::executableDir = AllocNstrcpy(C_DirWork::ExtractPath(argv[0]));
	} else {
		CConfig::executableDir = AllocNstrcpy(".");
	}

	config.Initialize("zemu");

	try
	{
		string str;

		// core
		str = config.GetString("core", "snapformat", "sna");
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);

		if (str == "sna") params.snapFormat = SNAP_FORMAT_SNA;
		else params.snapFormat = SNAP_FORMAT_Z80;

		// beta128
		str = config.GetString("beta128", "diskA", "");
		if (!str.empty()) wd1793_load_dimage(str.c_str(), 0);

		str = config.GetString("beta128", "diskB", "");
		if (!str.empty()) wd1793_load_dimage(str.c_str(), 1);

		str = config.GetString("beta128", "diskC", "");
		if (!str.empty()) wd1793_load_dimage(str.c_str(), 2);

		str = config.GetString("beta128", "diskD", "");
		if (!str.empty()) wd1793_load_dimage(str.c_str(), 3);

		wd1793_set_nodelay(config.GetBool("beta128", "nodelay", false));

		str = config.GetString("beta128", "sclboot", "boot.$b");

		str = config.FindDataFile("boot", str.c_str());
		if (!str.empty()) wd1793_set_appendboot(str.c_str());

		// display
		params.fullscreen = config.GetBool("display", "fullscreen", false);
		params.scale2x = config.GetBool("display", "scale2x", true);
		params.scanlines = config.GetBool("display", "scanlines", false);
		params.useFlipSurface = config.GetBool("display", "sdl_useflipsurface", false);
		params.antiFlicker = config.GetBool("display", "antiflicker", false);
		params.showInactiveIcons = config.GetBool("display", "showinactiveicons", false);

		// input
		params.mouseDiv = config.GetInt("input", "mousediv", 1);
		if (params.mouseDiv <= 0) params.mouseDiv = 1;
		else if (params.mouseDiv > 8) params.mouseDiv = 8;

		// sound
		params.sound = config.GetBool("sound", "enable", true);
		params.mixerMode = config.GetInt("sound", "mixermode", 1);

#ifdef _WIN32
		eSndBackend default_snd_backend = SND_BACKEND_WIN32;
#else
		eSndBackend default_snd_backend = SND_BACKEND_SDL;
#endif

		str = config.GetString("sound", "sound_backend", "auto");
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);
		if (str == "sdl") params.sndBackend = SND_BACKEND_SDL;

#ifndef _WIN32
		else if (str == "oss") params.sndBackend = SND_BACKEND_OSS;
#else
		else if (str == "win32") params.sndBackend = SND_BACKEND_WIN32;
#endif

		else params.sndBackend = default_snd_backend;
		params.sdlBufferSize = config.GetInt("sound", "sdlbuffersize", 4);

#ifdef __linux__
		params.soundParam = config.GetInt("sound", "ossfragnum", 8);
#endif
#ifdef _WIN32
		params.soundParam = config.GetInt("sound", "wqsize", 4);
#endif

		// cputrace
		params.cpuTraceEnabled = config.GetBool("cputrace", "enable", false);
		str = config.GetString("cputrace", "format", "[PC]> [M1]:[M2] dT=[DT] AF=[AF] BC=[BC] DE=[DE] HL=[HL] IX=[IX] IY=[IY] SP=[SP] I=[I] R=[R] AF'=[AF'] BC'=[BC'] DE'=[DE'] HL'=[HL'] IFF1=[IFF1] IFF2=[IFF2] IM=[IM] INTR=[INTR]");
		strcpy(params.cpuTraceFormat, str.c_str());
		str = config.GetString("cputrace", "filename", "cputrace.log");
		strcpy(params.cpuTraceFileName, str.c_str());

		int spec = SDL_INIT_VIDEO;
		if (params.sound && params.sndBackend == SND_BACKEND_SDL) spec |= SDL_INIT_AUDIO;
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

		#ifdef _WIN32
			strcpy(tempFolderName, "./_temp");
		#else
			strcpy(tempFolderName, "/tmp/zemu-XXXXXX");
			if (!mkdtemp(tempFolderName)) DEBUG_MESSAGE("mkdtemp failed");
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

		if (config.GetBool("core", "trdos_at_start", false))
		{
			dev_mman.OnOutputByte(0x7FFD, 0x10);
			dev_trdos.Enable();
		}

		if (argc != 1) ParseCmdLine(argc, argv);
		if (params.sound) InitAudio();

		// [boo_boo]
		C_JoystickManager::Instance()->Init();
		// [/boo_boo]

		Process();

		if (params.cpuTraceEnabled) CpuTrace_Close();
	}
	catch (C_E &e)
	{
		StrikeError("Error %d: %s", e.exc, e.Descr());
	}

	return 0;
}
