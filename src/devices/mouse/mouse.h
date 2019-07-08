#ifndef _MOUSE_H_INCLUDED_
#define _MOUSE_H_INCLUDED_

#include "../../zemu.h"
#include "../device.h"

class C_Mouse : public C_Device
{
	public:

	static Z80EX_BYTE portFBDF;
	static Z80EX_BYTE portFFDF;
	static Z80EX_BYTE portFADF;
	static Z80EX_BYTE wheelCnt;

	void Init(void);
	void Close(void);

	static void UpdateState(void);
	static bool InputByteCheckPort(Z80EX_WORD port);
	static bool OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval);
	static bool OnHwMouseButtonDown(ZHW_Event &ev);
};

#endif
