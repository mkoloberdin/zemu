#ifndef _KEMPSTON_H_INCLUDED_
#define _KEMPSTON_H_INCLUDED_

#include "zemu.h"
#include "../device.h"

class C_KempstonStick : public C_Device {
public:

    static uint8_t joyState;
    static uint8_t joyOnKeybState;

    void Init(void);
    void Close(void);

    static bool OnKeyDown(StageEvent& event);
    static bool OnKeyUp(StageEvent& event);
    static bool OnJoyDown(StageEvent& event);
    static bool OnJoyUp(StageEvent& event);

    static bool InputByteCheckPort(uint16_t port);
    static bool OnInputByte(uint16_t port, uint8_t& retval);
};

#endif
