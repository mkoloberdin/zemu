#include <zemu_env.h>
#include <exceptions.h>
#include "trdos.h"
#include "../mmanager/mmanager.h"

extern C_MemoryManager dev_mman;

bool C_TrDos::trdos;
uint8_t C_TrDos::rom[0x4000];

void C_TrDos::ReadFile(void)
{
  string filename;
  size_t offset;

  filename = env.getString("beta128", "rom", "trdos.rom");
  filename = split_romname(filename, &offset);

  if (env.loadDataFile("roms", filename.c_str(), rom, 0x4000, offset) != 0x4000) {
    throw C_E(E_General, string("Can't find \"roms/") + filename + "\"");
  }
}

void C_TrDos::Init(void)
{
  ReadFile();

  AttachZ80ReadHandler(ReadByteCheckAddr);
  AttachZ80InputHandler(InputOutputByteCheckPort, OnInputByte);
  AttachZ80OutputHandler(InputOutputByteCheckPort, OnOutputByte);
  AttachResetHandler(OnReset);

  trdos = false;
}

void C_TrDos::Close(void)
{
}

ptrOnReadByteFunc C_TrDos::ReadByteCheckAddr(uint16_t addr, bool m1)
{
  if (m1)
  {
    if (trdos && addr > 0x3FFF)
    {
      return OnReadByte_RAM_M1;
    }

    if (!trdos && (addr & 0xFF00) == 0x3D00) {
      return C_TrDos::OnReadByte_3Dxx_M1;
    }
  }

  if (trdos && addr < 0x4000) {
    return OnReadByte_ROM;
  }

  return nullptr;
}

uint8_t C_TrDos::OnReadByte_3Dxx_M1(uint16_t addr, bool m1)
{
  if (dev_mman.port7FFD & 0x10) { // TRDOS should not be activated if 128 Basic ROM is on
    Enable();
    return rom[addr];
  } else {
    return C_MemoryManager::OnReadByte_ROM(addr, m1);
  }
}

uint8_t C_TrDos::OnReadByte_RAM_M1(uint16_t addr, bool m1)
{
  Disable();

  if (addr < 0x8000) return C_MemoryManager::OnReadByte_Bank5(addr, m1);
  else if (addr < 0xC000) return C_MemoryManager::OnReadByte_Bank2(addr, m1);
  else return C_MemoryManager::OnReadByte_C000(addr, m1);
}

uint8_t C_TrDos::OnReadByte_ROM(uint16_t addr, bool m1)
{
  return rom[addr];
}

bool C_TrDos::InputOutputByteCheckPort(uint16_t port)
{
  int lport = port & 0xFF;
  return (trdos && (lport == 0xFF || lport == 0x7F || lport == 0x1F || lport == 0x3F || lport == 0x5F));
}

bool C_TrDos::OnInputByte(uint16_t port, uint8_t &retval)
{
  int err;
  int lport = port & 0xFF;

  retval = wd1793_in(lport, devClkCounter, &err);
  if (err) printf("WD1793 ERR: %02X\n", err);

  // AddLog("IN:%02X:%02X", lport, retval);

  return true;
}

bool C_TrDos::OnOutputByte(uint16_t port, uint8_t value)
{
  int err;
  int lport = port & 0xFF;

  // AddLog("OUT:%02X:%02X", lport, value);

  wd1793_out(lport, value, devClkCounter, &err);
  if (err) printf("WD1793 ERR: %02X\n", err);

  return true;
}

void C_TrDos::OnReset(void)
{
  Disable();
}

void C_TrDos::Enable(void)
{
  trdos = true;

  devMapRead = devMapRead_trdos;
  devMapInput = devMapInput_trdos;
  devMapOutput = devMapOutput_trdos;
}

void C_TrDos::Disable(void)
{
  trdos = false;

  devMapRead = devMapRead_base;
  devMapInput = devMapInput_base;
  devMapOutput = devMapOutput_base;
}

enum DRIVE_STATE C_TrDos::GetIndicatorState(void)
{
  return wd1793_get_drive_state(wd1793_get_current_drive(), devClkCounter);
}
