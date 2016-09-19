#include <stdio.h>
#include <cstdint>
#include "snap_sna.h"
#include "exceptions.h"
#include "zemu.h"

bool loadSnaSnap(const fs::path& filename, Z80EX_CONTEXT *cpu, C_MemoryManager& mmgr,
                 C_Border& border)
{
  fs::ifstream IFS;
  uint8_t border_color;
  bool banks[8];
  uint8_t buffer[16384];
  uint8_t retval;
  uint8_t port_7ffd;
  uint8_t is_trdos;

  try {
    IFS.open(filename, std::ios_base::binary);
  } catch (C_E &e) {
    return false;
  }

  z80ex_set_reg(cpu, regI, readU8(IFS));
  z80ex_set_reg(cpu, regHL_, readU16(IFS));
  z80ex_set_reg(cpu, regDE_, readU16(IFS));
  z80ex_set_reg(cpu, regBC_, readU16(IFS));
  z80ex_set_reg(cpu, regAF_, readU16(IFS));
  z80ex_set_reg(cpu, regHL, readU16(IFS));
  z80ex_set_reg(cpu, regDE, readU16(IFS));
  z80ex_set_reg(cpu, regBC, readU16(IFS));
  z80ex_set_reg(cpu, regIY, readU16(IFS));
  z80ex_set_reg(cpu, regIX, readU16(IFS));

  uint8_t intm = readU8(IFS);

  z80ex_set_reg(cpu, regIFF1, intm & 1);
  z80ex_set_reg(cpu, regIFF2, (intm >> 1) & 1);

  z80ex_set_reg(cpu, regR, readU8(IFS));
  z80ex_set_reg(cpu, regAF, readU16(IFS));
  z80ex_set_reg(cpu, regSP, readU16(IFS));
  z80ex_set_reg(cpu, regIM, readU8(IFS));

  border_color = readU8(IFS);

  mmgr.OnReset();
  for (int i = 0; i < 8; i++) banks[i] = false;

  for (int i = 0; i < 16384; i++) mmgr.OnWriteByte(i + 16384, readU8(IFS));
  banks[5] = true;

  for (int i = 0; i < 16384; i++) mmgr.OnWriteByte(i + 32768, readU8(IFS));
  banks[2] = true;

  for (int i = 0; i < 16384; i++) buffer[i] = readU8(IFS);

  // If this read op triggers EOF then it is a 48K snapshot
  uint16_t regPC_128K = readU16(IFS);
  
  if (IFS.eof()) // 48K snapshot
  {
    mmgr.OnOutputByte(0x7ffd, 0x30);  // set 48k mode
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
  else { // 128K snapshot
    z80ex_set_reg(cpu, regPC, regPC_128K);

    port_7ffd = readU8(IFS);
    is_trdos = readU8(IFS);

    mmgr.OnOutputByte(0x7ffd, port_7ffd & 7);
    for (int i = 0; i < 16384; i++) mmgr.OnWriteByte(i + 49152, buffer[i]);
    banks[port_7ffd & 7] = true;

    for (int k = 0; k < 8; k++)
    {
      if (banks[k]) continue;

      mmgr.OnOutputByte(0x7ffd, k);
      for (int i = 0; i < 16384; i++) mmgr.OnWriteByte(i + 49152, readU8(IFS));
    }

    C_TrDos::trdos = (is_trdos != 0);
    mmgr.OnOutputByte(0x7ffd, port_7ffd);
  }

  // set border color
  border.OnOutputByte(0x00FE, border_color);
  IFS.close();
  return true;
}

void saveSnaSnap(const fs::path& filename, Z80EX_CONTEXT *cpu, C_MemoryManager& mmgr,
                 C_Border& border)
{
  fs::ofstream OFS;
  bool banks[8];

  OFS.open(filename, std::ios_base::binary);

  writeU8(OFS, z80ex_get_reg(cpu, regI));
  writeU16(OFS, z80ex_get_reg(cpu, regHL_));
  writeU16(OFS, z80ex_get_reg(cpu, regDE_));
  writeU16(OFS, z80ex_get_reg(cpu, regBC_));
  writeU16(OFS, z80ex_get_reg(cpu, regAF_));
  writeU16(OFS, z80ex_get_reg(cpu, regHL));
  writeU16(OFS, z80ex_get_reg(cpu, regDE));
  writeU16(OFS, z80ex_get_reg(cpu, regBC));
  writeU16(OFS, z80ex_get_reg(cpu, regIY));
  writeU16(OFS, z80ex_get_reg(cpu, regIX));

  writeU8(OFS, (z80ex_get_reg(cpu, regIFF1) ? 1 : 0) | (z80ex_get_reg(cpu, regIFF2) ? 2 : 0));
  uint16_t sp = z80ex_get_reg(cpu, regSP);

  if (mmgr.port7FFD == 0x30)
  {
    // 48k
    sp -= 2;
  }

  writeU8(OFS, z80ex_get_reg(cpu, regR));
  writeU16(OFS, z80ex_get_reg(cpu, regAF));
  writeU16(OFS, sp);
  writeU16(OFS, z80ex_get_reg(cpu, regIM));

  writeU8(OFS, border.portFB & 7);

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

    OFS.write((const char *)buffer, 0xC000);
  }
  else
  {
    OFS.write((const char *)buffer, 0x8000);

    int bank = mmgr.port7FFD & 7;
    OFS.write((const char *)&mmgr.ram[0x4000 * bank], 0x4000);
    banks[bank] = true;

    writeU16(OFS, pc);
    writeU8(OFS, mmgr.port7FFD);
    writeU8(OFS, C_TrDos::trdos ? 1 : 0);

    for (int k = 0; k < 8; k++)
    {
      if (banks[k]) continue;
      OFS.write((const char *)&mmgr.ram[0x4000 * k], 0x4000);
    }
  }

  OFS.close();
}
