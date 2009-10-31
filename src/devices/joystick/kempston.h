// [boo_boo]

#ifndef _KEMPSTON_H_INCLUDED_
#define _KEMPSTON_H_INCLUDED_ 1

#include "../../zemu.h"
#include "../device.h"

class C_KempstonStick : public C_Device
{
	public:

	static Z80EX_BYTE joy_kbd;

	void Init(void);
	void Close(void);

	static bool OnKeyDown(SDL_Event &event);
	static bool OnKeyUp(SDL_Event &event);

	static bool InputByteCheckPort(Z80EX_WORD port);
	static bool OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval);
	static void SetJoyNum(int num);

	private:

	static int joy_num;
};

#endif
