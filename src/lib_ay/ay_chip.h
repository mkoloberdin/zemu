#ifndef _AY_CHIP_H_INCLUDED_
#define _AY_CHIP_H_INCLUDED_

#include "../zemu.h"
#include "sound/mixer.h"

const unsigned SNDR_DEFAULT_AY_RATE = 1774400; // original ZX-Spectrum soundchip clock fq

#define AYREG_fA        (0)     // word
#define AYREG_fB        (2)     // word
#define AYREG_fC        (4)     // word
#define AYREG_noise     (6)     // byte
#define AYREG_mix       (7)     // byte
#define AYREG_vA        (8)     // byte
#define AYREG_vB        (9)     // byte
#define AYREG_vC        (10)    // byte
#define AYREG_envT      (11)    // word
#define AYREG_env       (13)    // byte
#define AYREG_portA     (14)    // byte
#define AYREG_portB     (15)    // byte

class C_AyChipConfig {
    public:

    enum ChipType {
        TypeAy = 0,
        TypeYm = 1
    };

    enum VolType {
        VolAy = 0,
        VolYm = 1
    };

    enum PanType {
        PanMono = 0,
        PanABC = 1,
        PanACB = 2,
        PanBAC = 3,
        PanBCA = 4,
        PanCAB = 5,
        PanCBA = 6
    };

    C_AyChipConfig();

    void ReadConfig(void);

    const unsigned* GetVolTab(void);
    const unsigned* GetPanTab(void);

    ChipType chipType;
    VolType volType;
    PanType panType;
};

class C_AyChip {
    public:

    C_SndRenderer sndRenderer;

    C_AyChip();
    virtual ~C_AyChip();

    void Init(void);

    void StartFrame(void);
    void EndFrame(unsigned devClk);

    void Select(uint8_t reg);
    void Write(unsigned devClk, uint8_t val);
    uint8_t Read(void);

    void Reset(unsigned devClk = 0); // call with default parameter, when context outside StartFrame/EndFrame block

    protected:

    void Flush(unsigned tick);
    void ApplyRegs(unsigned devClk = 0);
    void SetVolumes(unsigned globalVol, const unsigned* volTab, const unsigned* panTab);
    void SetTimings(unsigned systemClockRate, unsigned chipClockRate, unsigned sampleRate);

    C_AyChipConfig conf;

    unsigned t;
    unsigned ta;
    unsigned tb;
    unsigned tc;
    unsigned tn;
    unsigned te;
    unsigned env;
    unsigned denv;

    unsigned bitA;
    unsigned bitB;
    unsigned bitC;
    unsigned bitN;
    unsigned ns;

    unsigned bit0;
    unsigned bit1;
    unsigned bit2;
    unsigned bit3;
    unsigned bit4;
    unsigned bit5;

    unsigned ea;
    unsigned eb;
    unsigned ec;
    unsigned va;
    unsigned vb;
    unsigned vc;

    unsigned fa;
    unsigned fb;
    unsigned fc;
    unsigned fn;
    unsigned fe;

    unsigned multConst;
    unsigned vols[6][32];

    uint8_t r13Reloaded;
    uint8_t selectedReg;
    uint8_t regs[0x10];

    unsigned chipClockRate;
    unsigned systemClockRate;

    uint64_t passedChipTicks;
    uint64_t passedClkTicks;
};

#endif
