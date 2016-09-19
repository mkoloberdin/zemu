#ifndef _WAV_FORMAT_H_
#define _WAV_FORMAT_H_

#include "../zemu.h"
#include "tape_format.h"

class C_WavFormat : public C_TapeFormat
{
private:
  fs::ifstream IFS;
  uint64_t allTicks;
  uint16_t compression;
  uint16_t channels;
  uint32_t rate;
  uint16_t bits;
  uint32_t dataSize;
  uint32_t dataFp;
  uint32_t dataPos;
  unsigned int divider;
  unsigned int sampleSz;
  bool active;
  bool currBit;

public:
  C_WavFormat();
  virtual ~C_WavFormat();

  virtual bool load(const fs::path& fname);
  virtual bool processTicks(uint64_t ticks);
  virtual bool getCurrBit(void);
  virtual void stop(void);
  virtual void start(void);
  virtual void rewind(void);
  virtual unsigned int getPosPerc(void);
  virtual bool isActive(void);
};

#endif /* _WAV_FORMAT_H_ */
