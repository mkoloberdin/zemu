#ifndef _TAPE_FORMAT_H_INCLUDED_
#define _TAPE_FORMAT_H_INCLUDED_

#include <cstdint>

class C_TapeFormat {
    public:

    C_TapeFormat() {}
    virtual ~C_TapeFormat() {}

    virtual bool Load(const char* fname) = 0;
    virtual bool ProcessTicks(uint64_t ticks) = 0;
    virtual bool GetCurrBit(void) = 0;
    virtual void Stop(void) = 0;
    virtual void Start(void) = 0;
    virtual void Rewind(void) = 0;
    virtual unsigned int GetPosPerc(void) = 0;
    virtual bool IsActive(void) = 0;
};

#endif
