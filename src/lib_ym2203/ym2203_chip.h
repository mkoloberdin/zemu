#ifndef _YM2203_CHIP_H_INCLUDED_
#define _YM2203_CHIP_H_INCLUDED_

#include "ym2203_emu.h"
#include "../params.h"

#define YM2203_SND_FQ SOUND_FREQ
#define YM2203_CHIP_CLOCK (MAX_FRAME_TACTS * 50)

class C_Ym2203Chip {
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
