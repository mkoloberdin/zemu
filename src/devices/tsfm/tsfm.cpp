// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "tsfm.h"
#include "../trdos/trdos.h"

#define CHIP_FLAG_MASK 1
#define STATUS_FLAG_MASK 2
#define FM_FLAG_MASK 4
#define SAA_FLAG_MASK 8

#define CHIP_NUM ((pseudoReg & CHIP_FLAG_MASK) ? 0 : 1)

C_Saa1099Chip C_TsFm::saa1099Chip;
C_Ym2203Chip C_TsFm::ym2203Chip[TSFM_CHIPS_COUNT];
C_AyChip C_TsFm::ayChip[TSFM_CHIPS_COUNT];
int C_TsFm::mode;
int C_TsFm::pseudoReg;
int C_TsFm::selectedReg;

void C_TsFm::Init(void) {
    const char* str;

    mode = TSFM_MODE_ZXM;
    pseudoReg = 15;
    selectedReg = 0;

    AttachZ80InputHandler(InputByteCheckPort, OnInputByte);
    AttachZ80OutputHandler(OutputByteCheckPort, OnOutputByte);
    AttachFrameStartHandler(OnFrameStart);
    AttachAfterFrameRenderHandler(OnAfterFrameRender);
    AttachResetHandler(OnReset);

    auto s = host->config()->getString("sound", "tsfmmode", "zxm");
    str = s.c_str();

    if (!strcasecmp(str, "ay")) {
        mode = TSFM_MODE_AY;
    } else if (!strcasecmp(str, "ts")) {
        mode = TSFM_MODE_TS;
    } else if (!strcasecmp(str, "tsfm")) {
        mode = TSFM_MODE_TSFM;
    }

    ayChip[0].Init();
    soundMixer.AddSource(&ayChip[0].sndRenderer);

    if (mode >= TSFM_MODE_TS) {
        ayChip[1].Init();
        soundMixer.AddSource(&ayChip[1].sndRenderer);

        if (mode >= TSFM_MODE_TSFM) {
            soundMixer.AddSource(&ym2203Chip[0].sndRenderer);
            soundMixer.AddSource(&ym2203Chip[1].sndRenderer);

            if (mode >= TSFM_MODE_ZXM) {
                soundMixer.AddSource(&saa1099Chip.sndRenderer);
            }
        }
    }
}

void C_TsFm::Close(void) {
}

bool C_TsFm::InputByteCheckPort(uint16_t port) {
    return ((port & 0b11000000'00000010) == 0b11000000'00000000); // 0xFFFD
}

bool C_TsFm::OnInputByte(uint16_t port, uint8_t& retval) {
    if (mode >= TSFM_MODE_TSFM) {
        if (!(pseudoReg & STATUS_FLAG_MASK) && !(pseudoReg & FM_FLAG_MASK)) {
            retval = ym2203Chip[CHIP_NUM].ReadStatus();
        } else {
            retval = (selectedReg < 0x10) ? ayChip[CHIP_NUM].Read() : ym2203Chip[CHIP_NUM].Read();
        }
    } else if (mode >= TSFM_MODE_TS) {
        retval = ayChip[CHIP_NUM].Read();
    } else {
        retval = ayChip[0].Read();
    }

    return true;
}

bool C_TsFm::OutputByteCheckPort(uint16_t port) {
    return (
        ((port & 0b11000000'00000010) == 0b11000000'00000000) // 0xFFFD
        || ((port & 0b11000000'00000010) == 0b10000000'00000000) // 0xBFFD
        || (!C_TrDos::trdos && ((port == 0x00FF) || (port == 0x01FF)))
    );
}

bool C_TsFm::OnOutputByte(uint16_t port, uint8_t value) {
    if (port == 0x00FF) {
        if (mode >= TSFM_MODE_ZXM) {
            saa1099Chip.WriteData(value);
        }
    } else if (port == 0x01FF) {
        if (mode >= TSFM_MODE_ZXM) {
            saa1099Chip.WriteAddress(value);
        }
    } else if ((port & 0b11000000'00000010) == 0b11000000'00000000) { // 0xFFFD
        if ((mode >= TSFM_MODE_TS) && ((value & 0b11110000) == 0b11110000)) {
            pseudoReg = value & 15;
            return true;
        }

        if (mode >= TSFM_MODE_TSFM) {
            if (value < 0x10) {
                ayChip[CHIP_NUM].Select(value);
            } else {
                ym2203Chip[CHIP_NUM].Select(value);
            }
        } else if (mode >= TSFM_MODE_TS) {
            ayChip[CHIP_NUM].Select(value);
        } else {
            ayChip[0].Select(value);
        }

        selectedReg = value;
    } else { // 0xBFFD
        if (mode >= TSFM_MODE_TSFM) {
            if (selectedReg < 0x10) {
                ayChip[CHIP_NUM].Write(SOUND_ENABLED ? devClk : 0, value);
            } else {
                ym2203Chip[CHIP_NUM].Write(value);
            }
        } else if (mode >= TSFM_MODE_TS) {
            ayChip[CHIP_NUM].Write(SOUND_ENABLED ? devClk : 0, value);
        } else {
            ayChip[0].Write(SOUND_ENABLED ? devClk : 0, value);
        }
    }

    return true;
}

void C_TsFm::OnFrameStart(void) {
    if (!SOUND_ENABLED) {
        return;
    }

    ayChip[0].StartFrame();

    if (mode >= TSFM_MODE_TS) {
        ayChip[1].StartFrame();

        if (mode >= TSFM_MODE_TSFM) {
            ym2203Chip[0].sndRenderer.StartFrame();
            ym2203Chip[1].sndRenderer.StartFrame();

            if (mode >= TSFM_MODE_ZXM) {
                saa1099Chip.sndRenderer.StartFrame();
            }
        }
    }
}

void C_TsFm::OnAfterFrameRender(void) {
    if (!SOUND_ENABLED) {
        return;
    }

    if ((mode >= TSFM_MODE_TSFM) && !(pseudoReg & FM_FLAG_MASK)) {
        ym2203Chip[0].Render(lastDevClk);
        ym2203Chip[1].Render(lastDevClk);
    }

    if ((mode >= TSFM_MODE_ZXM) && !(pseudoReg & SAA_FLAG_MASK)) {
        saa1099Chip.Render(lastDevClk);
    }

    ayChip[0].EndFrame(lastDevClk);

    if (mode >= TSFM_MODE_TS) {
        ayChip[1].EndFrame(lastDevClk);

        if (mode >= TSFM_MODE_TSFM) {
            ym2203Chip[0].sndRenderer.EndFrame(lastDevClk);
            ym2203Chip[1].sndRenderer.EndFrame(lastDevClk);

            if (mode >= TSFM_MODE_ZXM) {
                saa1099Chip.sndRenderer.EndFrame(lastDevClk);
            }
        }
    }
}

void C_TsFm::OnReset(void) {
    ayChip[0].Reset();

    if (mode >= TSFM_MODE_TS) {
        ayChip[1].Reset();

        if (mode >= TSFM_MODE_TSFM) {
            ym2203Chip[0].Reset();
            ym2203Chip[1].Reset();

            if (mode >= TSFM_MODE_ZXM) {
                saa1099Chip.Reset();
            }
        }
    }

    pseudoReg = 15;
    selectedReg = 0;
}
