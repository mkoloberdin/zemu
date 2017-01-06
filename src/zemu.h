#ifndef _ZEMU_H_INCLUDED_
#define _ZEMU_H_INCLUDED_

#include "defines.h"
#include <z80ex.h>
#include "sound/mixer.h"

#include "params.h"
#define SOUND_ENABLED (!params.maxSpeed && params.sound)


#include "platform/platform.h"
extern IPlatform *platform;

struct s_Action
{
  const char *name;
  void (*action)(void);
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
  int sdlBufferSize; // TODO: move to SDL platform
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

extern Z80EX_CONTEXT *cpu;
extern uint64_t cpuClk, devClk, lastDevClk, devClkCounter;
extern s_Params params;
extern int PITCH;
extern bool drawFrame;
extern int frames;
extern bool disableSound;
extern char tempFolderName[MAX_PATH];

extern s_Action cfgActions[];

//--------------------------------------------------------------------------------------------------

#define MAX_HANDLERS 64

typedef uint8_t(*ptrOnReadByteFunc)(uint16_t, bool);

void AttachZ80ReadHandler(ptrOnReadByteFunc(*check)(uint16_t, bool));
void AttachZ80WriteHandler(bool (*check)(uint16_t),
                           bool (*func)(uint16_t, uint8_t));
void AttachZ80InputHandler(bool (*check)(uint16_t),
                           bool (*func)(uint16_t, uint8_t &));
void AttachZ80OutputHandler(bool (*check)(uint16_t),
                            bool (*func)(uint16_t, uint8_t));
void AttachFrameStartHandler(void (*func)(void));
void AttachAfterFrameRenderHandler(void (*func)(void));
void AttachResetHandler(void (*func)(void));

extern ptrOnReadByteFunc *devMapRead;
extern bool (**devMapWrite)(uint16_t, uint8_t);
extern bool (**devMapInput)(uint16_t, uint8_t &);
extern bool (**devMapOutput)(uint16_t, uint8_t);

extern ptrOnReadByteFunc devMapRead_base[0x20000];
extern bool (*devMapWrite_base[0x10000])(uint16_t, uint8_t);
extern bool (*devMapInput_base[0x10000])(uint16_t, uint8_t &);
extern bool (*devMapOutput_base[0x10000])(uint16_t, uint8_t);

extern ptrOnReadByteFunc devMapRead_trdos[0x20000];
extern bool (*devMapInput_trdos[0x10000])(uint16_t, uint8_t &);
extern bool (*devMapOutput_trdos[0x10000])(uint16_t, uint8_t);

//--------------------------------------------------------------------------------------------------

extern bool joyOnKeyb;
extern int attributesHack;
extern int screensHack;   // 0 = no hack, 8 = swap screens
extern bool flashColor;
extern int colors_base[0x10];
extern int colors[0x10];
extern unsigned turboMultiplierNx;
extern bool unturboNx;

//--------------------------------------------------------------------------------------------------

extern bool breakpoints[0x10000];

#define MAX_WATCHES 0x10
extern uint16_t watches[MAX_WATCHES];
extern unsigned watchesCount;

uint8_t ReadByteDasm(uint16_t addr, void *userData);
void WriteByteDasm(uint16_t addr, uint8_t value);
void DebugStep(void);

//extern SDL_Surface *renderSurf;
extern PixBuf const *pixBuf;
extern int renderPitch;
extern unsigned long prevRenderClk;
extern void (*renderPtr)(unsigned long);

//--------------------------------------------------------------------------------------------------

void TryNLoadFile(const fs::path& fname, int drive = 0);
void UpdateScreen(void);
void DisplayTurboMessage(void);

//--------------------------------------------------------------------------------------------------

#endif
