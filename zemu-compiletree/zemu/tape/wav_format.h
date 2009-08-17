#ifndef _WAV_FORMAT_H_
#define _WAV_FORMAT_H_

#include "../zemu.h"
#include "../file.h"
#include "tape_format.h"

class C_WavFormat : public C_TapeFormat
{
	public:

	C_File fl;
	unsigned long long allTicks;
	WORD compression;
	WORD channels;
	DWORD rate;
	WORD bits;
	DWORD dataSize;
	DWORD dataFp;
	DWORD dataPos;
	unsigned int divider;
	unsigned int sampleSz;
	bool active;
	bool currBit;

	C_WavFormat();
	virtual ~C_WavFormat();

	bool Load(const char *fname);
	bool ProcessTicks(unsigned long long ticks);
	bool GetCurrBit(void);
	void Stop(void);
	void Start(void);
	void Rewind(void);
	unsigned int GetPosPerc(void);
	bool IsActive(void);
};

#endif /* _WAV_FORMAT_H_ */
