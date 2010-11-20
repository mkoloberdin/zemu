#ifndef _GSOUND_H_INCLUDED_
#define _GSOUND_H_INCLUDED_

#include "../../zemu.h"
#include "../device.h"
#include "sound/mixer.h"

class C_GSound : public C_Device
{
	public:

	static C_SndRenderer sndRenderer;
	static bool enabled;

	void Init(void);
	void Close(void);

	static bool InputByteCheckPort(Z80EX_WORD port);
	static bool OutputByteCheckPort(Z80EX_WORD port);
	static bool OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval);
	static bool OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value);
	static void OnFrameStart(void);
	static void OnAfterFrameRender(void);
	static void Reset(void);

	private:

	static void UpdateMaps();
	static void Update(unsigned clk);
	static void UpdateSound();

	static Z80EX_BYTE GsReadByte(Z80EX_CONTEXT_PARAM Z80EX_WORD addr, int m1_state, void *userData);
	static void GsWriteByte(Z80EX_CONTEXT_PARAM Z80EX_WORD addr, Z80EX_BYTE value, void *userData);
	static Z80EX_BYTE GsInputByte(Z80EX_CONTEXT_PARAM Z80EX_WORD port, void *userData);
	static void GsOutputByte(Z80EX_CONTEXT_PARAM Z80EX_WORD port, Z80EX_BYTE value, void *userData);
	static Z80EX_BYTE GsReadIntVec(Z80EX_CONTEXT_PARAM void *userData);

	static Z80EX_BYTE regCommand;
	static Z80EX_BYTE regStatus;
	static Z80EX_BYTE regData;
	static Z80EX_BYTE regOutput;
	static Z80EX_BYTE volume[4];
	static Z80EX_BYTE channel[4];
	static Z80EX_BYTE memPage;

	static Z80EX_CONTEXT *gsCpu;
	static Z80EX_BYTE mem[0x8000*32];

	static uint64_t gsClk;
	static Z80EX_BYTE *readMap[4];
	static Z80EX_BYTE *writeMap[4];
};

#endif
