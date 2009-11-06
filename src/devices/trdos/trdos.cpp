#include "trdos.h"
#include "../../exceptions.h"
#include "../mmanager/mmanager.h"

extern C_MemoryManager dev_mman;

bool C_TrDos::trdos;
BYTE C_TrDos::rom[0x4000];

void C_TrDos::ReadFile(void)
{
	string filename;
	size_t offset;

	filename = config.GetString("beta128", "rom", "trdos.rom");
	filename = split_romname(filename, &offset);
	if (config.LoadDataFile("roms", filename.c_str(), rom, 0x4000, offset) != 0x4000) {
		throw C_E(E_General, "Error loading " + filename);
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

ptrOnReadByteFunc C_TrDos::ReadByteCheckAddr(Z80EX_WORD addr, bool m1)
{
	if (m1)
	{
		if (trdos && addr > 0x3FFF)
		{
            return OnReadByte_RAM_M1;
		}

		if (!trdos && (addr & 0xFF00)==0x3D00) {
			return C_TrDos::OnReadByte_3Dxx_M1;
		}
	}

	if (trdos && addr<0x4000) {
        return OnReadByte_ROM;
	}
    return NULL;
}

Z80EX_BYTE C_TrDos::OnReadByte_3Dxx_M1(Z80EX_WORD addr, bool m1)
{
    if (dev_mman.port7FFD & 0x10) { // TRDOS should not be activated if 128 Basic ROM is on
        Enable();
		return rom[addr];
    }
    else {
        return C_MemoryManager::OnReadByte_ROM(addr, m1);
    }
}

Z80EX_BYTE C_TrDos::OnReadByte_RAM_M1(Z80EX_WORD addr, bool m1)
{
    Disable();
	if (addr < 0x8000) return C_MemoryManager::OnReadByte_Bank5(addr, m1);
	else
	if (addr < 0xC000) return C_MemoryManager::OnReadByte_Bank2(addr, m1);
	else return C_MemoryManager::OnReadByte_C000(addr, m1);
}

Z80EX_BYTE C_TrDos::OnReadByte_ROM(Z80EX_WORD addr, bool m1)
{
    return rom[addr];
}

bool C_TrDos::InputOutputByteCheckPort(Z80EX_WORD port)
{
	int lport = port & 0xFF;
	return (trdos && (lport==0xFF || lport==0x7F || lport==0x1F || lport==0x3F || lport==0x5F));
}

bool C_TrDos::OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval)
{
	int err;
	int lport = port & 0xFF;

	retval = wd1793_in(lport, devClkCounter, &err);
	if (err) printf("WD1793 ERR: %02X\n", err);

	// AddLog("IN:%02X:%02X", lport, retval);

	return true;
}

bool C_TrDos::OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value)
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
