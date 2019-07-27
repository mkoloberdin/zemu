// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "mouse.h"

uint8_t C_Mouse::portFBDF;
uint8_t C_Mouse::portFFDF;
uint8_t C_Mouse::portFADF;
uint8_t C_Mouse::wheelCnt;

void C_Mouse::Init(void) {
    AttachZ80InputHandler(InputByteCheckPort, OnInputByte);
    AttachHwHandler(STAGE_EVENT_MOUSEWHEEL, OnHwMouseWheel);

    portFBDF = 128;
    portFFDF = 96;
    portFADF = 255;
    wheelCnt = 255;
}

void C_Mouse::Close(void) {
}

void C_Mouse::UpdateState(void) {
    StageMouseState state;
    host->stage()->getRelativeMouseState(&state);

    portFBDF += state.x / params.mouseDivX;
    portFFDF -= state.y / params.mouseDivY;
    portFADF = (wheelCnt << 4) | 0x0F;

    if (state.buttons & STAGE_MOUSE_LMASK) {
        portFADF &= ~1;
    }

    if (state.buttons & STAGE_MOUSE_RMASK) {
        portFADF &= ~2;
    }

    if (state.buttons & STAGE_MOUSE_MMASK) {
        portFADF &= ~3; // middle button works as LMB + RMB
    }
}

bool C_Mouse::InputByteCheckPort(uint16_t port) {
    return ((port == 0xFBDF) | (port == 0xFFDF) | (port == 0xFADF));
}

bool C_Mouse::OnInputByte(uint16_t port, uint8_t& retval) {
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

bool C_Mouse::OnHwMouseWheel(StageEvent& event) {
    if (event.mouseWheelDirection < 0) {
        wheelCnt++;
        return true;
    }

    if (event.mouseWheelDirection > 0) {
        wheelCnt--;
        return true;
    }

    return false;
}
