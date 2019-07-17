#ifndef _VOC_FORMAT_H_INCLUDED_
#define _VOC_FORMAT_H_INCLUDED_

#include "zemu_env.h"
#include "tape_format.h"

class C_VocFormat : public C_TapeFormat {
public:

    DataReaderPtr reader;
    uint64_t allTicks = 0;
    bool active = false;
    bool currBit = false;

    // unsigned long dataSize;

    // WORD compression;
    // WORD channels;
    // DWORD rate;
    // WORD bits;
    // DWORD dataSize;
    // DWORD dataFp;
    // DWORD dataPos;
    // unsigned int divider;
    // unsigned int sampleSz;

    C_VocFormat() {}
    virtual ~C_VocFormat() {}

    bool Load(const char* fname);
    bool ProcessTicks(uint64_t ticks);
    bool GetCurrBit(void);
    void Stop(void);
    void Start(void);
    void Rewind(void);
    unsigned int GetPosPerc(void);
    bool IsActive(void);
};

#endif
