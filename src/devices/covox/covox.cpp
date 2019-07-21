// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "covox.h"

C_SndRenderer C_Covox::sndRenderer;
bool C_Covox::enabled = false;

void C_Covox::Init(void) {
    enabled = host->config()->getBool("sound", "enablecovox", false);

    if (enabled) {
        AttachZ80OutputHandler(OutputByteCheckPort, OnOutputByte);

        AttachFrameStartHandler(OnFrameStart);
        AttachAfterFrameRenderHandler(OnAfterFrameRender);
        AttachResetHandler(OnReset);

        soundMixer.AddSource(&sndRenderer);
    }
}

void C_Covox::Close(void) {
}

bool C_Covox::OutputByteCheckPort(uint16_t port) {
    return ((port & 0x07) == 0x03);
}

bool C_Covox::OnOutputByte(uint16_t port, uint8_t value) {
    if (SHOULD_OUTPUT_SOUND) {
        unsigned vol = ((unsigned)value << 7);
        sndRenderer.Update(devClk, vol, vol);
    }

    return true;
}

void C_Covox::OnFrameStart(void) {
    if (SHOULD_OUTPUT_SOUND) {
        sndRenderer.StartFrame();
    }
}

void C_Covox::OnAfterFrameRender(void) {
    if (SHOULD_OUTPUT_SOUND) {
        sndRenderer.EndFrame(lastDevClk);
    }
}

void C_Covox::OnReset(void) {
    if (SHOULD_OUTPUT_SOUND) {
        sndRenderer.Update(0, 0, 0);
    }
}
