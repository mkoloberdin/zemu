#ifndef _TAP_FORMAT_H_
#define _TAP_FORMAT_H_

#include <cstdint>
#include "tape_format.h"

class C_TapFormat : public C_TapeFormat
{
private:
  int state;
  int tapeBit;
  int counter;
  uint8_t *Data;
  long size;

  long blockPos;
  int blockSize;
  int posInBlock;
  uint8_t currentByte;
  int delay;

public:
  C_TapFormat();
  virtual ~C_TapFormat();

  virtual bool load(const fs::path& fname);
  virtual bool processTicks(uint64_t ticks);
  virtual bool getCurrBit(void);
  virtual void stop(void);
  virtual void start(void);
  virtual void rewind(void);
  virtual unsigned int getPosPerc(void);
  virtual bool isActive(void);

  uint8_t data(long pos);
};

#endif /* _TAP_FORMAT_H_ */
