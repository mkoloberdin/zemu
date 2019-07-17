// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "tap_format.h"
#include "zemu_env.h"

#define TAPE_STATE_STOP 0
#define TAPE_STATE_PLAY 1
#define TAPE_STATE_PILOT_TONE 2
#define TAPE_STATE_PILOT_TONE_LW 3
#define TAPE_STATE_PILOT_TONE_HW 4
#define TAPE_STATE_PILOT_END_LW 5
#define TAPE_STATE_PILOT_END_HW 6
#define TAPE_STATE_NEXT_BYTE 7
#define TAPE_STATE_NEXT_BIT 8
#define TAPE_STATE_BIT_LW 9
#define TAPE_STATE_BIT_HW 10
#define TAPE_STATE_DELAY 11

C_TapFormat::C_TapFormat() {
    state = TAPE_STATE_STOP;
}

C_TapFormat::~C_TapFormat() {
    if (data != nullptr) {
        delete[] data;
    }
}

uint8_t C_TapFormat::Data(long pos) {
    return ((pos >= 0 && pos < size) ? data[pos] : 0);
}

bool C_TapFormat::Load(const char* fname) {
    auto fpath = hostEnv->storage()->path(fname);

    if (!fpath->isFileExists()) {
        return false;
    }

    try {
        size = fpath->fileSize();
    } catch (...) {
        return false;
    }

    if (data != nullptr) {
        delete[] data;
    }

    data = new uint8_t[size];
    hostEnv->storage()->path(fname)->dataReader()->readBlock(data, size);

    blockPos = 0;
    blockSize = 0;
    posInBlock = 0;

    return true;
}

bool C_TapFormat::ProcessTicks(uint64_t ticks) {
    for (;;) {
        switch (state) {
            case TAPE_STATE_STOP: {
                tapeBit = 1;
                return true;
            }

            case TAPE_STATE_PLAY: {
                if (blockPos >= size) {
                    state = TAPE_STATE_STOP;
                    break;
                }

                blockSize = Data(blockPos) + 0x100 * Data(blockPos + 1);
                posInBlock = -1;
                state = TAPE_STATE_PILOT_TONE;
                counter = Data(blockPos + 2) ? 1611 : 4032;

                break;
            }

            case TAPE_STATE_PILOT_TONE: {
                tapeBit = 0;
                state = TAPE_STATE_PILOT_TONE_LW;
                return true;
            }

            case TAPE_STATE_PILOT_TONE_LW: {
                if (ticks < 2168) {
                    return false;
                }

                tapeBit = 1;
                state = TAPE_STATE_PILOT_TONE_HW;
                return true;
            }

            case TAPE_STATE_PILOT_TONE_HW: {
                if (ticks < 2168) {
                    return false;
                }

                tapeBit = 0;
                counter--;
                state = (counter <= 0 ? TAPE_STATE_PILOT_END_LW : TAPE_STATE_PILOT_TONE_LW);
                return true;
            }

            case TAPE_STATE_PILOT_END_LW: {
                if (ticks < 667) {
                    return false;
                }

                tapeBit = 1;
                state = TAPE_STATE_PILOT_END_HW;
                return true;
            }

            case TAPE_STATE_PILOT_END_HW: {
                if (ticks < 735) {
                    return false;
                }

                tapeBit = 0;
                state = TAPE_STATE_NEXT_BYTE;
                return true;
            }

            case TAPE_STATE_NEXT_BYTE: {
                posInBlock++;

                if (posInBlock > blockSize) {
                    blockPos += blockSize + 2;
                    state = TAPE_STATE_DELAY;
                } else {
                    currentByte = Data(blockPos + 2 + posInBlock);
                    counter = 8;
                    state = TAPE_STATE_NEXT_BIT;
                }

                break;
            }

            case TAPE_STATE_NEXT_BIT: {
                counter--;

                if (counter < 0) {
                    state = TAPE_STATE_NEXT_BYTE;
                    break;
                }

                delay = ((currentByte & 128) ? 1710 : 855);
                currentByte = (currentByte << 1) & 0xFF;

                state = TAPE_STATE_BIT_LW;
                break;
            }

            case TAPE_STATE_BIT_LW: {
                if (ticks < (uint64_t)delay) {
                    return false;
                }

                tapeBit = 1;
                state = TAPE_STATE_BIT_HW;
                return true;
            }

            case TAPE_STATE_BIT_HW: {
                if (ticks < (uint64_t)delay) {
                    return false;
                }

                tapeBit = 0;
                state = TAPE_STATE_NEXT_BIT;
                return true;
            }

            case TAPE_STATE_DELAY: {
                if (ticks < 2168 * 1000) {
                    return false;
                }

                state = TAPE_STATE_PLAY;
                return true;
            }

            default: {
                return false;
            }
        }
    }
}

bool C_TapFormat::GetCurrBit(void) {
    return tapeBit;
}

void C_TapFormat::Stop(void) {
    state = TAPE_STATE_STOP;
}

void C_TapFormat::Start(void) {
    if (data != nullptr) {
        state = TAPE_STATE_PLAY;
    }
}

void C_TapFormat::Rewind(void) {
    if (state != TAPE_STATE_STOP) {
        state = TAPE_STATE_PLAY;
    }

    blockPos = 0;
    blockSize = 0;
    posInBlock = 0;
}

unsigned int C_TapFormat::GetPosPerc(void) {
    if (blockPos >= size) {
        return 100;
    }

    return (long)(blockPos + posInBlock) * 100L / size;
}

bool C_TapFormat::IsActive(void) {
    return (state != TAPE_STATE_STOP);
}
