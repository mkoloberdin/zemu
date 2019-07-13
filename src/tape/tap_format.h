#ifndef _TAP_FORMAT_H_INCLUDED_
#define _TAP_FORMAT_H_INCLUDED_

#include "tape_format.h"

class C_TapFormat : public C_TapeFormat {
    public:

    int state;
    int tapeBit = 1;
    int counter = 0;
    uint8_t* data = nullptr;
    long size = 0;

    long blockPos = 0;
    int blockSize = 0;
    int posInBlock = 0;
    uint8_t currentByte = 0;
    int delay = 0;

    C_TapFormat();
    virtual ~C_TapFormat();

    bool Load(const char* fname);
    bool ProcessTicks(uint64_t ticks);
    bool GetCurrBit(void);
    void Stop(void);
    void Start(void);
    void Rewind(void);
    unsigned int GetPosPerc(void);
    bool IsActive(void);

    uint8_t Data(long pos);
};

#endif
