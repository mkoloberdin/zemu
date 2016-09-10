#include "ay_chip.h"

/*
 * (c) zame software development
 * AY-8910 and YM-2149 chip emulation
 *
 * YM-2149F emulator for Unreal Speccy project
 * created under public domain license by SMT, jan.2006
 */

const unsigned MULT_C_1 = 14; // fixed point precision for 'system tick -> ay tick'

C_AyChipConfig::C_AyChipConfig()
{
  chipType = TypeYm;
  volType = VolYm;
  panType = PanACB;
}

void C_AyChipConfig::ReadConfig(void)
{
  const char *str;

  str = env.GetString("sound", "aychiptype", "ym").c_str();
  chipType = (!strcasecmp(str, "ay") ? TypeAy : TypeYm);

  str = env.GetString("sound", "aychipvol", "ym").c_str();
  volType = (!strcasecmp(str, "ay") ? VolAy : VolYm);

  str = env.GetString("sound", "aychippan", "acb").c_str();

  if (!strcasecmp(str, "mono")) panType = PanMono;
  else if (!strcasecmp(str, "abc")) panType = PanABC;
  else if (!strcasecmp(str, "acb")) panType = PanACB;
  else if (!strcasecmp(str, "bac")) panType = PanBAC;
  else if (!strcasecmp(str, "bca")) panType = PanBCA;
  else if (!strcasecmp(str, "cab")) panType = PanCAB;
  else if (!strcasecmp(str, "cba")) panType = PanCBA;
}

const unsigned *C_AyChipConfig::GetVolTab(void)
{
  static unsigned volTab[2][32] = {
    // AY
    { 0x0000, 0x0000, 0x0340, 0x0340, 0x04C0, 0x04C0, 0x06F2, 0x06F2, 0x0A44, 0x0A44, 0x0F13, 0x0F13, 0x1510, 0x1510, 0x227E, 0x227E,
      0x289F, 0x289F, 0x414E, 0x414E, 0x5B21, 0x5B21, 0x7258, 0x7258, 0x905E, 0x905E, 0xB550, 0xB550, 0xD7A0, 0xD7A0, 0xFFFF, 0xFFFF
    },
    // YM
    { 0x0000, 0x0000, 0x00EF, 0x01D0, 0x0290, 0x032A, 0x03EE, 0x04D2, 0x0611, 0x0782, 0x0912, 0x0A36, 0x0C31, 0x0EB6, 0x1130, 0x13A0,
      0x1751, 0x1BF5, 0x20E2, 0x2594, 0x2CA1, 0x357F, 0x3E45, 0x475E, 0x5502, 0x6620, 0x7730, 0x8844, 0xA1D2, 0xC102, 0xE0A2, 0xFFFF
    }
  };

  return volTab[volType];
}

const unsigned *C_AyChipConfig::GetPanTab(void)
{
  static unsigned panTab[][6] = {
    { 100, 100, 100, 100, 100, 100 },		// Mono
    { 100, 10,  66, 66,   10, 100  },		// ABC
    { 100, 10,  10, 100,  66, 66   },		// ACB
    { 66, 66,   100, 10,  10, 100  },		// BAC
    { 10, 100,  100, 10,  66, 66   },		// BCA
    { 66, 66,   10, 100,  100, 10  },		// CAB
    { 10, 100,  66, 66,   100, 10  }		// CBA
  };

  return panTab[panType];
}

C_AyChip::C_AyChip()
{
  bitA = 0;
  bitB = 0;
  bitC = 0;
}

C_AyChip::~C_AyChip()
{
}

void C_AyChip::Init(void)
{
  conf.ReadConfig();

  SetTimings(SNDR_DEFAULT_SYSTICK_RATE, SNDR_DEFAULT_AY_RATE, SNDR_DEFAULT_SAMPLE_RATE);
  SetVolumes(0x7FFF, conf.GetVolTab(), conf.GetPanTab());

  Reset();
}

void C_AyChip::StartFrame(void)
{
  r13Reloaded = 0;
  sndRenderer.StartFrame();
}

void C_AyChip::EndFrame(unsigned devClk)
{
  uint64_t endChipTick = ((passedClkTicks + devClk) * chipClockRate) / systemClockRate;

  Flush((unsigned)(endChipTick - passedChipTicks));
  sndRenderer.EndFrame(t);

  passedClkTicks += devClk;
  passedChipTicks += t;
  t = 0;
}

void C_AyChip::Select(uint8_t reg)
{
  if (conf.chipType == C_AyChipConfig::TypeAy) {
    selectedReg = reg & 15;
  } else {
    selectedReg = reg;
  }
}

void C_AyChip::Write(unsigned devClk, uint8_t val)
{
  if (selectedReg > 15) return;

  if (selectedReg == 1 || selectedReg == 3 || selectedReg == 5 || selectedReg == 13) {
    val &= 0x0F;
  } else if (selectedReg == 6 || selectedReg == 8 || selectedReg == 9 || selectedReg == 10) {
    val &= 0x1F;
  }

  if (regs[selectedReg] == val && selectedReg != 13) return;
  regs[selectedReg] = val;

  if (devClk) Flush((devClk * multConst) >> MULT_C_1);

  switch (selectedReg)
  {
  case 0:
  case 1:
    fa = (uint16_t)regs[AYREG_fA] | (((uint16_t)regs[AYREG_fA + 1]) << 8);
    break;

  case 2:
  case 3:
    fb = (uint16_t)regs[AYREG_fB] | (((uint16_t)regs[AYREG_fB + 1]) << 8);
    break;

  case 4:
  case 5:
    fc = (uint16_t)regs[AYREG_fC] | (((uint16_t)regs[AYREG_fC + 1]) << 8);
    break;

  case 6:
    fn = val * 2;
    break;

  case 7:
    bit0 = 0 - ((val >> 0) & 1);
    bit1 = 0 - ((val >> 1) & 1);
    bit2 = 0 - ((val >> 2) & 1);
    bit3 = 0 - ((val >> 3) & 1);
    bit4 = 0 - ((val >> 4) & 1);
    bit5 = 0 - ((val >> 5) & 1);
    break;

  case 8:
    ea = (val & 0x10) ? -1 : 0;
    va = ((val & 0x0F) * 2 + 1) & ~ea;
    break;

  case 9:
    eb = (val & 0x10) ? -1 : 0;
    vb = ((val & 0x0F) * 2 + 1) & ~eb;
    break;

  case 10:
    ec = (val & 0x10) ? -1 : 0;
    vc = ((val & 0x0F) * 2 + 1) & ~ec;
    break;

  case 11:
  case 12:
    fe = (uint16_t)regs[AYREG_envT] | (((uint16_t)regs[AYREG_envT + 1]) << 8);
    break;

  case 13:
    r13Reloaded = 1;
    te = 0;

    if (regs[AYREG_env] & 4) {
      env = 0;
      denv = 1; // attack
    } else {
      env = 31;
      denv = ~(unsigned)0; // decay
    }
    break;
  }
}

uint8_t C_AyChip::Read(void)
{
  return (selectedReg > 15 ? 0xFF : regs[selectedReg]);
}

void C_AyChip::Reset(unsigned devClk)
{
  for (int i = 0; i < 14; i++) regs[i] = 0;
  ApplyRegs(devClk);
}

void C_AyChip::ApplyRegs(unsigned devClk)
{
  for (uint8_t r = 0; r < 16; r++)
  {
    Select(r);
    uint8_t p = regs[r];

    /* clr cached values */
    Write(devClk, p ^ 1);
    Write(devClk, p);
  }
}

void C_AyChip::Flush(unsigned tick)
{
  while (t < tick)
  {
    t++;

    if (++ta >= fa) {
      ta = 0;
      bitA ^= -1;
    }
    if (++tb >= fb) {
      tb = 0;
      bitB ^= -1;
    }
    if (++tc >= fc) {
      tc = 0;
      bitC ^= -1;
    }

    if (++tn >= fn)
    {
      tn = 0;
      ns = (ns * 2 + 1) ^ (((ns >> 16) ^ (ns >> 13)) & 1);
      bitN = 0 - ((ns >> 16) & 1);
    }

    if (++te >= fe)
    {
      te = 0;
      env += denv;

      if (env & ~31)
      {
        unsigned mask = (1 << regs[AYREG_env]);

        if (mask & ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 9) | (1 << 15))) {
          env = 0;
          denv = 0;
        } else if (mask & ((1 << 8) | (1 << 12))) {
          env &= 31;
        } else if (mask & ((1 << 10) | (1 << 14))) {
          denv = -(int)denv;
          env = env + denv;
        } else {
          env = 31;
          denv = 0; // 11,13
        }
      }
    }

    unsigned en, mixL, mixR;

    en = ((ea & env) | va) & ((bitA | bit0) & (bitN | bit3));
    mixL = vols[0][en];
    mixR = vols[1][en];

    en = ((eb & env) | vb) & ((bitB | bit1) & (bitN | bit4));
    mixL += vols[2][en];
    mixR += vols[3][en];

    en = ((ec & env) | vc) & ((bitC | bit2) & (bitN | bit5));
    mixL += vols[4][en];
    mixR += vols[5][en];

    sndRenderer.Update(t, mixL, mixR);
  }
}

void C_AyChip::SetVolumes(unsigned globalVol, const unsigned *volTab, const unsigned *panTab)
{
  for (int j = 0; j < 6; j++) {
    for (int i = 0; i < 32; i++) {
      vols[j][i] = (unsigned)(((uint64_t)globalVol * volTab[i] * panTab[j]) / (65535 * 100 * 3));
    }
  }
}

void C_AyChip::SetTimings(unsigned systemClockRate, unsigned chipClockRate, unsigned sampleRate)
{
  chipClockRate /= 8;

  this->systemClockRate = systemClockRate;
  this->chipClockRate = chipClockRate;

  multConst = (unsigned)(((uint64_t)chipClockRate << MULT_C_1) / systemClockRate);
  sndRenderer.SetTimings(chipClockRate, sampleRate);

  passedChipTicks = 0;
  passedClkTicks = 0;
  t = 0;
  ns = 0xFFFF;

  ApplyRegs();
}
