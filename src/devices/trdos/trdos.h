#ifndef _TRDOS_H_INCLUDED_
#define _TRDOS_H_INCLUDED_

#include "../../zemu.h"
#include "../device.h"
#include "../../lib_wd1793/wd1793_chip.h"

class C_TrDos : public C_Device {
    public:

    static bool trdos;
    static uint8_t rom[0x4000];

    static void ReadFile(void);
    void Init(void);
    void Close(void);

    static ptrOnReadByteFunc ReadByteCheckAddr(uint16_t addr, bool m1);
    static uint8_t OnReadByte_3Dxx_M1(uint16_t addr, bool m1);
    static uint8_t OnReadByte_RAM_M1(uint16_t addr, bool m1);
    static uint8_t OnReadByte_ROM(uint16_t addr, bool m1);
    static bool InputOutputByteCheckPort(uint16_t port);
    static bool OnInputByte(uint16_t port, uint8_t& retval);
    static bool OnOutputByte(uint16_t port, uint8_t value);
    static void OnReset(void);

    static void Enable(void);
    static void Disable(void);
    static DRIVE_STATE GetIndicatorState(void);
};

#endif
