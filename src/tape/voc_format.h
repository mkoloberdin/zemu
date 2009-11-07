#ifndef _VOC_FORMAT_H_
#define _VOC_FORMAT_H_

#include "../zemu.h"
#include "../file.h"
#include "tape_format.h"

class C_VocFormat : public C_TapeFormat
{
	public:

	C_File fl;
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
	C_VocFormat();
	virtual ~C_VocFormat();

	bool Load(const char *fname);
	bool ProcessTicks(uint64_t ticks);
	bool GetCurrBit(void);
	void Stop(void);
	void Start(void);
	void Rewind(void);
	unsigned int GetPosPerc(void);
	bool IsActive(void);
};

#endif /* _VOC_FORMAT_H_ */
