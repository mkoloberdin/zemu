// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "kempston.h"

uint8_t C_KempstonStick::joyState = 0;
uint8_t C_KempstonStick::joyOnKeybState = 0;

void C_KempstonStick::Init(void) {
    AttachHwHandler(HW_EVENT_KEYDOWN, OnKeyDown);
    AttachHwHandler(HW_EVENT_KEYUP, OnKeyUp);
    AttachHwHandler(HW_EVENT_JOYDOWN, OnJoyDown);
    AttachHwHandler(HW_EVENT_JOYUP, OnJoyUp);

    AttachZ80InputHandler(InputByteCheckPort, OnInputByte);
}

void C_KempstonStick::Close(void) {
}

bool C_KempstonStick::OnKeyDown(HardwareEvent& event) {
    if (!joyOnKeyb) {
        return false;
    }

    switch (event.keyCode) {
        case HW_KEYCODE_UP:
            joyOnKeybState |= 0x08;
            break;

        case HW_KEYCODE_DOWN:
            joyOnKeybState |= 0x04;
            break;

        case HW_KEYCODE_LEFT:
            joyOnKeybState |= 0x02;
            break;

        case HW_KEYCODE_RIGHT:
            joyOnKeybState |= 0x01;
            break;

        case HW_KEYCODE_RCTRL: // fallthrough
        case HW_KEYCODE_RALT:
            joyOnKeybState |= 0x10;
            break;

        default:
            break;
    }

    return false;
}

bool C_KempstonStick::OnKeyUp(HardwareEvent& event) {
    if (!joyOnKeyb) {
        return false;
    }

    switch (event.keyCode) {
        case HW_KEYCODE_UP:
            joyOnKeybState &= ~0x08;
            break;

        case HW_KEYCODE_DOWN:
            joyOnKeybState &= ~0x04;
            break;

        case HW_KEYCODE_LEFT:
            joyOnKeybState &= ~0x02;
            break;

        case HW_KEYCODE_RIGHT:
            joyOnKeybState &= ~0x01;
            break;

        case HW_KEYCODE_RCTRL: // fallthrough
        case HW_KEYCODE_RALT:
            joyOnKeybState &= ~0x10;
            break;

        default:
            break;
    }

    return false;
}

bool C_KempstonStick::OnJoyDown(HardwareEvent& event) {
    switch (event.joyButton) {
        case HW_JOYSTICK_UP:
            joyState |= 0x08;
            break;

        case HW_JOYSTICK_DOWN:
            joyState |= 0x04;
            break;

        case HW_JOYSTICK_LEFT:
            joyState |= 0x02;
            break;

        case HW_JOYSTICK_RIGHT:
            joyState |= 0x01;
            break;

        case HW_JOYSTICK_FIRE:
            joyState |= 0x10;
            break;
    }

    return true;
}

bool C_KempstonStick::OnJoyUp(HardwareEvent& event) {
    switch (event.joyButton) {
        case HW_JOYSTICK_UP:
            joyState &= ~0x08;
            break;

        case HW_JOYSTICK_DOWN:
            joyState &= ~0x04;
            break;

        case HW_JOYSTICK_LEFT:
            joyState &= ~0x02;
            break;

        case HW_JOYSTICK_RIGHT:
            joyState &= ~0x01;
            break;

        case HW_JOYSTICK_FIRE:
            joyState &= ~0x10;
            break;
    }

    return true;
}

bool C_KempstonStick::InputByteCheckPort(uint16_t port) {
    return (!(port & 0x20));
}

bool C_KempstonStick::OnInputByte(uint16_t port, uint8_t& retval) {
    retval = (joyOnKeyb ? joyOnKeybState : joyState);
    return true;
}
