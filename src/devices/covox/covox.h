#ifndef _COVOX_H_INCLUDED_
#define _COVOX_H_INCLUDED_

#include <cstdint>
#include "../device.h"
#include "sound/mixer.h"

class C_Covox : public C_Device
{
public:

  static C_SndRenderer sndRenderer;
  static bool enabled;

  void Init(void);
  void Close(void);

  static bool OutputByteCheckPort(uint16_t);
  static bool OnOutputByte(uint16_t port, uint8_t value);
  static void OnFrameStart(void);
  static void OnAfterFrameRender(void);
  static void OnReset(void);
};

#endif
