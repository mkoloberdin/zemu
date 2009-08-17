#ifndef _TSFM_H_INCLUDED_
#define _TSFM_H_INCLUDED_

#include "../../zemu.h"
#include "../device.h"
#include "../../sound/snd_renderer.h"
#include "../../lib_ay/ay_chip.h"
#include "../../lib_ym2203/ym2203_chip.h"

#define TSFM_CHIPS_COUNT 2
#define TSFM_MODE_AY 1
#define TSFM_MODE_TS 2
#define TSFM_MODE_TSFM 3

class C_TsFm : public C_Device
{
	public:

	static C_Ym2203Chip ym2203Chip[TSFM_CHIPS_COUNT];
	static C_AyChip ayChip[TSFM_CHIPS_COUNT];

	void Init(void);
	void Close(void);

	static bool InputByteCheckPort(Z80EX_WORD port);
	static bool OnInputByte(Z80EX_WORD port, Z80EX_BYTE &retval);
	static bool OutputByteCheckPort(Z80EX_WORD port);
	static bool OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value);
	static void OnFrameStart(void);
	static void OnAfterFrameRender(void);
	static void OnReset(void);

	static int mode;
	static int pseudoReg;
	static int selectedReg;
};

#endif
