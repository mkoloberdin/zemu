#include "voc_format.h"

#define VOC_THRESHOLD 140

C_VocFormat::C_VocFormat()
{
	allTicks = 0;
	active = false;
}

C_VocFormat::~C_VocFormat()
{
	if (fl.IsOpened()) fl.Close();
}

bool C_VocFormat::Load(const char *fname)
{
	if (fl.IsOpened()) fl.Close();
	active = false;

	fl.Read(fname);

	char buf[21];
	buf[20] = 0;

	fl.ReadBlock(buf, 20);

	if (strcmp(buf, "Creative Voice File\032"))
	{
		fl.Close();
		DEBUG_MESSAGE("\"Creative Voice File\" string not found");
		return false;
	}

	uint16_t dataOffset = fl.GetWORD();
	uint16_t verNum = fl.GetWORD();
	uint16_t verChk = fl.GetWORD();

	if ((uint16_t)((0xFFFF - verNum) + 0x1234) != verChk)
	{
		fl.Close();
		DEBUG_MESSAGE("Invalid voc file");
		return false;
	}

	long maxRate = 0;
	long rate;

	fl.SetFilePointer(dataOffset);

	while (!fl.Eof())
	{
		uint8_t type = fl.GetBYTE();

		if (type == 0) {
			break; /* Terminator */
		}

		long size = (long)fl.GetBYTE() + (long)0x100 * (long)fl.GetWORD();
		long pos = fl.GetFilePointer();

		if (type == 1)	/* Sound data */
		{
			long sr = fl.GetBYTE();
			rate = 1000000 / (256 - sr);
			if (rate > maxRate) maxRate = rate;
		}
		else
		if (type == 3)	/* Silence */
		{
			fl.GetWORD();	// skip Lenght of silence
			long sr = fl.GetBYTE();
			rate = 1000000 / (256 - sr);
			if (rate > maxRate) maxRate = rate;
		}
		else
		if (type == 8)	/* Extended */
		{
			long tc = fl.GetWORD();
			fl.GetBYTE();	// skip Pack

			rate = 256000000 / (65536 - tc);
			if (fl.GetBYTE()) rate /= 2;	// Stereo

			if (rate > maxRate) maxRate = rate;
		}
		else
		if (type == 9)
		{
			rate = fl.GetDWORD();
			if (rate > maxRate) maxRate = rate;
		}

		fl.SetFilePointer(pos + size);
	}

	// dataSize = sz * rate

	fl.SetFilePointer(dataOffset);

/*
	dataPos = 0;
	allTicks = 0;
	sampleSz = (bits == 8 ? 1 : (bits == 16 ? 2 : 3)) * channels;

	divider = (MAX_FRAME_TACTS * 50) / rate;		// 71680 = MAX_FRAME_TACTS
*/
	return true;
}

bool C_VocFormat::ProcessTicks(uint64_t ticks)
{
/*	if (!active)
	{
		currBit = true;
		return true;
	}

	allTicks += ticks;
	uint32_t pos = (allTicks / divider) * sampleSz;

	if (pos > dataSize)
	{
		currBit = 1;
		active = false;
		return true;
	}

	int cnt = (pos - dataPos) - sampleSz;
	if (cnt <= 0) return true;

	while (cnt--)
	{
		fl.GetBYTE();
		dataPos++;
	}

	long long res = 0;

	switch (bits)
	{
		case 8:
			for (int i = 0; i < channels; i++) {
				res += fl.GetBYTE();
			}
			break;

		case 16:
			for (int i = 0; i < channels; i++) {
				res += (short)fl.GetWORD();
			}
			break;

		case 32:
			for (int i = 0; i < channels; i++) {
				res += (long)fl.GetDWORD();
			}
			break;
	}

	res /= channels;

	switch (bits)
	{
		case 8:
			dataPos += channels;
			break;

		case 16:
			dataPos += channels * 2;
			res /= 0x100;
			break;

		case 32:
			dataPos += channels * 4;
			res /= 0x10000;
			break;
	}

	if (res < 0) res = -res;

	currBit = (res > WAV_THRESHOLD);
	return true; */

	return false;
}

bool C_VocFormat::GetCurrBit(void)
{
	return (active ? currBit : true);
}

void C_VocFormat::Stop(void)
{
	active = false;
}

void C_VocFormat::Start(void)
{
	active = true;
}

void C_VocFormat::Rewind(void)
{
//	dataPos = 0;
	allTicks = 0;
}

unsigned int C_VocFormat::GetPosPerc(void)
{
/*	uint32_t pos = (allTicks / divider) * sampleSz;
	return ((pos >= dataSize) ? 100 : (pos * 100 / dataSize)); */
	return 0;
}

bool C_VocFormat::IsActive(void)
{
	return active;
}
