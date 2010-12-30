#include "saa1099_chip.h"

C_Saa1099Chip::C_Saa1099Chip()
{
	chip = CreateCSAASound();
	chip->SetSoundParameters(SAAP_NOFILTER | SAAP_44100 | SAAP_16BIT | SAAP_STEREO);
	Reset();
}

C_Saa1099Chip::~C_Saa1099Chip()
{
	DestroyCSAASound(chip);
}

void C_Saa1099Chip::WriteAddress(unsigned char reg)
{
	chip->WriteAddress(reg);
}

void C_Saa1099Chip::WriteData(unsigned char data)
{
	chip->WriteData(data);
}

void C_Saa1099Chip::Reset(void)
{
	chip->Clear();
}

void C_Saa1099Chip::Render(unsigned devClk)
{
	chip->GenerateManyUsingSndRenderer(&sndRenderer, devClk);
}
