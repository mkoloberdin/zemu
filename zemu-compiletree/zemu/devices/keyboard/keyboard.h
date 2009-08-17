#ifndef _KEYBOARD_H_INCLUDED_
#define _KEYBOARD_H_INCLUDED_

#include "../../zemu.h"
#include "../device.h"

#define MAX_HOST_KEY_MODS 5
#define MAX_ZX_KEYS_MAP 5

class C_Keyboard : public C_Device
{
	public:

	struct s_ZxKeys
	{
		int count;
		int portnum[MAX_ZX_KEYS_MAP];
		int bitmask[MAX_ZX_KEYS_MAP];
	};

	struct s_HostKeyMods
	{
		int count;
		int keyMod[MAX_HOST_KEY_MODS];
		void (* actions[MAX_HOST_KEY_MODS])(void);
		s_ZxKeys zxKeys[MAX_HOST_KEY_MODS];
	};

	struct s_HostKey
	{
		s_ZxKeys normal;
		void (* action)(void);
		s_HostKeyMods mods;
	};

	static bool hostKeyPressed[SDLK_LAST];
	static s_HostKey hostKeys[SDLK_LAST];
	static int keyboard[8];

	static void ReadKbdConfig(void);
	void Init(void);
	void Close(void);

	static bool OnKeyDown(SDL_Event &event);
	static bool OnKeyUp(SDL_Event &event);

	static bool InputByteCheckPort(Z80EX_WORD port);
	static bool OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval);
};

#endif
