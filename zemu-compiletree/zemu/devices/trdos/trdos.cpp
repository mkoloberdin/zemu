#include "trdos.h"
#include "../../file.h"
#include "../../exceptions.h"
#include "../mmanager/mmanager.h"

extern C_MemoryManager dev_mman;

bool C_TrDos::trdos;
BYTE C_TrDos::rom[0x4000];

void C_TrDos::ReadFile(void)
{
	C_File fl;
	char *fname;

	if (!config.GetString("root/Roms/TrDos", &fname)) throw C_E(E_General, "root/Roms/TrDos not found in config");
	fl.Read(fname);
	fl.ReadBlock(rom, 0x4000);
	fl.Close();
}

void C_TrDos::Init(void)
{
	ReadFile();

	AttachZ80ReadHandler(ReadByteCheckAddr, OnReadByte);
	AttachZ80InputHandler(InputOutputByteCheckPort, OnInputByte);
	AttachZ80OutputHandler(InputOutputByteCheckPort, OnOutputByte);
	AttachResetHandler(OnReset);

	trdos = false;
}

void C_TrDos::Close(void)
{
}

bool C_TrDos::ReadByteCheckAddr(Z80EX_WORD addr, bool m1)
{
	return (trdos || (m1 && ((addr & 0xFF00) == 0x3D00)));
}

bool C_TrDos::OnReadByte(Z80EX_WORD addr, bool m1, Z80EX_BYTE &retval)
{
	if (m1)
	{
		if (addr > 0x3FFF)
		{
			Disable();
			return false;
		}

		if ((addr & 0xFF00)==0x3D00 && (dev_mman.port7FFD & 0x10)) {
			Enable();
		}
	}

	if (trdos && addr<0x4000) {
		retval = rom[addr];
	} else {
		C_MemoryManager::OnReadByte(addr, m1, retval);
	}

	return true;
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
