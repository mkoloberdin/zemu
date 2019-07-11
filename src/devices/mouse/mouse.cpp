#include "mouse.h"

Z80EX_BYTE C_Mouse::portFBDF;
Z80EX_BYTE C_Mouse::portFFDF;
Z80EX_BYTE C_Mouse::portFADF;
Z80EX_BYTE C_Mouse::wheelCnt;

void C_Mouse::Init(void) {
    AttachZ80InputHandler(InputByteCheckPort, OnInputByte);
    AttachHwHandler(ZHW_EVENT_MOUSEWHEEL, OnHwMouseButtonDown);

    portFBDF = 128;
    portFFDF = 96;
    portFADF = 255;
    wheelCnt = 255;
}

void C_Mouse::Close(void) {
}

void C_Mouse::UpdateState(void) {
    int mx;
    int my;

    int bt = ZHW_Moyse_GetRelativeState(&mx, &my);

    portFBDF += mx / params.mouseDiv;
    portFFDF -= my / params.mouseDiv;
    portFADF = (wheelCnt << 4) | 0x0F;

    if (bt & ZHW_MOUSE_LMASK) {
        portFADF &= ~1;
    }

    if (bt & ZHW_MOUSE_RMASK) {
        portFADF &= ~2;
    }

    if (bt & ZHW_MOUSE_MMASK) {
        portFADF &= ~3; // middle button works as LMB + RMB
    }
}

bool C_Mouse::InputByteCheckPort(Z80EX_WORD port) {
    return ((port == 0xFBDF) | (port == 0xFFDF) | (port == 0xFADF));
}

bool C_Mouse::OnInputByte(Z80EX_WORD port, Z80EX_BYTE& retval) {
    UpdateState();

    if (port == 0xFBDF) {
        retval = portFBDF;
    } else if (port == 0xFFDF) {
        retval = portFFDF;
    } else {
        retval = portFADF;
    }

    return true;
}

bool C_Mouse::OnHwMouseButtonDown(ZHW_Event& event) {
    if (ZHW_EVENT_WHEELDIRECTION(event) == ZHW_MOUSE_WHEELUP) {
        wheelCnt++;
        return true;
    }

    if (ZHW_EVENT_WHEELDIRECTION(event) == ZHW_MOUSE_WHEELDOWN) {
        wheelCnt--;
        return true;
    }

    return false;
}
