#ifndef __AY_CHIP__
#define __AY_CHIP__

#include "../zemu.h"
#include "../sound/snd_renderer.h"

/*
 * (c) zame software development
 * AY-8910 and YM-2149 chip emulation
 *
 * YM-2149F emulator for Unreal Speccy project
 * created under public domain license by SMT, jan.2006
 */

const unsigned SNDR_DEFAULT_AY_RATE = 1774400; // original ZX-Spectrum soundchip clock fq

#define AYREG_fA		(0)		// word
#define AYREG_fB		(2)		// word
#define AYREG_fC		(4)		// word
#define AYREG_noise		(6)		// byte
#define AYREG_mix		(7)		// byte
#define AYREG_vA		(8)		// byte
#define AYREG_vB		(9)		// byte
#define AYREG_vC		(10)	// byte
#define AYREG_envT		(11)	// word
#define AYREG_env		(13)	// byte
#define AYREG_portA		(14)	// byte
#define AYREG_portB		(15)	// byte

class C_AyChipConfig
{
	public:

	enum ChipType
	{
		TypeAy = 0,
		TypeYm = 1
	};

	enum VolType
	{
		VolAy = 0,
		VolYm = 1
	};

	enum PanType
	{
		PanMono = 0,
		PanABC = 1,
		PanACB = 2,
		PanBAC = 3,
		PanBCA = 4,
		PanCAB = 5,
		PanCBA = 6
	};

	C_AyChipConfig();

	void ReadConfig(void);

	const unsigned * GetVolTab(void);
	const unsigned * GetPanTab(void);

	ChipType chipType;
	VolType volType;
	PanType panType;
};

class C_AyChip
{
	public:

	C_SndRenderer sndRenderer;

	C_AyChip();
	virtual ~C_AyChip();

	void Init(void);

	void StartFrame(void);
	void EndFrame(unsigned devClk);

	void Select(unsigned char reg);
	void Write(unsigned devClk, unsigned char val);
	unsigned char Read(void);

	void Reset(unsigned devClk=0);	// call with default parameter, when context outside StartFrame/EndFrame block

	protected:

	void Flush(unsigned tick);
	void ApplyRegs(unsigned devClk=0);
	void SetVolumes(unsigned globalVol, const unsigned * volTab, const unsigned * panTab);
	void SetTimings(unsigned systemClockRate, unsigned chipClockRate, unsigned sampleRate);

	C_AyChipConfig conf;

	unsigned t, ta, tb, tc, tn, te, env, denv;
	unsigned bitA, bitB, bitC, bitN, ns;
	unsigned bit0, bit1, bit2, bit3, bit4, bit5;
	unsigned ea, eb, ec, va, vb, vc;
	unsigned fa, fb, fc, fn, fe;
	unsigned multConst;

	unsigned vols[6][32];

	unsigned char r13Reloaded;
	unsigned char selectedReg;
	unsigned char regs[0x10];

	unsigned chipClockRate, systemClockRate;
	uint64_t passedChipTicks, passedClkTicks;
};

#endif
