#include "voc_format.h"

#define VOC_THRESHOLD 140

C_VocFormat::C_VocFormat()
{
  allTicks = 0;
  active = false;
}

C_VocFormat::~C_VocFormat()
{
  if (IFS.is_open()) IFS.close();
}

bool C_VocFormat::load(const fs::path& fname)
{
  if(IFS.is_open())
    IFS.close();
  active = false;

  IFS.open(fname, std::ios_base::binary);

  char buf[21];
  buf[20] = 0;

  IFS.read(buf, 20);

  if (strcmp(buf, "Creative Voice File\032"))
  {
    IFS.close();
    DEBUG_MESSAGE("\"Creative Voice File\" string not found");
    return false;
  }

  uint16_t dataOffset = readU16(IFS);
  uint16_t verNum = readU16(IFS);
  uint16_t verChk = readU16(IFS);

  if ((uint16_t)((0xFFFF - verNum) + 0x1234) != verChk)
  {
    IFS.close();
    DEBUG_MESSAGE("Invalid voc file");
    return false;
  }

  long maxRate = 0;
  long rate;

  IFS.seekg(dataOffset, std::ios_base::beg);

  while (true)
  {
    uint8_t type = readU8(IFS);
    if(IFS.eof())
      break;

    if (type == 0) {
      break; /* Terminator */
    }

    long size = (long)readU8(IFS) + 0x100L * (long)readU16(IFS);
    long pos = IFS.tellg();

    if (type == 1)  /* Sound data */
    {
      long sr = readU8(IFS);
      rate = 1000000 / (256 - sr);
      if (rate > maxRate) maxRate = rate;
    }
    else if (type == 3) /* Silence */
    {
      readU16(IFS); // skip Lenght of silence
      long sr = readU8(IFS);
      rate = 1000000 / (256 - sr);
      if (rate > maxRate) maxRate = rate;
    }
    else if (type == 8) /* Extended */
    {
      long tc = readU16(IFS);
      readU8(IFS);  // skip Pack

      rate = 256000000 / (65536 - tc);
      if (readU8(IFS)) rate /= 2; // Stereo

      if (rate > maxRate) maxRate = rate;
    }
    else if (type == 9)
    {
      rate = readU32(IFS);
      if (rate > maxRate) maxRate = rate;
    }

    IFS.seekg(pos + size, std::ios_base::beg);
  }

  // dataSize = sz * rate

  IFS.seekg(dataOffset, std::ios_base::beg);

  /*
  	dataPos = 0;
  	allTicks = 0;
  	sampleSz = (bits == 8 ? 1 : (bits == 16 ? 2 : 3)) * channels;

  	divider = (MAX_FRAME_TACTS * 50) / rate;		// 71680 = MAX_FRAME_TACTS
  */
  return true;
}

bool C_VocFormat::processTicks(uint64_t ticks)
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

bool C_VocFormat::getCurrBit(void)
{
  return (active ? currBit : true);
}

void C_VocFormat::stop(void)
{
  active = false;
}

void C_VocFormat::start(void)
{
  active = true;
}

void C_VocFormat::rewind(void)
{
//	dataPos = 0;
  allTicks = 0;
}

unsigned int C_VocFormat::getPosPerc(void)
{
  /*	uint32_t pos = (allTicks / divider) * sampleSz;
    return ((pos >= dataSize) ? 100 : (pos * 100 / dataSize)); */
  return 0;
}

bool C_VocFormat::isActive(void)
{
  return active;
}
