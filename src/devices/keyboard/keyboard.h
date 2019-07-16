#ifndef _KEYBOARD_H_INCLUDED_
#define _KEYBOARD_H_INCLUDED_

#include "zemu.h"
#include "../device.h"
#include <set>
#include <map>

#define MAX_HOST_KEY_MODS 5
#define MAX_ZX_KEYS_MAP 5

class C_Keyboard : public C_Device {
public:

    struct s_ZxKeys {
        s_ZxKeys() : count(0) {} // -V730

        int count;
        int portnum[MAX_ZX_KEYS_MAP];
        int bitmask[MAX_ZX_KEYS_MAP];
    };

    struct s_HostKeyMods {
        s_HostKeyMods() : count(0) {} // -V730

        int count;
        int keyMod[MAX_HOST_KEY_MODS];
        void (* actions[MAX_HOST_KEY_MODS])(void);
        s_ZxKeys zxKeys[MAX_HOST_KEY_MODS];
    };

    struct s_HostKey {
        s_HostKey() : normal(), action(nullptr), mods() {}

        s_ZxKeys normal;
        void (* action)(void);
        s_HostKeyMods mods;
    };

    static std::set<ZHW_Keyboard_KeyCode> hostKeyPressed;
    static std::map<ZHW_Keyboard_KeyCode, s_HostKey> hostKeys;
    static int keyboard[8];

    static void ReadKbdConfig(void);
    void Init(void);
    void Close(void);

    static bool OnKeyDown(ZHW_Event& event);
    static bool OnKeyUp(ZHW_Event& event);

    static bool InputByteCheckPort(uint16_t port);
    static bool OnInputByte(uint16_t port, uint8_t& retval);
};

#endif
