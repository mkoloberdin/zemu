#ifndef _SND_RENDERER_H_INCLUDED_
#define _SND_RENDERER_H_INCLUDED_

/*
 * code refactoring
 * restorer
 *
 * sound resampling core for Unreal Speccy project
 * created under public domain license by SMT, jan.2006
*/

#ifdef _MSC_VER
  typedef unsigned __int64 uint64_t;
#endif

#ifdef __GNUC__
  #include <stdint.h>
  #define HANDLE_PRAGMA_PACK_PUSH_POP
#endif

#include "../defines.h"
#include "../params.h"

const unsigned SNDR_DEFAULT_SYSTICK_RATE = MAX_FRAME_TACTS * 50; // ZX-Spectrum Z80 clock
const unsigned SNDR_DEFAULT_SAMPLE_RATE = SOUND_FREQ;
const unsigned SNDR_ACTIVE_CNT_UPD = 50;

struct s_Sample
{
  unsigned long left;
  unsigned long right;
};

class C_SndRenderer
{
public:

  unsigned int samples;
  unsigned int activeCnt;
  s_Sample *mixBuffer;

  C_SndRenderer();

  void SetTimings(unsigned clockRate, unsigned sampleRate);

  void StartFrame();
  void Update(unsigned clk, unsigned left, unsigned right);
  void EndFrame(unsigned clk);

protected:

  void Flush(unsigned endTick);

  s_Sample *startPos;
  s_Sample *currPos;

  unsigned mixL, mixR;
  unsigned tick, baseTick;
  unsigned s1l, s1r;
  unsigned s2l, s2r;

  unsigned sampleRate, clockRate;
  uint64_t passedClkTicks, passedSndTicks;
  unsigned multConst;
};

#endif
