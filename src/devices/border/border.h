#ifndef _BORDER_H_INCLUDED_
#define _BORDER_H_INCLUDED_

#include "../../zemu.h"
#include "../device.h"
#include "sound/mixer.h"

#define MAX_SPK_VOL 0x3FFF
#define MAX_TAPE_SAVE_VOL 0x0FFF

class C_Border : public C_Device
{
public:

  static C_SndRenderer sndRenderer;
  static Z80EX_BYTE portFB;

  void Init(void);
  void Close(void);

  static bool OutputByteCheckPort(Z80EX_WORD port);
  static bool OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value);
  static void OnFrameStart(void); // [boo]
  static void OnAfterFrameRender(void); // [boo]
};

#endif
