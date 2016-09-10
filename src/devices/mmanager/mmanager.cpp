#include <zemu_env.h>
#include "mmanager.h"
#include "../../exceptions.h"
#include "../extport/extport.h"

#include <stdlib.h>

extern C_ExtPort dev_extport;

Z80EX_BYTE C_MemoryManager::port7FFD;
Z80EX_BYTE C_MemoryManager::rom[0x8000];
Z80EX_BYTE C_MemoryManager::ram[0x4000 * 64];
Z80EX_BYTE *C_MemoryManager::rom_map;
Z80EX_BYTE *C_MemoryManager::ram_map;
bool C_MemoryManager::enable512;
bool C_MemoryManager::enable1024;

std::string split_romname(std::string &romname, size_t *offset) {
  size_t pos;
  *offset = 0;
  if ((pos = romname.rfind(':')) != std::string::npos) {
    std::string str_offset = romname.substr(pos + 1);
    if (!str_offset.empty()) {
      *offset = atoi(str_offset.c_str());
      if (*offset < 32) { // assume that it is a page number, not a literal offset
        *offset *= 0x4000;
      }
    }
    return romname.substr(0, pos);
  }
  return romname;
}

void C_MemoryManager::ReadFile(void)
{
  string filename;
  size_t offset;

  filename = env.GetString("core", "rom_48", "pentagon.rom:1");
  filename = split_romname(filename, &offset);
  if (env.LoadDataFile("roms", filename.c_str(), &rom[0x4000], 0x4000, offset) != 0x4000) {
    throw C_E(E_General, string("Can't find \"roms/") + filename + "\"");
  }

  filename = env.GetString("core", "rom_128", "pentagon.rom:0");
  filename = split_romname(filename, &offset);
  if (env.LoadDataFile("roms", filename.c_str(), rom, 0x4000, offset) != 0x4000) {
    throw C_E(E_General, string("Can't find \"roms/") + filename + "\"");
  }

  enable512 = env.GetBool("core", "enable512", false);
  enable1024 = env.GetBool("core", "enable1024",  false);

  if (!enable512) enable1024 = false;
}

void C_MemoryManager::Init(void)
{
  ReadFile();

  AttachZ80ReadHandler(ReadByteCheckAddr);
  AttachZ80WriteHandler(WriteByteCheckAddr, OnWriteByte);
  AttachZ80OutputHandler(OutputByteCheckPort, OnOutputByte);
  AttachResetHandler(OnReset);

  port7FFD = 0;
  Remap();
}

void C_MemoryManager::Close(void)
{
}

void C_MemoryManager::Remap(void)
{
  if (enable1024) ram_map = &ram[(((port7FFD & 0xE0) >> 2) | (port7FFD & 7)) * 0x4000];
  else if (enable512) ram_map = &ram[(((port7FFD & 0xC0) >> 3) | (port7FFD & 7)) * 0x4000];
  else ram_map = &ram[(port7FFD & 7) * 0x4000];

  rom_map = &rom[(port7FFD & 16) ? 0x4000 : 0];
}

ptrOnReadByteFunc C_MemoryManager::ReadByteCheckAddr(Z80EX_WORD addr, bool m1)
{
  if (addr < 0x4000) return OnReadByte_ROM;
  else if (addr < 0x8000) return OnReadByte_Bank5;
  else if (addr < 0xC000) return OnReadByte_Bank2;
  else return OnReadByte_C000;
}

Z80EX_BYTE C_MemoryManager::OnReadByte_ROM(Z80EX_WORD addr, bool m1)
{
  return (dev_extport.IsRamMapRom() ? ram[addr] : rom_map[addr]);
}

Z80EX_BYTE C_MemoryManager::OnReadByte_Bank5(Z80EX_WORD addr, bool m1)
{
  return ram[addr - 0x4000 + RAM_BANK5];
}

Z80EX_BYTE C_MemoryManager::OnReadByte_Bank2(Z80EX_WORD addr, bool m1)
{
  return ram[addr - 0x8000 + RAM_BANK2];
}

Z80EX_BYTE C_MemoryManager::OnReadByte_C000(Z80EX_WORD addr, bool m1)
{
  return ram_map[addr - 0xC000];
}

bool C_MemoryManager::WriteByteCheckAddr(Z80EX_WORD addr)
{
  return true;
}

bool C_MemoryManager::OnWriteByte(Z80EX_WORD addr, Z80EX_BYTE value)
{
  if (addr < 0x4000)
  {
    if (dev_extport.IsRamMapRom()) ram[addr] = value;
    return true;
  }

  if (addr < 0x8000) ram[addr - 0x4000 + RAM_BANK5] = value;
  else if (addr < 0xC000) ram[addr - 0x8000 + RAM_BANK2] = value;
  else ram_map[addr - 0xC000] = value;

  return true;
}

bool C_MemoryManager::OutputByteCheckPort(Z80EX_WORD port)
{
  return ((port & 0x8003) == 0x0001);		// 0x7FFD
}

bool C_MemoryManager::OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value)
{
  bool himemEnabled = !dev_extport.Is128Lock();

  if (!(port7FFD & 32) || (enable1024 && himemEnabled))
  {
    if (enable512 && port == 0x7FFD && himemEnabled) port7FFD = value;
    else port7FFD = value & 63;

    Remap();
  }

  return true;
}

void C_MemoryManager::OnReset(void)
{
  port7FFD = 0;
  Remap();
}
