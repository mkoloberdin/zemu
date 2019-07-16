#ifndef _MOUSE_H_INCLUDED_
#define _MOUSE_H_INCLUDED_

#include "zemu.h"
#include "../device.h"

class C_Mouse : public C_Device {
public:

    static uint8_t portFBDF;
    static uint8_t portFFDF;
    static uint8_t portFADF;
    static uint8_t wheelCnt;

    void Init(void);
    void Close(void);

    static void UpdateState(void);
    static bool InputByteCheckPort(uint16_t port);
    static bool OnInputByte(uint16_t port, uint8_t& retval);
    static bool OnHwMouseButtonDown(ZHW_Event& ev);
};

#endif
