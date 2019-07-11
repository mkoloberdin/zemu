#ifndef _TAP_FORMAT_H_INCLUDED_
#define _TAP_FORMAT_H_INCLUDED_

#include "../zemu.h"
#include "tape_format.h"

class C_TapFormat : public C_TapeFormat {
    public:

    int state;
    int tapeBit;
    int counter;
    uint8_t* data;
    long size;

    long blockPos;
    int blockSize;
    int posInBlock;
    uint8_t currentByte;
    int delay;

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
