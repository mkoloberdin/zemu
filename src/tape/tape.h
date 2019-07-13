#ifndef _TAPE_H_INCLUDED_
#define _TAPE_H_INCLUDED_

#include "../sound/snd_renderer.h"
#include "tape_format.h"

#define MAX_TAPE_VOL 0x03FF

class C_Tape {
    public:

    static C_SndRenderer sndRenderer;
    static uint64_t prevDevClkCounter;
    static C_TapeFormat* currentFormat;

    static void Init(void);
    static void Close(void);
    static void OnFrameStart(void);
    static void OnAfterFrameRender(void);

    static int GetCurrBit(void);
    static void Process(void);

    static bool IsLoaded(void);
    static bool IsActive(void);
    static unsigned int GetPosPerc(void);

    static bool IsTapeFormat(const char* fname);

    static void Eject(void);
    static bool Insert(const char* fname);
    static void Start(void);
    static void Stop(void);
    static void Rewind(void);
};

#endif
