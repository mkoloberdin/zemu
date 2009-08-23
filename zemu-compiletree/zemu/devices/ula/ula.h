#ifndef _ULA_H_INCLUDED_
#define _ULA_H_INCLUDED_

#include "../../zemu.h"
#include "../device.h"
#include <types.h>

class C_ULA : public C_ULA
{
	public:

	void Init(void);
	void Close(void);

	static bool OutputByteCheckPort(Z80EX_WORD port);
	static bool OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value);
    
    private:
    
    static uint8_t ScreenCache[0x1800]; // cache of zx screen
    static uint8_t AttrCache[0x1800]; // cache of zx attributes for every byte of the screen
    unit8_t BorderColor; // latest value from port #FB
    unsigned long long LastBorderTState; // 
};

#endif
