#ifndef _GSOUND_H_INCLUDED_
#define _GSOUND_H_INCLUDED_

#include "../../zemu.h"
#include "../../sound/mixer.h"
#include "../device.h"

// 16 pages for 512mb (classic GS)
// 64 pages for 2gb (NeoGS)

#define GS_MEM_PAGES (16) // including rom, so actual memory pages = GS_PAGES - 1
#define GS_MEMPAGE_MASK (15)

class C_GSound : public C_Device {
public:

    static C_SndRenderer sndRenderer;
    static bool enabled;

    void Init(void);
    void Close(void);

    static bool InputByteCheckPort(uint16_t port);
    static bool OutputByteCheckPort(uint16_t port);
    static bool OnInputByte(uint16_t port, uint8_t& retval);
    static bool OnOutputByte(uint16_t port, uint8_t value);
    static void OnFrameStart(void);
    static void OnAfterFrameRender(void);
    static void Reset(void);

private:

    static void UpdateMaps();
    static void Update(unsigned clk);
    static void UpdateSound();

    static uint8_t GsReadByte(Z80EX_CONTEXT_PARAM uint16_t addr, int m1_state, void* userData);
    static void GsWriteByte(Z80EX_CONTEXT_PARAM uint16_t addr, uint8_t value, void* userData);
    static uint8_t GsInputByte(Z80EX_CONTEXT_PARAM uint16_t port, void* userData);
    static void GsOutputByte(Z80EX_CONTEXT_PARAM uint16_t port, uint8_t value, void* userData);
    static uint8_t GsReadIntVec(Z80EX_CONTEXT_PARAM void* userData);

    static uint8_t regCommand;
    static uint8_t regStatus;
    static uint8_t regData;
    static uint8_t regOutput;
    static uint8_t volume[4];
    static uint8_t channel[4];
    static uint8_t memPage;

    static Z80EX_CONTEXT* gsCpu;
    static uint8_t mem[0x8000 * GS_MEM_PAGES];

    static unsigned gsClk;
    static uint8_t* readMap[4];
    static uint8_t* writeMap[4];
};

#endif
