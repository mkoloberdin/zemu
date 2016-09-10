#ifndef _COVOX_H_INCLUDED_
#define _COVOX_H_INCLUDED_

#include <z80ex.h>
#include "../device.h"
#include "sound/mixer.h"

class C_Covox : public C_Device
{
public:

  static C_SndRenderer sndRenderer;
  static bool enabled;

  void Init(void);
  void Close(void);

  static bool OutputByteCheckPort(Z80EX_WORD);
  static bool OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value);
  static void OnFrameStart(void);
  static void OnAfterFrameRender(void);
  static void OnReset(void);
};

#endif
