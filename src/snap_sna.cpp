#include <stdio.h>
#include <cstdint>
#include "snap_sna.h"
#include "exceptions.h"
#include "zemu.h"
#include "file.h"

bool load_sna_snap(const char *filename, Z80EX_CONTEXT *cpu, C_MemoryManager &mmgr, C_Border &border)
{
  C_File fl;
  uint8_t border_color;
  bool banks[8];
  uint8_t buffer[16384];
  uint8_t retval;
  uint8_t port_7ffd;
  uint8_t is_trdos;

  try {
    fl.Read(filename);
  } catch (C_E &e) {
    return false;
  }

  z80ex_set_reg(cpu, regI, fl.GetBYTE());
  z80ex_set_reg(cpu, regHL_, fl.GetWORD());
  z80ex_set_reg(cpu, regDE_, fl.GetWORD());
  z80ex_set_reg(cpu, regBC_, fl.GetWORD());
  z80ex_set_reg(cpu, regAF_, fl.GetWORD());
  z80ex_set_reg(cpu, regHL, fl.GetWORD());
  z80ex_set_reg(cpu, regDE, fl.GetWORD());
  z80ex_set_reg(cpu, regBC, fl.GetWORD());
  z80ex_set_reg(cpu, regIY, fl.GetWORD());
  z80ex_set_reg(cpu, regIX, fl.GetWORD());

  uint8_t intm = fl.GetBYTE();

  z80ex_set_reg(cpu, regIFF1, intm & 1);
  z80ex_set_reg(cpu, regIFF2, (intm >> 1) & 1);

  z80ex_set_reg(cpu, regR, fl.GetBYTE());
  z80ex_set_reg(cpu, regAF, fl.GetWORD());
  z80ex_set_reg(cpu, regSP, fl.GetWORD());
  z80ex_set_reg(cpu, regIM, fl.GetBYTE());

  border_color = fl.GetBYTE();

  mmgr.OnReset();
  for (int i = 0; i < 8; i++) banks[i] = false;

  for (int i = 0; i < 16384; i++) mmgr.OnWriteByte(i + 16384, fl.GetBYTE());
  banks[5] = true;

  for (int i = 0; i < 16384; i++) mmgr.OnWriteByte(i + 32768, fl.GetBYTE());
  banks[2] = true;

  for (int i = 0; i < 16384; i++) buffer[i] = fl.GetBYTE();

  if (fl.Eof())
  {
    mmgr.OnOutputByte(0x7ffd, 0x30);	// set 48k mode
    for (int i = 0; i < 16384; i++) mmgr.OnWriteByte(i + 49152, buffer[i]);

    uint16_t sp = z80ex_get_reg(cpu, regSP);

    retval = ReadByteDasm(sp, nullptr);
    uint16_t pc = retval;
    mmgr.OnWriteByte(sp, 0);
    sp++;

    retval = ReadByteDasm(sp, nullptr);
    pc |= ((uint16_t)retval << 8);
    mmgr.OnWriteByte(sp, 0);
    sp++;

    z80ex_set_reg(cpu, regSP, sp);
    z80ex_set_reg(cpu, regPC, pc);
  }
  else
  {
    z80ex_set_reg(cpu, regPC, fl.GetWORD());

    port_7ffd = fl.GetBYTE();
    is_trdos = fl.GetBYTE();

    mmgr.OnOutputByte(0x7ffd, port_7ffd & 7);
    for (int i = 0; i < 16384; i++) mmgr.OnWriteByte(i + 49152, buffer[i]);
    banks[port_7ffd & 7] = true;

    for (int k = 0; k < 8; k++)
    {
      if (banks[k]) continue;

      mmgr.OnOutputByte(0x7ffd, k);
      for (int i = 0; i < 16384; i++) mmgr.OnWriteByte(i + 49152, fl.GetBYTE());
    }

    C_TrDos::trdos = (is_trdos != 0);
    mmgr.OnOutputByte(0x7ffd, port_7ffd);
  }

  // set border color
  border.OnOutputByte(0x00FE, border_color);
  fl.Close();
  return true;
}

void save_sna_snap(const char *filename, Z80EX_CONTEXT *cpu, C_MemoryManager &mmgr, C_Border &border)
{
  C_File fl;
  bool banks[8];

  fl.Write(filename);

  fl.PutBYTE(z80ex_get_reg(cpu, regI));
  fl.PutWORD(z80ex_get_reg(cpu, regHL_));
  fl.PutWORD(z80ex_get_reg(cpu, regDE_));
  fl.PutWORD(z80ex_get_reg(cpu, regBC_));
  fl.PutWORD(z80ex_get_reg(cpu, regAF_));
  fl.PutWORD(z80ex_get_reg(cpu, regHL));
  fl.PutWORD(z80ex_get_reg(cpu, regDE));
  fl.PutWORD(z80ex_get_reg(cpu, regBC));
  fl.PutWORD(z80ex_get_reg(cpu, regIY));
  fl.PutWORD(z80ex_get_reg(cpu, regIX));

  fl.PutBYTE((z80ex_get_reg(cpu, regIFF1) ? 1 : 0) | (z80ex_get_reg(cpu, regIFF2) ? 2 : 0));
  uint16_t sp = z80ex_get_reg(cpu, regSP);

  if (mmgr.port7FFD == 0x30)
  {
    // 48k
    sp -= 2;
  }

  fl.PutBYTE(z80ex_get_reg(cpu, regR));
  fl.PutWORD(z80ex_get_reg(cpu, regAF));
  fl.PutWORD(sp);
  fl.PutBYTE(z80ex_get_reg(cpu, regIM));

  fl.PutBYTE(border.portFB & 7);

  uint8_t buffer[0xC000];
  for (int i = 0; i < 8; i++) banks[i] = false;

  for (int i = 0; i < 0x4000; i++) buffer[i] = mmgr.ram[5 * 0x4000 + i];
  banks[5] = true;

  for (int i = 0; i < 0x4000; i++) buffer[i + 0x4000] = mmgr.ram[2 * 0x4000 + i];
  banks[2] = true;

  uint16_t pc = z80ex_get_reg(cpu, regPC);

  if (mmgr.port7FFD == 0x30)  // 48k
  {
    for (int i = 0; i < 16384; i++) buffer[i + 0x8000] = mmgr.ram[0 * 0x4000 + i];

    if (sp >= 0x4000) buffer[sp - 0x4000] = (uint8_t)(pc & 0xFF);
    sp++;
    if (sp >= 0x4000) buffer[sp - 0x4000] = (uint8_t)(pc >> 8);

    fl.WriteBlock(buffer, 0xC000);
  }
  else
  {
    fl.WriteBlock(buffer, 0x8000);

    int bank = mmgr.port7FFD & 7;
    fl.WriteBlock(&mmgr.ram[0x4000 * bank], 0x4000);
    banks[bank] = true;

    fl.PutWORD(pc);
    fl.PutBYTE(mmgr.port7FFD);
    fl.PutBYTE(C_TrDos::trdos ? 1 : 0);

    for (int k = 0; k < 8; k++)
    {
      if (banks[k]) continue;
      fl.WriteBlock(&mmgr.ram[0x4000 * k], 0x4000);
    }
  }

  fl.Close();
}
