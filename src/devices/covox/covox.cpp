#include "covox.h"

C_SndRenderer C_Covox::sndRenderer;
bool C_Covox::enabled = false;

void C_Covox::Init(void) {
    enabled = config.GetBool("sound", "enablecovox", false);

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
    if (SOUND_ENABLED) {
        unsigned vol = ((unsigned)value << 7);
        sndRenderer.Update(devClk, vol, vol);
    }

    return true;
}

void C_Covox::OnFrameStart(void) {
    if (SOUND_ENABLED) {
        sndRenderer.StartFrame();
    }
}

void C_Covox::OnAfterFrameRender(void) {
    if (SOUND_ENABLED) {
        sndRenderer.EndFrame(lastDevClk);
    }
}

void C_Covox::OnReset(void) {
    if (SOUND_ENABLED) {
        sndRenderer.Update(0, 0, 0);
    }
}
