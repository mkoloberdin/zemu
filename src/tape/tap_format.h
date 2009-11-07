#ifndef _TAP_FORMAT_H_
#define _TAP_FORMAT_H_

#include "../zemu.h"
#include "tape_format.h"

class C_TapFormat : public C_TapeFormat
{
	public:

	int state;
	int tapeBit;
	int counter;
	Z80EX_BYTE *data;
	long size;

	long blockPos;
	int blockSize;
	int posInBlock;
	Z80EX_BYTE currentByte;
	int delay;

	C_TapFormat();
	virtual ~C_TapFormat();

	bool Load(const char *fname);
	bool ProcessTicks(uint64_t ticks);
	bool GetCurrBit(void);
	void Stop(void);
	void Start(void);
	void Rewind(void);
	unsigned int GetPosPerc(void);
	bool IsActive(void);

	Z80EX_BYTE Data(long pos);
};

#endif /* _TAP_FORMAT_H_ */
