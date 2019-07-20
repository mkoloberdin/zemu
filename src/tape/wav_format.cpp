// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "wav_format.h"
#include "defines.h"
#include "params.h"

#define WAV_THRESHOLD 140

bool C_WavFormat::Load(const char* fname)
{
    active = false;
    reader = host->storage()->path(fname)->dataReader();

    char chunkName[5];
    chunkName[4] = 0;

    reader->readBlock(chunkName, 4);

    if (strcmp(chunkName, "RIFF") != 0) {
        reader = nullptr;
        DEBUG_MESSAGE("RIFF chunk not found");
        return false;
    }

    reader->readDword();  // skip chunk size
    reader->readBlock(chunkName, 4);

    if (strcmp(chunkName, "WAVE") != 0) {
        reader = nullptr;
        DEBUG_MESSAGE("Invalid RIFF type (must be WAVE)");
        return false;
    }

    bool fmtFound = false;

    while (!reader->isEof() && !fmtFound) {
        reader->readBlock(chunkName, 4);

        if (reader->isEof()) {
            break;
        }

        uint32_t size = reader->readDword();

        if (reader->isEof()) {
            break;
        }

        if (strcmp(chunkName, "fmt ") != 0) {
            reader->setPosition(reader->getPosition() + size);
            continue;
        }

        uint16_t compression = reader->readWord();

        if (compression != 1) {
            reader = nullptr;
            DEBUG_MESSAGE("Only uncompressed PCM supported");
            return false;
        }

        channels = reader->readWord();
        rate = reader->readDword();

        reader->readDword(); // skip byte rate
        reader->readWord(); // skip block align

        bits = reader->readWord();

        if (bits != 8 && bits != 16 && bits != 32) {
            reader = nullptr;
            DEBUG_MESSAGE("Unsupported bits per sample. 8, 16 or 32 supported");
            return false;
        }

        fmtFound = true;
    }

    if (!fmtFound) {
        reader = nullptr;
        DEBUG_MESSAGE("fmt subchunk not found");
        return false;
    }

    bool dataFound = false;
    reader->setPosition(12);

    while (!reader->isEof() && !dataFound) {
        reader->readBlock(chunkName, 4);

        if (reader->isEof()) {
            break;
        }

        dataSize = reader->readDword();

        if (reader->isEof()) {
            break;
        }

        if (strcmp(chunkName, "data") != 0) {
            reader->setPosition(reader->getPosition() + dataSize);
            continue;
        }

        // dataFp = reader->getPosition();
        dataFound = true;
    }

    if (!dataFound) {
        reader = nullptr;
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
    //     reader->readByte();
    //     dataPos++;
    // }

    while (samplesCnt--) {
        int64_t res = 0;

        switch (bits) {
            case 8:
                for (int i = 0; i < channels; i++) {
                    res += reader->readByte();
                }

                dataPos += channels;
                break;

            case 16:
                for (int i = 0; i < channels; i++) {
                    res += (short)reader->readWord();
                }

                dataPos += channels * 2;
                res /= 0x100;
                break;

            case 32:
                for (int i = 0; i < channels; i++) {
                    res += (long)reader->readDword();
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

    if (reader) {
        reader->setPosition(0);
    }
}

unsigned int C_WavFormat::GetPosPerc(void) {
    uint32_t pos = (allTicks / divider) * sampleSz;
    return ((pos >= dataSize) ? 100 : (pos * 100 / dataSize));
}

bool C_WavFormat::IsActive(void) {
    return active;
}
