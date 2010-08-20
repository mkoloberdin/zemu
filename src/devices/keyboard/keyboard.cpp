#include <string.h>

#include "keys.h"
#include "keyboard.h"
#include "../../file.h"
#include "../../exceptions.h"
#include "../../tape/tape.h"

bool C_Keyboard::hostKeyPressed[SDLK_LAST];
C_Keyboard::s_HostKey C_Keyboard::hostKeys[SDLK_LAST];
int C_Keyboard::keyboard[8];

void C_Keyboard::ReadKbdConfig(void)
{
	int i, hkey, ln, kmod, hkeyadd, kmodadd=0;
	C_File fl;
	s_HostKey hk;
	s_ZxKeys zxk;
	char buf[0x100], *s, *p;
	const char *fname;
	void (* act)(void);
	bool canAct;

	hk.normal.count = 0;
	hk.mods.count = 0;
	hk.action = NULL;

	for (i = 0; i < SDLK_LAST; i++)
	{
		hostKeys[i] = hk;
		hostKeyPressed[i] = false;
	}

	string value = config.GetString("input", "keymap", "keys.config");
	value = config.FindDataFile("", value.c_str());
	fname = value.c_str();

	ln = 0;
	fl.Read(fname);
	while (!fl.Eof())
	{
		fl.GetS(buf, sizeof(buf));
		ln++;
		s = buf;

		while (*s && ((*s)==' ' || (*s)==9)) s++;
		if (!(*s)) continue;
		if ((*s) == '#') continue;

		p = s;
		while (*s && (*s)!=' ' && (*s)!=9) s++;
		if (!(*s)) StrikeError("Bad %s file at line %d", fname, ln);

		*s = 0;
		s++;

		for (i=0, hkey=0, hkeyadd=0; cfgHostKeys[i].cfgname[0]; i++)
		{
			if (!stricmp(p, cfgHostKeys[i].cfgname))
			{
				hkey = cfgHostKeys[i].keysym;
				hkeyadd = cfgHostKeys[i].keysymadd;
				break;
			}
		}

		if (!hkey) StrikeError("Host key \"%s\" not found at line %d of %s", p, ln, fname);
		kmod = 0;

		while (*s && ((*s)==' ' || (*s)==9)) s++;
		if (!(*s)) StrikeError("ZX keys not found at line %d of %s", ln, fname);

		if ((*s) != ':')
		{
			kmod = hkey;
			kmodadd = hkeyadd;

			p = s;
			while (*s && (*s)!=' ' && (*s)!=9) s++;
			if (!(*s)) StrikeError("Bad %s file at line %d", fname, ln);

			*s = 0;
			s++;

			for (i=0, hkey=0, hkeyadd=0; cfgHostKeys[i].cfgname[0]; i++)
			{
				if (!stricmp(p, cfgHostKeys[i].cfgname))
				{
					hkey = cfgHostKeys[i].keysym;
					hkeyadd = cfgHostKeys[i].keysymadd;
					break;
				}
			}

			if (!hkey) StrikeError("Host key \"%s\" not found at line %d of %s", p, ln, fname);

			while (*s && ((*s)==' ' || (*s)==9)) s++;
			if (!(*s)) StrikeError("ZX keys not found at line %d of %s", ln, fname);

			if ((*s) != ':') StrikeError("Bad %s file at line %d", fname, ln);
		}

		canAct = true;
		act = NULL;
		zxk.count = 0;
		s++;

		while (*s)
		{
			while (*s && ((*s)==' ' || (*s)==9)) s++;
			if (!(*s)) break;
			if ((*s) == '#') break;

			if ((*s) == '@')
			{
				if (canAct)
				{
					s++;
					p = s;
					while (*s && (*s)!=' ' && (*s)!=9) s++;
					if (*s) *(s++) = 0;

					for (i=0; cfgActions[i].name[0]; i++) {
						if (!stricmp(p, cfgActions[i].name)) {
							act = cfgActions[i].action;
							break;
						}
					}

					if (act == NULL) StrikeError("Action %s not defined (line %d of %s)", p, ln, fname);
					break;
				}
				else StrikeError("Mixing ZX keys and actions are not allowed (line %d of %s)", ln, fname);
			}

			if (zxk.count >= MAX_ZX_KEYS_MAP) StrikeError("Maximum %d ZX keys per line allowed (line %d of %s)", MAX_ZX_KEYS_MAP, ln, fname);
			canAct = false;

			p = s;
			while (*s && (*s)!=' ' && (*s)!=9) s++;
			if (*s) *(s++) = 0;

			for (i=0; cfgZxKeys[i].cfgname[0]; i++) {
				if (!stricmp(p, cfgZxKeys[i].cfgname)) break;
			}

			if (!cfgZxKeys[i].cfgname[0]) StrikeError("ZX key \"%s\" not found at line %d of %s", p, ln, fname);

			zxk.portnum[zxk.count] = cfgZxKeys[i].portnum;
			zxk.bitmask[zxk.count] = cfgZxKeys[i].bitmask;
			zxk.count++;
		}

		if (!zxk.count && !act) StrikeError("No ZX keys or actions defined at line %d of %s", ln, fname);

		if (kmod)
		{
			if (hostKeys[hkey].mods.count >= MAX_HOST_KEY_MODS) StrikeError("Maximum %d overloads for host key allowed (line %d of %s)", MAX_HOST_KEY_MODS, ln, fname);
			hostKeys[hkey].mods.keyMod[hostKeys[hkey].mods.count] = kmod;
			hostKeys[hkey].mods.actions[hostKeys[hkey].mods.count] = act;
			hostKeys[hkey].mods.zxKeys[hostKeys[hkey].mods.count] = zxk;
			hostKeys[hkey].mods.count++;

			if (hkeyadd)
			{
				if (hostKeys[hkeyadd].mods.count >= MAX_HOST_KEY_MODS) StrikeError("Maximum %d overloads for host key allowed (line %d of %s)", MAX_HOST_KEY_MODS, ln, fname);
				hostKeys[hkeyadd].mods.keyMod[hostKeys[hkeyadd].mods.count] = kmod;
				hostKeys[hkeyadd].mods.actions[hostKeys[hkeyadd].mods.count] = act;
				hostKeys[hkeyadd].mods.zxKeys[hostKeys[hkeyadd].mods.count] = zxk;
				hostKeys[hkeyadd].mods.count++;
			}

			if (kmodadd)
			{
				if (hostKeys[hkey].mods.count >= MAX_HOST_KEY_MODS) StrikeError("Maximum %d overloads for host key allowed (line %d of %s)", MAX_HOST_KEY_MODS, ln, fname);
				hostKeys[hkey].mods.keyMod[hostKeys[hkey].mods.count] = kmodadd;
				hostKeys[hkey].mods.actions[hostKeys[hkey].mods.count] = act;
				hostKeys[hkey].mods.zxKeys[hostKeys[hkey].mods.count] = zxk;
				hostKeys[hkey].mods.count++;

				if (hkeyadd)
				{
					if (hostKeys[hkeyadd].mods.count >= MAX_HOST_KEY_MODS) StrikeError("Maximum %d overloads for host key allowed (line %d of %s)", MAX_HOST_KEY_MODS, ln, fname);
					hostKeys[hkeyadd].mods.keyMod[hostKeys[hkeyadd].mods.count] = kmodadd;
					hostKeys[hkeyadd].mods.actions[hostKeys[hkeyadd].mods.count] = act;
					hostKeys[hkeyadd].mods.zxKeys[hostKeys[hkeyadd].mods.count] = zxk;
					hostKeys[hkeyadd].mods.count++;
				}
			}
		}
		else
		{
			hostKeys[hkey].normal = zxk;
			hostKeys[hkey].action = act;
			if (hkeyadd) hostKeys[hkeyadd].normal = zxk;
		}
	}
}

void C_Keyboard::Init(void)
{
	ReadKbdConfig();

	AttachSDLHandler(SDL_KEYDOWN, OnKeyDown);
	AttachSDLHandler(SDL_KEYUP, OnKeyUp);

	AttachZ80InputHandler(InputByteCheckPort, OnInputByte);

	for (int i = 0; i < 8; i++) keyboard[i] = 255;
}

void C_Keyboard::Close(void)
{
}

bool C_Keyboard::OnKeyDown(SDL_Event &event)
{
	int k, key;

	key = event.key.keysym.sym;

	if (joyOnKeyb) {
		if (key==SDLK_UP || key==SDLK_DOWN || key==SDLK_LEFT || key==SDLK_RIGHT || key==SDLK_RCTRL) {
			return false;
		}
	}

	hostKeyPressed[key] = true;

	for (k = 0; k < hostKeys[key].mods.count; k++)
	{
		if (hostKeyPressed[hostKeys[key].mods.keyMod[k]])
		{
			if (hostKeys[key].mods.actions[k] && hostKeys[key].mods.keyMod[k]==SDLK_NUMLOCK) {
				hostKeys[key].mods.actions[k]();
				return false;
			}

			for (int i = 0; i < hostKeys[hostKeys[key].mods.keyMod[k]].normal.count; i++) {
				keyboard[hostKeys[hostKeys[key].mods.keyMod[k]].normal.portnum[i]] |= hostKeys[hostKeys[key].mods.keyMod[k]].normal.bitmask[i];
			}

			for (int i = 0; i < hostKeys[key].mods.zxKeys[k].count; i++) {
				keyboard[hostKeys[key].mods.zxKeys[k].portnum[i]] &= ~hostKeys[key].mods.zxKeys[k].bitmask[i];
			}

			return false;
		}
	}

	if (hostKeys[key].action && key==SDLK_NUMLOCK) {
		hostKeys[key].action();
		return false;
	}

	if (hostKeys[key].normal.count) {
		for (int i = 0; i < hostKeys[key].normal.count; i++) {
			keyboard[hostKeys[key].normal.portnum[i]] &= ~hostKeys[key].normal.bitmask[i];
		}
	}

	return false;
}

bool C_Keyboard::OnKeyUp(SDL_Event &event)
{
	int k, i, key;

	key = event.key.keysym.sym;

	if (joyOnKeyb) {
		if (key==SDLK_UP || key==SDLK_DOWN || key==SDLK_LEFT || key==SDLK_RIGHT || key==SDLK_RCTRL) {
			return false;
		}
	}

	hostKeyPressed[key] = false;

	for (k = 0; k < hostKeys[key].mods.count; k++)
	{
		if (hostKeyPressed[hostKeys[key].mods.keyMod[k]] && hostKeys[key].mods.actions[k]) {
			hostKeys[key].mods.actions[k]();
			return false;
		}

		for (int i = 0; i < hostKeys[key].mods.zxKeys[k].count; i++) {
			keyboard[hostKeys[key].mods.zxKeys[k].portnum[i]] |= hostKeys[key].mods.zxKeys[k].bitmask[i];
		}

		if (hostKeyPressed[hostKeys[key].mods.keyMod[k]]) {
			for (int i = 0; i < hostKeys[hostKeys[key].mods.keyMod[k]].normal.count; i++) {
				keyboard[hostKeys[hostKeys[key].mods.keyMod[k]].normal.portnum[i]] &= ~hostKeys[hostKeys[key].mods.keyMod[k]].normal.bitmask[i];
			}
		}
	}

	if (hostKeys[key].mods.count) {
		for (k = 0; k < SDLK_LAST; k++) {
			if (hostKeyPressed[k] && hostKeys[k].normal.count) {
				for (i = 0; i < hostKeys[k].normal.count; i++) {
					keyboard[hostKeys[k].normal.portnum[i]] &= ~hostKeys[k].normal.bitmask[i];
				}
			}
		}
	}

	if (hostKeys[key].action) {
		hostKeys[key].action();
		return false;
	}

	if (hostKeys[key].normal.count) {
		for (int i = 0; i < hostKeys[key].normal.count; i++) {
			keyboard[hostKeys[key].normal.portnum[i]] |= hostKeys[key].normal.bitmask[i];
		}
	}

	return false;
}

bool C_Keyboard::InputByteCheckPort(Z80EX_WORD port)
{
	return (!(port & 1));
}

bool C_Keyboard::OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval)
{
	retval = 255;
	int hport = (port >> 8);

	for (int i = 0; i < 8; i++) {
		if (!(hport & 1)) retval &= keyboard[i];
		hport >>= 1;
	}

	if (!C_Tape::GetCurrBit()) retval &= ~0x40;

	return true;
}
