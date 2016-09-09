#ifndef _MMANAGER_H_INCLDUED_
#define _MMANAGER_H_INCLDUED_

#include "../../zemu.h"
#include "../device.h"

#define RAM_BANK2 (0x4000 * 2)
#define RAM_BANK5 (0x4000 * 5)
#define RAM_BANK7 (0x4000 * 7)

#define RAM_BANK4 (0x4000 * 4)
#define RAM_BANK6 (0x4000 * 6)

string split_romname(string &romname, size_t *offset);

class C_MemoryManager : public C_Device
{
public:

  static Z80EX_BYTE port7FFD;
  static Z80EX_BYTE rom[0x8000];
  static Z80EX_BYTE ram[0x4000 * 64];

  static Z80EX_BYTE *rom_map, *ram_map;
  static bool enable512;
  static bool enable1024;

  static void ReadFile(void);
  void Init(void);
  void Close(void);

  static void Remap(void);
  static ptrOnReadByteFunc ReadByteCheckAddr(Z80EX_WORD addr, bool m1);
  static Z80EX_BYTE OnReadByte_ROM(Z80EX_WORD addr, bool m1);
  static Z80EX_BYTE OnReadByte_Bank5(Z80EX_WORD addr, bool m1);
  static Z80EX_BYTE OnReadByte_Bank2(Z80EX_WORD addr, bool m1);
  static Z80EX_BYTE OnReadByte_C000(Z80EX_WORD addr, bool m1);
  static bool WriteByteCheckAddr(Z80EX_WORD addr);
  static bool OnWriteByte(Z80EX_WORD addr, Z80EX_BYTE value);
  static bool OutputByteCheckPort(Z80EX_WORD port);
  static bool OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value);
  static void OnReset(void);
};

#endif
