#ifndef _TRDOS_INCLUDED_
#define _TRDOS_INCLUDED_

#include <z80ex.h>
#include <zemu.h>
#include "../device.h"
#include "../../lib_wd1793/wd1793_chip.h"

class C_TrDos : public C_Device
{
public:

  static bool trdos;
  static Z80EX_BYTE rom[0x4000];

  static void ReadFile(void);
  void Init(void);
  void Close(void);

  static ptrOnReadByteFunc ReadByteCheckAddr(Z80EX_WORD addr, bool m1);
  static Z80EX_BYTE OnReadByte_3Dxx_M1(Z80EX_WORD addr, bool m1);
  static Z80EX_BYTE OnReadByte_RAM_M1(Z80EX_WORD addr, bool m1);
  static Z80EX_BYTE OnReadByte_ROM(Z80EX_WORD addr, bool m1);
  static bool InputOutputByteCheckPort(Z80EX_WORD port);
  static bool OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval);
  static bool OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value);
  static void OnReset(void);

  static void Enable(void);
  static void Disable(void);
  static DRIVE_STATE GetIndicatorState(void);
};

#endif
