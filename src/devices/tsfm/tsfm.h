#ifndef _TSFM_H_INCLUDED_
#define _TSFM_H_INCLUDED_

#include "zemu.h"
#include "sound/mixer.h"
#include "lib_ay/ay_chip.h"
#include "lib_ym2203/ym2203_chip.h"
#include "lib_saa1099/saa1099_chip.h"
#include "../device.h"

#define TSFM_CHIPS_COUNT 2

#define TSFM_MODE_AY 1
#define TSFM_MODE_TS 2
#define TSFM_MODE_TSFM 3
#define TSFM_MODE_ZXM 4

class C_TsFm : public C_Device {
public:

    static C_Saa1099Chip saa1099Chip;
    static C_Ym2203Chip ym2203Chip[TSFM_CHIPS_COUNT];
    static C_AyChip ayChip[TSFM_CHIPS_COUNT];

    void Init(void);
    void Close(void);

    static bool InputByteCheckPort(uint16_t port);
    static bool OnInputByte(uint16_t port, uint8_t& retval);
    static bool OutputByteCheckPort(uint16_t port);
    static bool OnOutputByte(uint16_t port, uint8_t value);
    static void OnFrameStart(void);
    static void OnAfterFrameRender(void);
    static void OnReset(void);

    static int mode;
    static int pseudoReg;
    static int selectedReg;
};

#endif
