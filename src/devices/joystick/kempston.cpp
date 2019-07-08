// [boo_boo]

#include "kempston.h"
#include "../../joystick_manager.h"

int C_KempstonStick::joy_num = -1;
Z80EX_BYTE C_KempstonStick::joy_kbd = 0;

void C_KempstonStick::Init(void) {
    AttachHwHandler(ZHW_EVENT_KEYDOWN, OnKeyDown);
    AttachHwHandler(ZHW_EVENT_KEYUP, OnKeyUp);

    AttachZ80InputHandler(InputByteCheckPort, OnInputByte);
}

void C_KempstonStick::Close(void) {
}

void C_KempstonStick::SetJoyNum(int num) {
    joy_num = num;
}

bool C_KempstonStick::OnKeyDown(ZHW_Event &event) {
    if (!joyOnKeyb || !ZHW_EVENT_OKKEY(window, event)) {
        return false;
    }

    switch (event.key.keysym.sym) {
        case ZHW_KEY_UP:
            joy_kbd |= 0x08;
            break;

        case ZHW_KEY_DOWN:
            joy_kbd |= 0x04;
            break;

        case ZHW_KEY_LEFT:
            joy_kbd |= 0x02;
            break;

        case ZHW_KEY_RIGHT:
            joy_kbd |= 0x01;
            break;

        case ZHW_KEY_RCTRL:
        case ZHW_KEY_RALT:
            joy_kbd |= 0x10;
            break;

        default:
            break;
    }

    return false;
}

bool C_KempstonStick::OnKeyUp(ZHW_Event &event) {
    if (!joyOnKeyb || !ZHW_EVENT_OKKEY(window, event)) {
        return false;
    }

    switch (event.key.keysym.sym) {
        case ZHW_KEY_UP:
            joy_kbd &= ~0x08;
            break;

        case ZHW_KEY_DOWN:
            joy_kbd &= ~0x04;
            break;

        case ZHW_KEY_LEFT:
            joy_kbd &= ~0x02;
            break;

        case ZHW_KEY_RIGHT:
            joy_kbd &= ~0x01;
            break;

        case ZHW_KEY_RCTRL:
        case ZHW_KEY_RALT:
            joy_kbd &= ~0x10;
            break;

        default:
            break;
    }

    return false;
}

bool C_KempstonStick::InputByteCheckPort(Z80EX_WORD port) {
    return (!(port & 0x20));
}

bool C_KempstonStick::OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval) {
    retval = 0;

    if (joyOnKeyb) {
        retval = joy_kbd;
    } else if (joy_num >= 0) {
        JoystickState *jstate = C_JoystickManager::Instance()->GetJoystickState(joy_num);

        if (jstate->right) {
            retval |= 0x01;
        }

        if (jstate->left) {
            retval |= 0x02;
        }

        if (jstate->down) {
            retval |= 0x04;
        }

        if (jstate->up) {
            retval |= 0x08;
        }

        if (jstate->fire) {
            retval |= 0x10;
        }
    }

    return true;
}
