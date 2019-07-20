// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "tape.h"
#include "zemu.h"
#include "tap_format.h"
#include "wav_format.h"
#include "voc_format.h"

uint64_t C_Tape::prevDevClkCounter = 0;
C_TapeFormat* C_Tape::currentFormat = nullptr;
C_SndRenderer C_Tape::sndRenderer;

void C_Tape::Init(void) {
    AttachFrameStartHandler(OnFrameStart);
    AttachAfterFrameRenderHandler(OnAfterFrameRender);
    soundMixer.AddSource(&sndRenderer);
}

void C_Tape::Close(void) {
    if (currentFormat != nullptr) {
        delete currentFormat;
    }
}

void C_Tape::OnFrameStart(void) {
    if (SOUND_ENABLED) {
        sndRenderer.StartFrame();
    }
}

void C_Tape::OnAfterFrameRender(void) {
    if (SOUND_ENABLED) {
        sndRenderer.EndFrame(lastDevClk);
    }
}

int C_Tape::GetCurrBit(void) {
    return (currentFormat == nullptr ? 1 : currentFormat->GetCurrBit());
}

void C_Tape::Process(void) {
    if (!currentFormat) {
        return;
    }

    if (SOUND_ENABLED) {
        unsigned val = (currentFormat->GetCurrBit() ? MAX_TAPE_VOL : 0);
        sndRenderer.Update(devClk, val, val);
    }

    if (currentFormat->ProcessTicks(devClkCounter - prevDevClkCounter)) {
        prevDevClkCounter = devClkCounter;
    }
}

bool C_Tape::IsLoaded(void) {
    return (currentFormat != nullptr);
}

bool C_Tape::IsActive(void) {
    return (currentFormat == nullptr ? false : currentFormat->IsActive());
}

unsigned int C_Tape::GetPosPerc(void) {
    return (currentFormat == nullptr ? 0 : currentFormat->GetPosPerc());
}

void C_Tape::Eject(void) {
    if (currentFormat != nullptr) {
        delete currentFormat;
        currentFormat = nullptr;
    }
}

bool C_Tape::IsTapeFormat(const char* fname) {
    auto ext = host->storage()->path(fname)->extensionLc();
    return (ext == ".tap" || ext == ".wav" || ext == ".voc");
}

bool C_Tape::Insert(const char* fname) {
    if (currentFormat != nullptr) {
        delete currentFormat;
        currentFormat = nullptr;
    }

    auto ext = host->storage()->path(fname)->extensionLc();

    if (ext == ".tap") {
        currentFormat = new C_TapFormat();
    } else if (ext == ".wav") {
        currentFormat = new C_WavFormat();
    } else if (ext == ".voc") {
        currentFormat = new C_VocFormat();
    }

    if (currentFormat == nullptr) {
        return false;
    }

    if (!currentFormat->Load(fname)) {
        delete currentFormat;
        currentFormat = nullptr;
        return false;
    }

    return true;
}

void C_Tape::Start(void) {
    if (currentFormat != nullptr) {
        prevDevClkCounter = devClkCounter;
        currentFormat->Start();
    }
}

void C_Tape::Stop(void) {
    if (currentFormat != nullptr) {
        currentFormat->Stop();
    }
}

void C_Tape::Rewind(void) {
    if (currentFormat != nullptr) {
        currentFormat->Rewind();
    }
}
