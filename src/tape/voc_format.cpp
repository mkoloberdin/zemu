// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "voc_format.h"
#include "../defines.h"

#define VOC_THRESHOLD 140

bool C_VocFormat::Load(const char* fname) {
    active = false;
    reader = hostEnv->fileSystem()->path(fname)->fileReader();

    char buf[21];
    buf[20] = 0;

    reader->readBlock(buf, 20);

    if (strcmp(buf, "Creative Voice File\032") != 0) {
        reader = nullptr;
        DEBUG_MESSAGE("\"Creative Voice File\" string not found");
        return false;
    }

    uint16_t dataOffset = reader->readWord();
    uint16_t verNum = reader->readWord();
    uint16_t verChk = reader->readWord();

    if ((uint16_t)((0xFFFF - verNum) + 0x1234) != verChk) {
        reader = nullptr;
        DEBUG_MESSAGE("Invalid voc file");
        return false;
    }

    long maxRate = 0;
    long rate;

    reader->setPosition(dataOffset);

    while (!reader->isEof()) {
        uint8_t type = reader->readByte();

        if (type == 0) {
            break; // Terminator
        }

        long size = (long)reader->readByte() + (long)0x100 * (long)reader->readWord();
        long pos = reader->getPosition();

        if (type == 1) { // Sound data
            long sr = reader->readByte();
            rate = 1000000 / (256 - sr);

            if (rate > maxRate) {
                maxRate = rate;
            }
        } else if (type == 3) { // Silence
            reader->readWord(); // skip Lenght of silence
            long sr = reader->readByte();
            rate = 1000000 / (256 - sr);

            if (rate > maxRate) {
                maxRate = rate;
            }
        } else if (type == 8) { // Extended
            long tc = reader->readWord();
            reader->readByte(); // skip Pack
            rate = 256000000 / (65536 - tc);

            if (reader->readByte()) {
                rate /= 2; // Stereo
            }

            if (rate > maxRate) {
                maxRate = rate;
            }
        } else if (type == 9) {
            rate = reader->readWord();

            if (rate > maxRate) {
                maxRate = rate;
            }
        }

        reader->setPosition(pos + size);
    }

    // dataSize = sz * rate
    reader->setPosition(dataOffset);

    // dataPos = 0;
    // allTicks = 0;
    // sampleSz = (bits == 8 ? 1 : (bits == 16 ? 2 : 3)) * channels;
    // divider = (MAX_FRAME_TACTS * 50) / rate; // 71680 = MAX_FRAME_TACTS

    return true;
}

bool C_VocFormat::ProcessTicks(uint64_t ticks)
{
    /*
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

    if (cnt <= 0) {
        return true;
    }

    while (cnt--) {
        reader->readByte();
        dataPos++;
    }

    long long res = 0;

    switch (bits) {
        case 8:
            for (int i = 0; i < channels; i++) {
                res += reader->readByte();
            }

            break;

        case 16:
            for (int i = 0; i < channels; i++) {
                res += (short)reader->readWord();
            }

            break;

        case 32:
            for (int i = 0; i < channels; i++) {
                res += (long)reader->readWord();
            }

            break;
    }

    res /= channels;

    switch (bits) {
        case 8:
            dataPos += channels;
            break;

        case 16:
            dataPos += channels * 2;
            res /= 0x100;
            break;

        case 32:
            dataPos += channels * 4;
            res /= 0x10000;
            break;
    }

    if (res < 0) {
        res = -res;
    }

    currBit = (res > WAV_THRESHOLD);
    return true;
    */

    return false;
}

bool C_VocFormat::GetCurrBit(void) {
    return (active ? currBit : true);
}

void C_VocFormat::Stop(void) {
    active = false;
}

void C_VocFormat::Start(void) {
    active = true;
}

void C_VocFormat::Rewind(void) {
    // dataPos = 0;
    allTicks = 0;
}

unsigned int C_VocFormat::GetPosPerc(void) {
    // uint32_t pos = (allTicks / divider) * sampleSz;
    // return ((pos >= dataSize) ? 100 : (pos * 100 / dataSize));

    return 0;
}

bool C_VocFormat::IsActive(void) {
    return active;
}
