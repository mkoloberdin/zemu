#include "tsfm.h"
#include "../../bin.h"

#define CHIP_FLAG_MASK 1
#define STATUS_FLAG_MASK 2
#define FM_FLAG_MASK 4

#define CHIP_NUM ((pseudoReg & CHIP_FLAG_MASK) ? 0 : 1)

C_Ym2203Chip C_TsFm::ym2203Chip[TSFM_CHIPS_COUNT];
C_AyChip C_TsFm::ayChip[TSFM_CHIPS_COUNT];
int C_TsFm::mode;
int C_TsFm::pseudoReg;
int C_TsFm::selectedReg;

void C_TsFm::Init(void)
{
	const char *str;

	mode = TSFM_MODE_TSFM;
	pseudoReg = 7;
	selectedReg = 0;

	AttachZ80InputHandler(InputByteCheckPort, OnInputByte);
	AttachZ80OutputHandler(OutputByteCheckPort, OnOutputByte);
	AttachFrameStartHandler(OnFrameStart);
	AttachAfterFrameRenderHandler(OnAfterFrameRender);
	AttachResetHandler(OnReset);

	string s = config.GetString("sound", "tsfmmode", "tsfm");
	str = s.c_str();

	if (!strcasecmp(str, "ay")) mode = TSFM_MODE_AY;
	else
	if (!strcasecmp(str, "ts")) mode = TSFM_MODE_TS;

	ayChip[0].Init();
	SoundMixer.AddSource(&ayChip[0].sndRenderer);

	if (mode != TSFM_MODE_AY)
	{
		ayChip[1].Init();
		SoundMixer.AddSource(&ayChip[1].sndRenderer);

		if (mode == TSFM_MODE_TSFM)
		{
			SoundMixer.AddSource(&ym2203Chip[0].sndRenderer);
			SoundMixer.AddSource(&ym2203Chip[1].sndRenderer);
		}
	}
}

void C_TsFm::Close(void)
{
}

bool C_TsFm::InputByteCheckPort(Z80EX_WORD port)
{
	return ((port & MAKEWORD(BIN(0x11000000),BIN(0x00000010))) == MAKEWORD(BIN(0x11000000),BIN(0x00000000)));	// 0xFFFD
}

bool C_TsFm::OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval)
{
	if (mode == TSFM_MODE_AY)
	{
		retval = ayChip[0].Read();
	}
	else if (mode == TSFM_MODE_TS)
	{
		retval = ayChip[CHIP_NUM].Read();
	}
	else
	{
		if ( !(pseudoReg & STATUS_FLAG_MASK) && !(pseudoReg & FM_FLAG_MASK) ) {
			retval = ym2203Chip[CHIP_NUM].ReadStatus();
		} else {
			retval = (selectedReg < 0x10) ? ayChip[CHIP_NUM].Read() : ym2203Chip[CHIP_NUM].Read();
		}
	}

	return true;
}

bool C_TsFm::OutputByteCheckPort(Z80EX_WORD port)
{
	return (
		((port & MAKEWORD(BIN(0x11000000),BIN(0x00000010))) == MAKEWORD(BIN(0x11000000),BIN(0x00000000)))	// 0xFFFD
		||
		((port & MAKEWORD(BIN(0x11000000),BIN(0x00000010))) == MAKEWORD(BIN(0x10000000),BIN(0x00000000)))	// 0xBFFD
	);
}

bool C_TsFm::OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value)
{
	if ((port & MAKEWORD(BIN(0x11000000),BIN(0x00000010))) == MAKEWORD(BIN(0x11000000),BIN(0x00000000)))	// 0xFFFD
	{
		if ((mode != TSFM_MODE_AY) && ((value & BIN(0x11111000)) == BIN(0x11111000)))
		{
			pseudoReg = value & 7;
			return true;
		}

		if (mode == TSFM_MODE_AY)
		{
			ayChip[0].Select(value);
		}
		else if (mode == TSFM_MODE_TS)
		{
			ayChip[CHIP_NUM].Select(value);
		}
		else
		{
			if (value < 0x10) {
				ayChip[CHIP_NUM].Select(value);
			} else {
				ym2203Chip[CHIP_NUM].Select(value);
			}
		}

		selectedReg = value;
	}
	else	// 0xBFFD
	{
		if (mode == TSFM_MODE_AY)
		{
			ayChip[0].Write(SOUND_ENABLED ? devClk : 0, value);
		}
		else if (mode == TSFM_MODE_TS)
		{
			ayChip[CHIP_NUM].Write(SOUND_ENABLED ? devClk : 0, value);
		}
		else
		{
			if (selectedReg < 0x10) {
				ayChip[CHIP_NUM].Write(SOUND_ENABLED ? devClk : 0, value);
			} else {
				ym2203Chip[CHIP_NUM].Write(value);
			}
		}
	}

	return true;
}

void C_TsFm::OnFrameStart(void)
{
	if (SOUND_ENABLED)
	{
		ayChip[0].StartFrame();

		if (mode != TSFM_MODE_AY)
		{
			ayChip[1].StartFrame();

			if (mode == TSFM_MODE_TSFM)
			{
				ym2203Chip[0].sndRenderer.StartFrame();
				ym2203Chip[1].sndRenderer.StartFrame();
			}
		}
	}
}

void C_TsFm::OnAfterFrameRender(void)
{
	if (SOUND_ENABLED)
	{
		if ((mode == TSFM_MODE_TSFM) && !(pseudoReg & FM_FLAG_MASK))
		{
			ym2203Chip[0].Render(lastDevClk);
			ym2203Chip[1].Render(lastDevClk);
		}

		ayChip[0].EndFrame(lastDevClk);

		if (mode != TSFM_MODE_AY)
		{
			ayChip[1].EndFrame(lastDevClk);

			if (mode == TSFM_MODE_TSFM)
			{
				ym2203Chip[0].sndRenderer.EndFrame(lastDevClk);
				ym2203Chip[1].sndRenderer.EndFrame(lastDevClk);
			}
		}
	}
}

void C_TsFm::OnReset(void)
{
	ayChip[0].Reset();

	if (mode != TSFM_MODE_AY)
	{
		ayChip[1].Reset();

		if (mode == TSFM_MODE_TSFM)
		{
			ym2203Chip[0].Reset();
			ym2203Chip[1].Reset();
		}
	}

	pseudoReg = 7;
	selectedReg = 0;
}
