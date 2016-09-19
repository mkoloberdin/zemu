#ifndef _VOC_FORMAT_H_
#define _VOC_FORMAT_H_

#include "../zemu.h"
#include "tape_format.h"

class C_VocFormat : public C_TapeFormat
{
private:
  fs::ifstream IFS;
  uint64_t allTicks;
  bool active;
  bool currBit;

  unsigned long dataSize;

  /*
  	WORD compression;
  	WORD channels;
  	DWORD rate;
  	WORD bits;
  	DWORD dataSize;
  	DWORD dataFp;
  	DWORD dataPos;
  	unsigned int divider;
  	unsigned int sampleSz;
  */
public:
  C_VocFormat();
  virtual ~C_VocFormat();

  virtual bool load(const fs::path& fname);
  virtual bool processTicks(uint64_t ticks);
  virtual bool getCurrBit(void);
  virtual void stop(void);
  virtual void start(void);
  virtual void rewind(void);
  virtual unsigned int getPosPerc(void);
  virtual bool isActive(void);
};

#endif /* _VOC_FORMAT_H_ */
