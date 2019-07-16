#ifndef _EXTPORT_H_INCLUDED_
#define _EXTPORT_H_INCLUDED_

#include "zemu.h"
#include "../device.h"

class C_ExtPort : public C_Device {
public:

    static uint8_t portEFF7;
    static bool oldEFF7Mode;
    static bool useEFF7Turbo;
    static bool enabled;

    void Init(void);
    void Close(void);

    static bool Is16Colors(void);
    static bool Is512x192(void);
    static bool Is128Lock(void);
    static bool IsRamMapRom(void);
    static bool IsGigascreen(void);
    static bool IsMulticolor(void);
    static bool Is384x304(void);
    static bool IsCmos(void);

    static bool OutputByteCheckPort(uint16_t port);
    static bool OnOutputByte(uint16_t port, uint8_t value);
    static void OnReset(void);
};

#endif
