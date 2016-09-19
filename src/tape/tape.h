#ifndef _TAPE_H_INCLUDED_
#define _TAPE_H_INCLUDED_

#include "../zemu.h"
#include "sound/mixer.h"
#include "tape_format.h"

#define MAX_TAPE_VOL 0x03FF

class C_Tape
{
public:

  static C_SndRenderer sndRenderer;

  static uint64_t prevDevClkCounter;
  static C_TapeFormat *currentFormat;

  static void Init(void);
  static void Close(void);
  static void OnFrameStart(void);
  static void OnAfterFrameRender(void);

  static int getCurrBit(void);
  static void process(void);

  static bool isLoaded(void);
  static bool isActive(void);
  static unsigned int getPosPerc(void);

  static bool isTapeFormat(const fs::path& fname);

  static void eject(void);
  static bool insert(const fs::path& fname);
  static void start(void);
  static void stop(void);
  static void rewind(void);
};

#endif
