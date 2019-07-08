#ifndef _WAV_FORMAT_H_INCLUDED_
#define _WAV_FORMAT_H_INCLUDED_

#include "../zemu.h"
#include "../file.h"
#include "tape_format.h"

class C_WavFormat : public C_TapeFormat
{
	public:

	C_File fl;
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

	C_WavFormat();
	virtual ~C_WavFormat();

	bool Load(const char *fname);
	bool ProcessTicks(uint64_t ticks);
	bool GetCurrBit(void);
	void Stop(void);
	void Start(void);
	void Rewind(void);
	unsigned int GetPosPerc(void);
	bool IsActive(void);
};

#endif
