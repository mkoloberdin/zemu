#include "mmanager.h"
#include "../../file.h"
#include "../../exceptions.h"
#include "../extport/extport.h"

extern C_ExtPort dev_extport;
 
Z80EX_BYTE C_MemoryManager::port7FFD;
BYTE C_MemoryManager::rom[0x8000];
BYTE C_MemoryManager::ram[0x4000*64];
BYTE* C_MemoryManager::rom_map;
BYTE* C_MemoryManager::ram_map;
bool C_MemoryManager::enable512;
bool C_MemoryManager::enable1024;

void C_MemoryManager::ReadFile(void)
{
	C_File fl;
	char *fname;

	if (config.GetString("root/Roms/32k", &fname))
	{
		fl.Read(fname);
		fl.ReadBlock(rom, 0x8000);
		fl.Close();
	}
	else
	if (config.GetString("root/Roms/48", &fname))
	{
		fl.Read(fname);
		fl.ReadBlock(&rom[0x4000], 0x4000);
		fl.Close();

		if (config.GetString("root/Roms/128", &fname))
		{
			fl.Read(fname);
			fl.ReadBlock(rom, 0x4000);
			fl.Close();
		}
		else throw C_E(E_General, "(root/Roms/32k or (root/Roms/48 and root/Roms/128)) not found in config");
	}
	else throw C_E(E_General, "(root/Roms/32k or (root/Roms/48 and root/Roms/128)) not found in config");

	if (!config.GetBool("root/Enable512", &enable512)) enable512 = false;
	if (!config.GetBool("root/Enable1024", &enable1024)) enable1024 = false;

	if (!enable512) enable1024 = false;
}

void C_MemoryManager::Init(void)
{
	ReadFile();

	AttachZ80ReadHandler(ReadByteCheckAddr, OnReadByte);
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
	else
	if (enable512) ram_map = &ram[(((port7FFD & 0xC0) >> 3) | (port7FFD & 7)) * 0x4000];
	else ram_map = &ram[(port7FFD & 7) * 0x4000];

	rom_map = &rom[(port7FFD & 16) ? 0x4000 : 0];
}

bool C_MemoryManager::ReadByteCheckAddr(Z80EX_WORD addr, bool m1)
{
	return true;
}

bool C_MemoryManager::OnReadByte(Z80EX_WORD addr, bool m1, Z80EX_BYTE &retval)
{
	if (addr < 0x4000) retval = (dev_extport.IsRamMapRom() ? ram[addr] : rom_map[addr]);
	else
	if (addr < 0x8000) retval = ram[addr-0x4000 + RAM_BANK5];
	else
	if (addr < 0xC000) retval = ram[addr-0x8000 + RAM_BANK2];
	else retval = ram_map[addr-0xC000];

	return true;
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

	if (addr < 0x8000) ram[addr-0x4000 + RAM_BANK5] = value;
	else
	if (addr < 0xC000) ram[addr-0x8000 + RAM_BANK2] = value;
	else ram_map[addr-0xC000] = value;

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
		if (enable512 && port==0x7FFD && himemEnabled) port7FFD = value;
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
