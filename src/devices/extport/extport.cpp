#include "extport.h"

#define EXTPORT_16COLORS_MASK		1		// 16 colors (4bits per pixel)
#define EXTPORT_512x192_MASK		2		// 512x192 monochrome
#define EXTPORT_128LOCK_MASK		4		// disable memory above 128k
#define EXTPORT_RAM_MAP_ROM			8		// first ram page (0 bank) maps to rom
#define EXTPORT_TURBO_MASK			16		// turbo (0 - on, 1 - off) pentagon sl 2.2
#define EXTPORT_MULTICOLOR_MASK		32		// hardware multicolor
#define EXTPORT_384x304_MASK		64		// 384x304 mode
#define EXTPORT_CMOS_MASK			128		// cmos

#define EXTPORT_OLD_MULTICOLOR_MASK	1		// hardware multicolor
#define EXTPORT_OLD_CACHE_RO_MASK	8		// read only cache
#define EXTPORT_OLD_GIGASCREEN_MASK	16		// gigascreen
#define EXTPORT_OLD_16COLORS_MASK	32		// 16 colors (4bits per pixel)

Z80EX_BYTE C_ExtPort::portEFF7;
bool C_ExtPort::oldEFF7Mode;
bool C_ExtPort::useEFF7Turbo;
bool C_ExtPort::enabled;

void C_ExtPort::Init(void)
{
	portEFF7 = 0;
	if (!config.GetBool("root/EnableEFF7", &enabled)) enabled = false;

	if (enabled)
	{
		AttachZ80OutputHandler(OutputByteCheckPort, OnOutputByte);
		AttachResetHandler(OnReset);

		if (!config.GetBool("root/OldEFF7Mode", &oldEFF7Mode)) oldEFF7Mode = false;
		if (!config.GetBool("root/UseEFF7Turbo", &useEFF7Turbo)) useEFF7Turbo = false;

		if (useEFF7Turbo) turboMultiplierNx = 2;
	}
}

void C_ExtPort::Close(void)
{
}

bool C_ExtPort::OutputByteCheckPort(Z80EX_WORD port)
{
	return (port == 0xEFF7);
}

bool C_ExtPort::OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value)
{
	if (useEFF7Turbo && (value & EXTPORT_TURBO_MASK)) {
		turboMultiplierNx = ((value & EXTPORT_TURBO_MASK) ? 1 : 2);
	}

	portEFF7 = value;
	return true;
}

void C_ExtPort::OnReset(void)
{
	portEFF7 = 0;
}

bool C_ExtPort::Is16Colors(void)
{
	return (portEFF7 & (oldEFF7Mode ? EXTPORT_OLD_16COLORS_MASK : EXTPORT_16COLORS_MASK));
}

bool C_ExtPort::Is512x192(void)
{
	return (portEFF7 & EXTPORT_512x192_MASK);
}

bool C_ExtPort::Is128Lock(void)
{
	return (portEFF7 & EXTPORT_128LOCK_MASK);
}

bool C_ExtPort::IsRamMapRom(void)
{
	return (portEFF7 & EXTPORT_RAM_MAP_ROM);
}

bool C_ExtPort::IsGigascreen(void)
{
	return (useEFF7Turbo ? 0 : (portEFF7 & EXTPORT_OLD_GIGASCREEN_MASK));
}

bool C_ExtPort::IsMulticolor(void)
{
	return (portEFF7 & (oldEFF7Mode ? EXTPORT_OLD_MULTICOLOR_MASK : EXTPORT_MULTICOLOR_MASK));
}

bool C_ExtPort::Is384x304(void)
{
	return (portEFF7 & EXTPORT_384x304_MASK);
}

bool C_ExtPort::IsCmos(void)
{
	return (portEFF7 & EXTPORT_CMOS_MASK);
}
