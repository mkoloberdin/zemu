#ifndef _COVOX_H_INCLUDED_
#define _COVOX_H_INCLUDED_

#include "../../zemu.h"
#include "../../sound/mixer.h"
#include "../device.h"

class C_Covox : public C_Device {
    public:

    static C_SndRenderer sndRenderer;
    static bool enabled;

    void Init(void);
    void Close(void);

    static bool OutputByteCheckPort(uint16_t port);
    static bool OnOutputByte(uint16_t port, uint8_t value);
    static void OnFrameStart(void);
    static void OnAfterFrameRender(void);
    static void OnReset(void);
};

#endif
