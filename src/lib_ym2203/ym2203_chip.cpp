#include "ym2203_chip.h"

C_Ym2203Chip::C_Ym2203Chip()
{
  chip = YM2203Init(YM2203_CHIP_CLOCK, YM2203_SND_FQ);
  for (int i = 0; i < 0x100; i++) regs[i] = 0;
  Reset();
}

C_Ym2203Chip::~C_Ym2203Chip()
{
  YM2203Shutdown(chip);
}

void C_Ym2203Chip::Select(int reg)
{
  selectedReg = (unsigned char)reg;
}

void C_Ym2203Chip::Write(unsigned char val)
{
  regs[selectedReg] = val;

  YM2203Write(chip, 0, selectedReg);
  YM2203Write(chip, 1, val);
}

unsigned char C_Ym2203Chip::Read(void)
{
  return regs[selectedReg];
}

unsigned char C_Ym2203Chip::ReadStatus(void)
{
  return 0;
}

void C_Ym2203Chip::Reset(void)
{
  YM2203ResetChip(chip);
  YM2203SetMute(chip, 0x0F);
  YM2203Write(chip, 0, 0x27);
  YM2203Write(chip, 1, 0);
}

void C_Ym2203Chip::Render(unsigned devClk)
{
  YM2203UpdateOne(chip, &sndRenderer, devClk);
}
