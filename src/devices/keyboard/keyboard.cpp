// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <string>
#include <stdexcept>
#include <string.h>
#include "zemu_env.h"
#include "tape/tape.h"
#include "keyboard.h"
#include "keys.h"

std::set<int> C_Keyboard::hostKeyPressed;
std::map<int, C_Keyboard::s_HostKey> C_Keyboard::hostKeys;
int C_Keyboard::keyboard[8];

void C_Keyboard::ReadKbdConfig(void) {
    char buf[0x1000];
    s_ZxKeys zxKeys;

    std::string value = host->config()->getString("input", "keymap", "keys.config");
    std::string keysConfigPath = host->storage()->findExtras(value)->string();

    if (keysConfigPath.empty()) {
        throw std::runtime_error(std::string("Couldn't find \"" + value + "\""));
    }

    int lineNum = 0;
    const char* fileName = keysConfigPath.c_str();

    printf("Trying to load keys config from \"%s\" ...\n", fileName);
    auto reader = host->storage()->path(keysConfigPath)->dataReader();

    while (!reader->isEof()) {
        strcpy(buf, reader->readLine().c_str());
        lineNum++;

        char* s = buf;

        while (*s && ((*s) == ' ' || (*s) == 9)) {
            s++;
        }

        if (!(*s) || (*s) == '#') {
            continue;
        }

        char* p = s;

        while (*s && (*s) != ' ' && (*s) != 9) {
            s++;
        }

        if (!(*s)) {
            StrikeError("Bad %s file at line %d", fileName, lineNum);
        }

        *s = 0;
        s++;

        int keySym = 0;
        int keySymAdd = 0;

        for (int i = 0; cfgHostKeys[i].cfgname[0]; i++) {
            if (!strcasecmp(p, cfgHostKeys[i].cfgname)) {
                keySym = cfgHostKeys[i].keysym;
                keySymAdd = cfgHostKeys[i].keysymadd;
                break;
            }
        }

        if (!keySym) {
            StrikeError("Host key \"%s\" not found at line %d of %s", p, lineNum, fileName);
        }

        int keyMod = 0;
        int keyModAdd = 0;

        while (*s && ((*s) == ' ' || (*s) == 9)) {
            s++;
        }

        if (!(*s)) {
            StrikeError("ZX keys not found at line %d of %s", lineNum, fileName);
        }

        if ((*s) != ':') {
            keyMod = keySym;
            keyModAdd = keySymAdd;
            p = s;

            while (*s && (*s) != ' ' && (*s) != 9) {
                s++;
            }

            if (!(*s)) {
                StrikeError("Bad %s file at line %d", fileName, lineNum);
            }

            keySym = 0;
            keySymAdd = 0;
            *s = 0;
            s++;

            for (int i = 0; cfgHostKeys[i].cfgname[0]; i++) {
                if (!strcasecmp(p, cfgHostKeys[i].cfgname)) {
                    keySym = cfgHostKeys[i].keysym;
                    keySymAdd = cfgHostKeys[i].keysymadd;
                    break;
                }
            }

            if (!keySym) {
                StrikeError("Host key \"%s\" not found at line %d of %s", p, lineNum, fileName);
            }

            while (*s && ((*s) == ' ' || (*s) == 9)) {
                s++;
            }

            if (!(*s)) {
                StrikeError("ZX keys not found at line %d of %s", lineNum, fileName);
            }

            if ((*s) != ':') {
                StrikeError("Bad %s file at line %d", fileName, lineNum);
            }
        }

        bool canBeAction = true;
        void (* action)(void) = nullptr;
        zxKeys.count = 0;
        s++;

        while (*s) {
            while (*s && ((*s) == ' ' || (*s) == 9)) {
                s++;
            }

            if (!(*s) || (*s) == '#') {
                break;
            }

            if ((*s) == '@') {
                if (!canBeAction) {
                    StrikeError("Mixing ZX keys and actions are not allowed (line %d of %s)", lineNum, fileName);
                }

                s++;
                p = s;

                while (*s && (*s) != ' ' && (*s) != 9) {
                    s++;
                }

                if (*s) {
                    *(s++) = 0;
                }

                for (int i = 0; cfgActions[i].name[0]; i++) {
                    if (!strcasecmp(p, cfgActions[i].name)) {
                        action = cfgActions[i].action;
                        break;
                    }
                }

                if (action == nullptr) {
                    StrikeError("Action %s not defined (line %d of %s)", p, lineNum, fileName);
                }

                break;
            }

            if (zxKeys.count >= MAX_ZX_KEYS_MAP) {
                StrikeError("Maximum %d ZX keys per line allowed (line %d of %s)", MAX_ZX_KEYS_MAP, lineNum, fileName);
            }

            canBeAction = false;
            p = s;

            while (*s && (*s) != ' ' && (*s) != 9) {
                s++;
            }

            if (*s) {
                *(s++) = 0;
            }

            s_CfgZxKey* cfgZxKey = nullptr;

            for (int i = 0; cfgZxKeys[i].cfgname[0]; i++) {
                if (!strcasecmp(p, cfgZxKeys[i].cfgname)) {
                    cfgZxKey = &cfgZxKeys[i];
                    break;
                }
            }

            if (!cfgZxKey || !cfgZxKey->cfgname[0]) {
                StrikeError("ZX key \"%s\" not found at line %d of %s", p, lineNum, fileName);
            }

            zxKeys.portnum[zxKeys.count] = cfgZxKey->portnum;
            zxKeys.bitmask[zxKeys.count] = cfgZxKey->bitmask;
            zxKeys.count++;
        }

        if (!zxKeys.count && !action) {
            StrikeError("No ZX keys or actions defined at line %d of %s", lineNum, fileName);
        }

        s_HostKey* hostKey = &hostKeys[keySym];
        s_HostKey* hostKeyAdd = (keySymAdd ? &hostKeys[keySymAdd] : nullptr);

        if (keyMod) {
            if (hostKey->mods.count >= MAX_HOST_KEY_MODS) {
                StrikeError("Maximum %d overloads for host key allowed (line %d of %s)", MAX_HOST_KEY_MODS, lineNum, fileName);
            }

            hostKey->mods.keyMod[hostKey->mods.count] = keyMod;
            hostKey->mods.actions[hostKey->mods.count] = action;
            hostKey->mods.zxKeys[hostKey->mods.count] = zxKeys;
            hostKey->mods.count++;

            if (hostKeyAdd) {
                if (hostKeyAdd->mods.count >= MAX_HOST_KEY_MODS) {
                    StrikeError("Maximum %d overloads for host key allowed (line %d of %s)", MAX_HOST_KEY_MODS, lineNum, fileName);
                }

                hostKeyAdd->mods.keyMod[hostKeyAdd->mods.count] = keyMod;
                hostKeyAdd->mods.actions[hostKeyAdd->mods.count] = action;
                hostKeyAdd->mods.zxKeys[hostKeyAdd->mods.count] = zxKeys;
                hostKeyAdd->mods.count++;
            }

            if (keyModAdd) {
                if (hostKey->mods.count >= MAX_HOST_KEY_MODS) {
                    StrikeError("Maximum %d overloads for host key allowed (line %d of %s)", MAX_HOST_KEY_MODS, lineNum, fileName);
                }

                hostKey->mods.keyMod[hostKey->mods.count] = keyModAdd;
                hostKey->mods.actions[hostKey->mods.count] = action;
                hostKey->mods.zxKeys[hostKey->mods.count] = zxKeys;
                hostKey->mods.count++;

                if (hostKeyAdd) {
                    if (hostKeys[keySymAdd].mods.count >= MAX_HOST_KEY_MODS) {
                        StrikeError("Maximum %d overloads for host key allowed (line %d of %s)", MAX_HOST_KEY_MODS, lineNum, fileName);
                    }

                    hostKeyAdd->mods.keyMod[hostKeyAdd->mods.count] = keyModAdd;
                    hostKeyAdd->mods.actions[hostKeyAdd->mods.count] = action;
                    hostKeyAdd->mods.zxKeys[hostKeyAdd->mods.count] = zxKeys;
                    hostKeyAdd->mods.count++;
                }
            }
        } else {
            hostKey->normal = zxKeys;
            hostKey->action = action;

            if (hostKeyAdd) {
                hostKeyAdd->normal = zxKeys;
            }
        }
    }

    printf("Config loaded successfully\n");
}

void C_Keyboard::Init(void) {
    ReadKbdConfig();

    AttachHwHandler(STAGE_EVENT_KEYDOWN, OnKeyDown);
    AttachHwHandler(STAGE_EVENT_KEYUP, OnKeyUp);

    AttachZ80InputHandler(InputByteCheckPort, OnInputByte);

    for (int i = 0; i < 8; i++) {
        keyboard[i] = 255;
    }
}

void C_Keyboard::Close(void) {
}

void C_Keyboard::ResetPressedState(void) {
    hostKeyPressed.clear();

    for (int i = 0; i < 8; i++) {
        keyboard[i] = 255;
    }
}

bool C_Keyboard::OnKeyDown(StageEvent& event) {
    int key = event.keyCode;

    if (joyOnKeyb && (key == STAGE_KEYCODE_UP
        || key == STAGE_KEYCODE_DOWN
        || key == STAGE_KEYCODE_LEFT
        || key == STAGE_KEYCODE_RIGHT
        || key == STAGE_KEYCODE_RCTRL
        || key == STAGE_KEYCODE_RALT
    )) {
        return false;
    }

    auto keyIt = hostKeys.find(key);

    if (keyIt == hostKeys.end()) {
        return false;
    }

    hostKeyPressed.insert(key);
    s_HostKey* hostKey = &keyIt->second;

    for (int k = 0; k < hostKey->mods.count; k++) {
        int keyMod = hostKey->mods.keyMod[k];

        if (hostKeyPressed.find(keyMod) == hostKeyPressed.end()) {
            continue;
        }

        if (hostKey->mods.actions[k] && keyMod == STAGE_KEYCODE_NUMLOCK) {
            hostKey->mods.actions[k]();
            return false;
        }

        s_HostKey* hostKeyMod = &hostKeys[keyMod];
        s_ZxKeys* zxKeys = &hostKey->mods.zxKeys[k];

        for (int i = 0; i < hostKeyMod->normal.count; i++) {
            keyboard[hostKeyMod->normal.portnum[i]] |= hostKeyMod->normal.bitmask[i];
        }

        for (int i = 0; i < zxKeys->count; i++) {
            keyboard[zxKeys->portnum[i]] &= ~zxKeys->bitmask[i];
        }

        return false;
    }

    if (hostKey->action && key == STAGE_KEYCODE_NUMLOCK) {
        hostKey->action();
        return false;
    }

    if (hostKey->normal.count) {
        for (int i = 0; i < hostKey->normal.count; i++) {
            keyboard[hostKey->normal.portnum[i]] &= ~hostKey->normal.bitmask[i];
        }
    }

    return false;
}

bool C_Keyboard::OnKeyUp(StageEvent& event) {
    int key = event.keyCode;

    if (joyOnKeyb && (key == STAGE_KEYCODE_UP
        || key == STAGE_KEYCODE_DOWN
        || key == STAGE_KEYCODE_LEFT
        || key == STAGE_KEYCODE_RIGHT
        || key == STAGE_KEYCODE_RCTRL
        || key == STAGE_KEYCODE_RALT
    )) {
        return false;
    }

    auto keyIt = hostKeys.find(key);

    if (keyIt == hostKeys.end()) {
        return false;
    }

    hostKeyPressed.erase(key);
    s_HostKey* hostKey = &keyIt->second;

    for (int k = 0; k < hostKey->mods.count; k++)
    {
        int keyMod = hostKey->mods.keyMod[k];
        bool isModPressed = (hostKeyPressed.find(keyMod) != hostKeyPressed.end());

        if (isModPressed && hostKey->mods.actions[k]) {
            hostKey->mods.actions[k]();
            return false;
        }

        s_ZxKeys* zxKeys = &hostKey->mods.zxKeys[k];

        for (int i = 0; i < zxKeys->count; i++) {
            keyboard[zxKeys->portnum[i]] |= zxKeys->bitmask[i];
        }

        if (isModPressed) {
            s_HostKey* hostKeyMod = &hostKeys[keyMod];

            for (int i = 0; i < hostKeyMod->normal.count; i++) {
                keyboard[hostKeyMod->normal.portnum[i]] &= ~hostKeyMod->normal.bitmask[i];
            }
        }
    }

    if (hostKey->mods.count) {
        for (auto it = hostKeys.begin(); it != hostKeys.end(); ++it) {
            if (hostKeyPressed.find(it->first) != hostKeyPressed.end() && it->second.normal.count) {
                for (int i = 0; i < it->second.normal.count; i++) {
                    keyboard[it->second.normal.portnum[i]] &= ~it->second.normal.bitmask[i];
                }
            }
        }
    }

    if (hostKey->action) {
        hostKey->action();
        return false;
    }

    if (hostKey->normal.count) {
        for (int i = 0; i < hostKey->normal.count; i++) {
            keyboard[hostKey->normal.portnum[i]] |= hostKey->normal.bitmask[i];
        }
    }

    return false;
}

bool C_Keyboard::InputByteCheckPort(uint16_t port) {
    return (!(port & 1));
}

bool C_Keyboard::OnInputByte(uint16_t port, uint8_t& retval) {
    retval = 255;
    int hport = (port >> 8);

    for (int i = 0; i < 8; i++) {
        if (!(hport & 1)) {
            retval &= keyboard[i];
        }

        hport >>= 1;
    }

    if (!C_Tape::GetCurrBit()) {
        retval &= ~0x40;
    }

    return true;
}
