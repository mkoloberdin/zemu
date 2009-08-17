#ifndef __YM2203_CHIP_H__
#define __YM2203_CHIP_H__

#include "ym2203_emu.h"

#define YM2203_SND_FQ 44100
#define YM2203_CHIP_CLOCK 3500000

class C_Ym2203Chip
{
	public:

	C_SndRenderer sndRenderer;

	C_Ym2203Chip();
	virtual ~C_Ym2203Chip();

	void Select(int reg);
	void Write(unsigned char val);
	unsigned char Read(void);
	unsigned char ReadStatus(void);
	
	void Reset(void);
	void Render(unsigned devClk);

	protected:

	void* chip;
	int selectedReg;
	unsigned char regs[0x100];
};

#endif
