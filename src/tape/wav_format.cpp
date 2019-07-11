// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "wav_format.h"

#define WAV_THRESHOLD 140

C_WavFormat::C_WavFormat() {
    allTicks = 0;
    active = false;
}

C_WavFormat::~C_WavFormat() {
    if (fl.IsOpened()) {
        fl.Close();
    }
}

bool C_WavFormat::Load(const char* fname)
{
    if (fl.IsOpened()) {
        fl.Close();
    }

    active = false;

    fl.Read(fname);
    char chunkName[5];
    chunkName[4] = 0;

    fl.ReadBlock(chunkName, 4);

    if (strcmp(chunkName, "RIFF")) {
        fl.Close();
        DEBUG_MESSAGE("RIFF chunk not found");
        return false;
    }

    fl.GetDWORD();  // skip chunk size
    fl.ReadBlock(chunkName, 4);

    if (strcmp(chunkName, "WAVE")) {
        fl.Close();
        DEBUG_MESSAGE("Invalid RIFF type (must be WAVE)");
        return false;
    }

    bool fmtFound = false;

    while (!fl.Eof() && !fmtFound) {
        fl.ReadBlock(chunkName, 4);

        if (fl.Eof()) {
            break;
        }

        uint32_t size = fl.GetDWORD();

        if (fl.Eof()) {
            break;
        }

        if (strcmp(chunkName, "fmt ")) {
            fl.SetFilePointer(fl.GetFilePointer() + size);
            continue;
        }

        compression = fl.GetWORD();

        if (compression != 1) {
            fl.Close();
            DEBUG_MESSAGE("Only uncompressed PCM supported");
            return false;
        }

        channels = fl.GetWORD();
        rate = fl.GetDWORD();

        fl.GetDWORD(); // skip byte rate
        fl.GetWORD(); // skip block align

        bits = fl.GetWORD();

        if (bits != 8 && bits != 16 && bits != 32) {
            fl.Close();
            DEBUG_MESSAGE("Unsupported bits per sample. 8, 16 or 32 supported");
            return false;
        }

        fmtFound = true;
    }

    if (!fmtFound) {
        fl.Close();
        DEBUG_MESSAGE("fmt subchunk not found");
        return false;
    }

    bool dataFound = false;
    fl.SetFilePointer(12);

    while (!fl.Eof() && !dataFound) {
        fl.ReadBlock(chunkName, 4);

        if (fl.Eof()) {
            break;
        }

        dataSize = fl.GetDWORD();

        if (fl.Eof()) {
            break;
        }

        if (strcmp(chunkName, "data")) {
            fl.SetFilePointer(fl.GetFilePointer() + dataSize);
            continue;
        }

        dataFp = fl.GetFilePointer();
        dataFound = true;
    }

    if (!dataFound) {
        fl.Close();
        DEBUG_MESSAGE("data chunk not found");
        return false;
    }

    dataPos = 0;
    allTicks = 0;
    sampleSz = (bits == 8 ? 1 : (bits == 16 ? 2 : 3)) * channels;
    divider = (MAX_FRAME_TACTS * 50) / rate;

    return true;
}

bool C_WavFormat::ProcessTicks(uint64_t ticks) {
    if (!active) {
        currBit = true;
        return true;
    }

    allTicks += ticks;
    uint32_t pos = (allTicks / divider) * sampleSz;

    if (pos > dataSize) {
        currBit = 1;
        active = false;
        return true;
    }

    int cnt = (pos - dataPos) - sampleSz;
    if (cnt <= 0) return true;

    int samples = (pos - dataPos) / sampleSz;
    int samplesCnt = samples;
    int64_t allRes = 0;

    // while (cnt--)
    // {
    //     fl.GetBYTE();
    //     dataPos++;
    // }

    while (samplesCnt--) {
        int64_t res = 0;

        switch (bits) {
            case 8:
                for (int i = 0; i < channels; i++) {
                    res += fl.GetBYTE();
                }

                dataPos += channels;
                break;

            case 16:
                for (int i = 0; i < channels; i++) {
                    res += (short)fl.GetWORD();
                }

                dataPos += channels * 2;
                res /= 0x100;
                break;

            case 32:
                for (int i = 0; i < channels; i++) {
                    res += (long)fl.GetDWORD();
                }

                dataPos += channels * 4;
                res /= 0x10000;
                break;
        }

        res /= channels;
        allRes += res;
    }

    allRes /= samples;

    // if (res < 0) { res = -res; }
    // currBit = (res > WAV_THRESHOLD);

    if (allRes < 0) {
        allRes = -allRes;
    }

    currBit = (allRes > WAV_THRESHOLD);
    return true;
}

bool C_WavFormat::GetCurrBit(void) {
    return (active ? currBit : true);
}

void C_WavFormat::Stop(void) {
    active = false;
}

void C_WavFormat::Start(void) {
    active = true;
}

void C_WavFormat::Rewind(void) {
    dataPos = 0;
    allTicks = 0;

    if (fl.IsOpened()) {
        fl.SetFilePointer(0);
    }
}

unsigned int C_WavFormat::GetPosPerc(void) {
    uint32_t pos = (allTicks / divider) * sampleSz;
    return ((pos >= dataSize) ? 100 : (pos * 100 / dataSize));
}

bool C_WavFormat::IsActive(void) {
    return active;
}
