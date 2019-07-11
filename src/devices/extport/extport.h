#ifndef _EXTPORT_H_INCLUDED_
#define _EXTPORT_H_INCLUDED_

#include "../../zemu.h"
#include "../device.h"

class C_ExtPort : public C_Device {
    public:

    static Z80EX_BYTE portEFF7;
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

    static bool OutputByteCheckPort(Z80EX_WORD port);
    static bool OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value);
    static void OnReset(void);
};

#endif
