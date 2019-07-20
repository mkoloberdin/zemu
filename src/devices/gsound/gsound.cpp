// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <string>
#include <stdexcept>
#include "gsound.h"
#include "../mmanager/mmanager.h"

C_SndRenderer C_GSound::sndRenderer;
bool C_GSound::enabled = false;
uint8_t C_GSound::regCommand = 0;
uint8_t C_GSound::regStatus = 0x7E;
uint8_t C_GSound::regData = 0;
uint8_t C_GSound::regOutput = 0;
uint8_t C_GSound::volume[4] = { 0, 0, 0, 0 };
uint8_t C_GSound::channel[4] = { 0, 0, 0, 0 };
uint8_t C_GSound::memPage = 0;
Z80EX_CONTEXT* C_GSound::gsCpu = nullptr;
uint8_t C_GSound::mem[0x8000 * GS_MEM_PAGES];

unsigned C_GSound::gsClk = 0;
uint8_t* C_GSound::readMap[4];
uint8_t* C_GSound::writeMap[4];

#define GS_DEV_TO_CLK(clk) ((clk) << 2)
#define GS_CLK_TO_DEV(clk) ((clk) >> 2)
#define GS_INT_PERIOD (384)

void C_GSound::Init(void) {
    auto config = host->config();
    enabled = config->getBool("sound", "enablegs", false);

    if (!enabled) {
        return;
    }

    AttachZ80InputHandler(InputByteCheckPort, OnInputByte);
    AttachZ80OutputHandler(OutputByteCheckPort, OnOutputByte);

    AttachFrameStartHandler(OnFrameStart);
    AttachAfterFrameRenderHandler(OnAfterFrameRender);

    soundMixer.AddSource(&sndRenderer);

    std::string filename;
    size_t offset;

    filename = config->getString("sound", "gsrom", "gs105a.rom");
    filename = split_romname(filename, &offset);

    if (host->storage()->readExtras("roms", filename, mem, 0x8000, offset) != 0x8000) {
        throw std::runtime_error(std::string("Couldn't find \"roms/") + filename + "\"");
    }

    gsCpu = z80ex_create(
        GsReadByte,
        nullptr,
        GsWriteByte,
        nullptr,
        GsInputByte,
        nullptr,
        GsOutputByte,
        nullptr,
        GsReadIntVec,
        nullptr
    );

    Reset();
}

void C_GSound::Close(void) {
    if (gsCpu) {
        z80ex_destroy(gsCpu);
        gsCpu = nullptr;
    }
}

bool C_GSound::InputByteCheckPort(uint16_t port) {
    return ((port & 0xFF) == 0xB3 || (port & 0xFF) == 0xBB);
}

bool C_GSound::OutputByteCheckPort(uint16_t port) {
    return ((port & 0xFF) == 0xB3 || (port & 0xFF) == 0xBB);
}

bool C_GSound::OnInputByte(uint16_t port, uint8_t& retval) {
    Update(devClk);

    if ((port & 0xFF) == 0xB3) {
        retval = regOutput;
        regStatus &= ~0x80;
    } else {
        retval = regStatus;
    }

    return true;
}

bool C_GSound::OnOutputByte(uint16_t port, uint8_t value) {
    Update(devClk);

    if ((port & 0xFF) == 0xB3) {
        regData = value;
        regStatus |= 0x80;
    } else {
        regCommand = value;
        regStatus |= 0x01;
    }

    return true;
}

void C_GSound::OnFrameStart(void) {
    if (SOUND_ENABLED) {
        sndRenderer.StartFrame();
    }
}

void C_GSound::OnAfterFrameRender(void) {
    Update(lastDevClk);
    gsClk -= GS_DEV_TO_CLK(lastDevClk);

    if (SOUND_ENABLED) {
        sndRenderer.EndFrame(lastDevClk);
    }
}

void C_GSound::Reset(void) {
    if (!gsCpu) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        volume[i] = 0;
        channel[i] = 0;
    }

    memPage = 0;
    UpdateMaps();

    z80ex_reset(gsCpu);
}

void C_GSound::UpdateMaps() {
    readMap[0] = mem;
    readMap[1] = &mem[0x8000 + 0x4000];
    readMap[2] = &mem[0x8000 * memPage];
    readMap[3] = &mem[0x8000 * memPage + 0x4000];

    writeMap[0] = nullptr;
    writeMap[1] = &mem[0x8000 + 0x4000];
    writeMap[2] = (memPage == 0 ? nullptr : &mem[0x8000 * memPage]);
    writeMap[3] = (memPage == 0 ? nullptr : &mem[0x8000 * memPage + 0x4000]);
}

void C_GSound::Update(unsigned clk) {
    unsigned nextGsClk = GS_DEV_TO_CLK(clk);
    unsigned nextIntClk = gsClk - (gsClk % GS_INT_PERIOD) + GS_INT_PERIOD;

    if ((gsClk % GS_INT_PERIOD) < 32) {
        unsigned tmp = gsClk - (gsClk % GS_INT_PERIOD) + 32;

        while (gsClk < tmp && gsClk < nextGsClk) {
            gsClk += (unsigned)z80ex_int(gsCpu);
            gsClk += (unsigned)z80ex_step(gsCpu);
        }
    }

    while (gsClk < nextGsClk) {
        while (gsClk < nextIntClk && gsClk < nextGsClk) {
            gsClk += (unsigned)z80ex_step(gsCpu);
        }

        if (gsClk >= nextIntClk && gsClk < nextGsClk) {
            unsigned tmp = nextIntClk + 32;

            while (gsClk < tmp && gsClk < nextGsClk) {
                gsClk += (unsigned)z80ex_int(gsCpu);
                gsClk += (unsigned)z80ex_step(gsCpu);
            }

            nextIntClk = gsClk - (gsClk % GS_INT_PERIOD) + GS_INT_PERIOD;
        }
    }
}

void C_GSound::UpdateSound() {
    unsigned lt = (unsigned)volume[0] * (unsigned)channel[0] + (unsigned)volume[1] * (unsigned)channel[1];
    unsigned rt = (unsigned)volume[2] * (unsigned)channel[2] + (unsigned)volume[3] * (unsigned)channel[3];

    sndRenderer.Update((unsigned)GS_CLK_TO_DEV(gsClk), (lt << 1) + (rt >> 4), (rt << 1) + (lt >> 4));
}

uint8_t C_GSound::GsReadByte(Z80EX_CONTEXT_PARAM uint16_t addr, int m1_state, void* userData) {
    uint8_t value = readMap[(unsigned)addr >> 14][addr & 0x3FFF];

    if ((addr & 0xE000) == 0x6000) {
        channel[(addr >> 8) & 3] = value;

        if (SOUND_ENABLED) {
            UpdateSound();
        }
    }

    return value;
}

void C_GSound::GsWriteByte(Z80EX_CONTEXT_PARAM uint16_t addr, uint8_t value, void* userData) {
    unsigned bank = (unsigned)addr >> 14;

    if (writeMap[bank]) {
        writeMap[bank][addr & 0x3FFF] = value;
    }
}

uint8_t C_GSound::GsInputByte(Z80EX_CONTEXT_PARAM uint16_t port, void* userData) {
    switch (port & 0x0F) {
        case 0x01:
            return regCommand;

        case 0x02:
            regStatus &= ~0x80;
            return regData;

        case 0x04:
            return regStatus;

        case 0x05:
            regStatus &= ~0x01;
            return 0xFF;

        case 0x0A:
            regStatus = 0x7E | (regStatus & 0x01) | (((memPage & 0x01) ^ 0x01) << 7);
            return 0xFF;

        case 0x0B:
            regStatus = 0x7E | (regStatus & 0x80) | ((volume[3] & 0x20) >> 5);
            return 0xFF;
    }

    return 0xFF;
}

void C_GSound::GsOutputByte(Z80EX_CONTEXT_PARAM uint16_t port, uint8_t value, void* userData) {
    switch (port & 0x0F) {
        case 0x00:
            memPage = value & GS_MEMPAGE_MASK;
            UpdateMaps();
            return;

        case 0x03:
            regOutput = value;
            regStatus |= 0x80;
            return;

        case 0x05:
            regStatus &= ~0x01;
            return;

        case 0x06: // fallthrough
        case 0x07: // fallthrough
        case 0x08: // fallthrough
        case 0x09:
            volume[(port & 0x0F) - 0x06] = value & 0x3F;

            if (SOUND_ENABLED) {
                UpdateSound();
            }

            return;

        case 0x0A:
            regStatus = 0x7E | (regStatus & 0x01) | (((memPage & 0x01) ^ 0x01) << 7);
            return;

        case 0x0B:
            regStatus = 0x7E | (regStatus & 0x80) | ((volume[3] & 0x20) >> 5);
            return;
    }
}

uint8_t C_GSound::GsReadIntVec(Z80EX_CONTEXT_PARAM void* userData) {
    return 0xFF;
}
