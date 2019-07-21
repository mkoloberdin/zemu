// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <string>
#include <stdexcept>
#include <boost/format.hpp>
#include <SDL_audio.h>
#include <SDL_timer.h>
#include "sound_driver_generic.h"

namespace {
    const int MAX_WRITE_WAIT_MILLIS = 2000;
    const int MAX_EMPTY_FILLS = 8;
}

void soundDriverGenericAudioCallback(void* userData, uint8_t* stream, int len) {
    static_cast<SoundDriverGeneric*>(userData)->fillStream(stream, len);
}

SoundDriverGeneric::SoundDriverGeneric(int soundFreq, int bufferScale, int preBufferFragments, int fragmentSamplesScale) {
    if (bufferScale < 0 || bufferScale > 4) {
        throw std::logic_error("\"bufferScale\" should be 0 (default value) or between 1 and 4 inclusive");
    }

    if (bufferScale == 0) {
        bufferScale = 2;
    }

    int bufferFragments = (1 << bufferScale);

    if (preBufferFragments < 0 || preBufferFragments >= bufferFragments) {
        throw std::logic_error((
            boost::format("\"preBufferFragments\" should be 0 (default value) or between 1 and %d inclusive (depends on \"bufferScale\")")
            % (bufferFragments - 1)
        ).str());
    }

    if (preBufferFragments == 0) {
        // bufferFragments >= 2, so will be preBufferFragments >= 1
        preBufferFragments = bufferFragments * 2 / 3;
    }

    if (fragmentSamplesScale < 0 || fragmentSamplesScale > 5) {
        throw std::logic_error("\"fragmentSamplesScale\" should be 0 (default value) or between 1 and 5 inclusive");
    }

    if (fragmentSamplesScale == 0) {
        fragmentSamplesScale = 2;
    }

    // 1 -> 512, 2 -> 1024, 3 -> 2048, 4 -> 4096, 5 -> 8192
    int samples = 256 << fragmentSamplesScale;

    SDL_AudioSpec desired;
    SDL_AudioSpec obtained;

    desired.freq = soundFreq;
    desired.format = AUDIO_S16SYS;
    desired.channels = 2;
    desired.samples = samples;
    desired.callback = soundDriverGenericAudioCallback;
    desired.userdata = this;

    if (SDL_OpenAudio(&desired, &obtained) < 0) {
        throw std::runtime_error(std::string("SDL_OpenAudio() failed: ") + SDL_GetError());
    }

    bufferSize = (samples << bufferScale) * 4;
    bufferSizeMask = bufferSize - 1;
    preBufferSize = samples * preBufferFragments * 4;
    ringBuffer = new uint8_t[bufferSize];

    SDL_PauseAudio(0);
}

SoundDriverGeneric::~SoundDriverGeneric() {
    SDL_CloseAudio();

    if (ringBuffer) {
        delete[] ringBuffer;
    }
}

void SoundDriverGeneric::render(uint32_t* buffer, int samples) {
    if (samples <= 0) {
        return;
    }

    uint8_t* byteBuffer = (uint8_t*)buffer;
    int byteSamples = samples * 4;
    int waitMillis = 0;

    for (;;) {
        int distance = ((writePosition >= playPosition)
            ? (bufferSize - writePosition + playPosition)
            : (playPosition - writePosition)
        );

        if (distance < 8) {
            SDL_Delay(1);
            waitMillis++;

            if (waitMillis < MAX_WRITE_WAIT_MILLIS) {
                continue;
            }

            SDL_LockAudio();
            emptyFills = 0;
            isInitialLoop = true;
            playPosition = 0;
            writePosition = 0;
            SDL_UnlockAudio();

            break;
        }

        waitMillis = 0;
        int len = std::min(distance - 1, byteSamples);
        byteSamples -= len;

        SDL_LockAudio();

        if (writePosition + len <= bufferSize) {
            memcpy(ringBuffer + writePosition, byteBuffer, len);
        } else {
            int partSize = bufferSize - writePosition;
            memcpy(ringBuffer + writePosition, byteBuffer, partSize);
            memcpy(ringBuffer, byteBuffer + partSize, len - partSize);
        }

        writePosition = (writePosition + len) % bufferSizeMask;
        byteBuffer += len;

        SDL_UnlockAudio();

        if (byteSamples <= 0) {
            break;
        }
    }
}

void SoundDriverGeneric::fillStream(uint8_t* stream, int len) {
    if (len <= 0) {
        return;
    }

    if (isInitialLoop) {
        if (writePosition < preBufferSize) {
            memset(stream, 0, len);
            return;
        }

        isInitialLoop = false;
    }

    int distance = ((playPosition >= writePosition)
        ? (bufferSize - playPosition + writePosition)
        : (writePosition - playPosition)
    );

    if (distance <= len) {
        memset(stream, 0, len);
        emptyFills += 2;

        if (emptyFills > MAX_EMPTY_FILLS) {
            emptyFills = 0;
            isInitialLoop = true;
            playPosition = 0;
            writePosition = 0;
        }

        return;
    }

    if (emptyFills > 0) {
        --emptyFills;
    }

    if (playPosition + len <= bufferSize) {
        memcpy(stream, ringBuffer + playPosition, len);
    } else {
        int partSize = bufferSize - playPosition;
        memcpy(stream, ringBuffer + playPosition, partSize);
        memcpy(stream + partSize, ringBuffer, len - partSize);
    }

    playPosition = (playPosition + len) % bufferSizeMask;
}
