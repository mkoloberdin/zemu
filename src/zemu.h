#ifndef _ZEMU_H_INCLUDED_
#define _ZEMU_H_INCLUDED_

#include "defines.h"
#include <SDL.h>
#include <z80ex_wrapper.h>
#include "config.h"
#include "sound/mixer.h"

#define WIDTH 320
#define HEIGHT 240

#define FRAME_WAIT_MS 20
#define MAX_SPEED_FRAME_SKIP 64

#define MAX_DEV_CLK 72000

#define MAX_TRACE_FORMAT 0x100

#define SOUND_ENABLED (!params.maxSpeed && params.sound)

struct s_Action
{
	const char *name;
	void (* action)(void);
};

struct s_Params
{
	bool useFlipSurface;
	bool fullscreen;
	bool scale2x;
	bool scanlines;
	bool sound;
	bool maxSpeed;
	bool antiFlicker;
	eSndBackend sndBackend;
	int sdlBufferSize;
	int mouseDiv;
	bool showInactiveIcons;
	int soundParam;
	bool cpuTraceEnabled;
	char cpuTraceFormat[MAX_TRACE_FORMAT];
	char cpuTraceFileName[MAX_PATH];
	int mixerMode;
	int snapFormat;

	// [boo_boo]
	bool kempstonEnable;
	int kempstonOnStickNum;
	int kempstonAxisTreshold;
};

extern CConfig config;
extern Z80EX_CONTEXT *cpu;
extern uint64_t clk, devClk, lastDevClk, devClkCounter;
extern s_Params params;
extern SDL_Surface *screen;
extern int PITCH;
extern bool drawFrame;
extern int frames;
extern bool disableSound;
extern char tempFolderName[MAX_PATH];

extern s_Action cfgActions[];

//--------------------------------------------------------------------------------------------------------------

#define MAX_HANDLERS 64

typedef Z80EX_BYTE(*ptrOnReadByteFunc)(Z80EX_WORD, bool);

void AttachZ80ReadHandler(ptrOnReadByteFunc (* check)(Z80EX_WORD, bool));
void AttachZ80WriteHandler(bool (* check)(Z80EX_WORD), bool (* func)(Z80EX_WORD, Z80EX_BYTE));
void AttachZ80InputHandler(bool (* check)(Z80EX_WORD), bool (* func)(Z80EX_WORD, Z80EX_BYTE&));
void AttachZ80OutputHandler(bool (* check)(Z80EX_WORD), bool (* func)(Z80EX_WORD, Z80EX_BYTE));
void AttachFrameStartHandler(void (* func)(void));
void AttachAfterFrameRenderHandler(void (* func)(void));
void AttachSDLHandler(int eventType, bool (* func)(SDL_Event&));
void AttachResetHandler(void (* func)(void));

extern ptrOnReadByteFunc * devMapRead;
extern bool (** devMapWrite)(Z80EX_WORD, Z80EX_BYTE);
extern bool (** devMapInput)(Z80EX_WORD, Z80EX_BYTE&);
extern bool (** devMapOutput)(Z80EX_WORD, Z80EX_BYTE);

extern ptrOnReadByteFunc devMapRead_base[0x20000];
extern bool (* devMapWrite_base[0x10000])(Z80EX_WORD, Z80EX_BYTE);
extern bool (* devMapInput_base[0x10000])(Z80EX_WORD, Z80EX_BYTE&);
extern bool (* devMapOutput_base[0x10000])(Z80EX_WORD, Z80EX_BYTE);

extern ptrOnReadByteFunc devMapRead_trdos[0x20000];
extern bool (* devMapInput_trdos[0x10000])(Z80EX_WORD, Z80EX_BYTE&);
extern bool (* devMapOutput_trdos[0x10000])(Z80EX_WORD, Z80EX_BYTE);

//--------------------------------------------------------------------------------------------------------------

extern bool joyOnKeyb;
extern bool attributesHack;
extern int screensHack;		// 0 = no hack, 8 = swap screens
extern bool flashColor;
extern int colors_base[0x10];
extern int colors[0x10];
extern unsigned turboMultiplierNx;
extern bool unturboNx;

//--------------------------------------------------------------------------------------------------------------

extern bool breakpoints[0x10000];

#define MAX_WATCHES 0x10
extern uint16_t watches[MAX_WATCHES];
extern unsigned watchesCount;

Z80EX_BYTE ReadByteDasm(Z80EX_WORD addr, void *userData);
void WriteByteDasm(Z80EX_WORD addr, Z80EX_BYTE value);
void DebugStep(void);

//--------------------------------------------------------------------------------------------------------------

void TryNLoadFile(char *fname);
void TryNLoadFile(char *fname, int drive);
void ScaleImage(void);
void UpdateScreen(void);
void DisplayTurboMessage(void);

//--------------------------------------------------------------------------------------------------------------

#endif
