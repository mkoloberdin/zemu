// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Joystick events now refer to an SDL_JoystickID.
// This is because SDL 2.0 can handle joysticks coming and going, as devices are plugged in and pulled out during your game's lifetime,
// so the index into the device list that 1.2 uses would be meaningless as the available device list changes.
//
// See https://wiki.libsdl.org/MigrationGuide !!!

#include "joystick_manager.h"
#include "exceptions.h"
#include "devs.h"

C_JoystickManager* C_JoystickManager::_instance = 0;

C_JoystickManager* C_JoystickManager::Instance() {
    if (!_instance) {
        _instance = new C_JoystickManager;
    }

    return _instance;
}

void C_JoystickManager::Init() {
    auto config = hostEnv->config();
    params.kempstonEnable = config->getBool("kempstonjoystick", "enable", false);

    if (!params.kempstonEnable) {
        return;
    }

    params.kempstonOnStickNum = config->getInt("kempstonjoystick", "sysjoysticknum", -1);
    params.kempstonAxisTreshold = config->getInt("kempstonjoystick", "axisthreshold", DEF_JOY_AXIS_TRESHOLD);

    // если номер джойстика < 0, то кемпстоном можно будет управлять только с клавиатуры
    if (params.kempstonOnStickNum < 0) {
        return;
    }

    if (AddJoystick(params.kempstonOnStickNum, params.kempstonAxisTreshold)) {
        dev_kempston.SetJoyNum(params.kempstonOnStickNum);
        EnableProcessing();
    } else {
        StrikeMessage("Error initializing kempston joystick");
    }
}

C_JoystickManager::C_JoystickManager() {
    if (ZHW_Joystick_Init() < 0) {
        throw C_E(E_General, "cannot initialize joystick subsystem");
    }

    memset(jstate, 0, sizeof(JoystickState) * MAX_JOYSTICKS);
    num_joysticks = ZHW_Joystick_GetNum();

    if (num_joysticks > MAX_JOYSTICKS) {
        num_joysticks = MAX_JOYSTICKS;
    }

    is_enabled = false;

    AttachHwHandler(ZHW_EVENT_JOYAXISMOTION, OnJoystickEvent);
    AttachHwHandler(ZHW_EVENT_JOYBUTTONDOWN, OnJoystickEvent);
    AttachHwHandler(ZHW_EVENT_JOYBUTTONUP, OnJoystickEvent);
}

bool C_JoystickManager::AddJoystick(int joy_num, int axis_treshold) {
    ZHW_Joystick* joy;

    if (joy_num >= num_joysticks || (joy = ZHW_Joystick_Open(joy_num)) == nullptr) {
        return false;
    }

    jstate[joy_num].init_ok = true;
    jstate[joy_num].axis_treshold = axis_treshold;

    return true;
}

bool C_JoystickManager::OnJoystickEvent(ZHW_Event& event) {
    return C_JoystickManager::Instance()->ProcessJoystickEvent(event);
}

bool C_JoystickManager::ProcessJoystickEvent(ZHW_Event& event) {
    int axis_state;
    int joy_num;
    bool ev_processed = false;

    if (!is_enabled) {
        return false;
    }

    switch (event.type) {
        case ZHW_EVENT_JOYAXISMOTION: {
            joy_num = event.jaxis.which;

            if (joy_num >= num_joysticks || !jstate[joy_num].init_ok) {
                break;
            }

            axis_state = 0;

            if (event.jaxis.value < -jstate[joy_num].axis_treshold) {
                axis_state = -1;
            } else if (event.jaxis.value > jstate[joy_num].axis_treshold) {
                axis_state = 1;
            }

            switch (event.jaxis.axis) {
                case 0: // axis 0: left-right
                    if (axis_state > 0) {
                        jstate[joy_num].right = true;
                        jstate[joy_num].left = false;
                    } else if (axis_state < 0) {
                        jstate[joy_num].left = true;
                        jstate[joy_num].right = false;
                    } else {
                        jstate[joy_num].left = false;
                        jstate[joy_num].right = false;
                    }

                    ev_processed = true;
                    break;

                case 1: // axis 1: up-down
                    if (axis_state > 0) {
                        jstate[joy_num].down = true;
                        jstate[joy_num].up = false;
                    } else if (axis_state < 0) {
                        jstate[joy_num].up = true;
                        jstate[joy_num].down = false;
                    } else {
                        jstate[joy_num].up = false;
                        jstate[joy_num].down = false;
                    }

                    ev_processed = true;
                    break;
            }

            break;
        }

        case ZHW_EVENT_JOYBUTTONDOWN: {
            joy_num = event.jbutton.which;
            jstate[joy_num].fire = true;
            ev_processed = true;
            break;
        }

        case ZHW_EVENT_JOYBUTTONUP: {
            joy_num = event.jbutton.which;
            jstate[joy_num].fire = false;
            ev_processed = true;
            break;
        }
    }

    return ev_processed;
}

JoystickState* C_JoystickManager::GetJoystickState(int joy_num) {
    if (!is_enabled || joy_num >= num_joysticks || !jstate[joy_num].init_ok) {
        throw C_E(E_General, "cannot read state for this joystick");
    }

    return &jstate[joy_num];
}

bool C_JoystickManager::EnableProcessing() {
    if (!num_joysticks) {
        return false;
    }

    return (is_enabled = true);
}

bool C_JoystickManager::DisableProcessing() {
    if (!is_enabled) {
        return false;
    }

    return (is_enabled = false);
}
