// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifdef _WIN32

#include <string>
#include <stdexcept>
#include <windows.h>
#include <boost/format.hpp>
#include "sound_driver_win32.h"

SoundDriverWin32::SoundDriverWin32(int soundFreq, int wqSize) {
    if (wqSize < 0 || wqSize > MAX_WQSIZE) {
        throw std::logic_error((
            boost::format("\"wqSize\" should be 0 (default value) or between 1 and %d exclusive") % MAX_WQSIZE
        ).str());
    }

    if (wqSize == 0) {
        wqSize = 4;
    }

    this->wqSize = wqSize;

    WAVEFORMATEX wf = {0};
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nSamplesPerSec = soundFreq;
    wf.nChannels = 2;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = 4;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

    waveOutOpen(&hwo, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL);
    wqhead = 0;
    wqtail = 0;
}

SoundDriverWin32::~SoundDriverWin32() {
    if (hwo) {
        waveOutReset(hwo);
        waveOutClose(hwo);
    }
}

void SoundDriverWin32::render(uint32_t* buffer, int samples) {
    for (;;) {
        while ((wqTail != wqHead) && (wq[wqTail].dwFlags & WHDR_DONE)) {
            waveOutUnprepareHeader(hwo, &wq[wqTail], sizeof(WAVEHDR));

            if (++wqTail == wqSize) {
                wqTail = 0;
            }
        }

        if ((wqHead + 1) % wqSize != wqTail) {
            break;
        }

        Sleep(10);
    }

    LPSTR bfPos = (LPSTR)(wbuffer + wqHead * MAX_DSPIECE);
    memcpy(bfPos, (uint8_t*)buffer, samples * sizeof(uint32_t));

    wq[wqHead].lpData = bfPos;
    wq[wqHead].dwBufferLength = samples * sizeof(uint32_t);
    wq[wqHead].dwFlags = 0;

    waveOutPrepareHeader(hwo, &wq[wqHead], sizeof(WAVEHDR));
    waveOutWrite(hwo, &wq[wqHead], sizeof(WAVEHDR));

    if (++wqHead == wqSize) {
        wqHead = 0;
    }
}

#endif
