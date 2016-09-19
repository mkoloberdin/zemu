#include "wav_format.h"

#define WAV_THRESHOLD 140

C_WavFormat::C_WavFormat()
{
  allTicks = 0;
  active = false;
}

C_WavFormat::~C_WavFormat()
{
  if(IFS.is_open())
    IFS.close();
}

bool C_WavFormat::load(const fs::path& fname)
{
  if(IFS.is_open())
    IFS.close();
  active = false;

  IFS.open(fname, std::ios_base::binary);
  char chunkName[5];
  chunkName[4] = 0;

  IFS.read(chunkName, 4);

  if (strcmp(chunkName, "RIFF"))
  {
    IFS.close();
    DEBUG_MESSAGE("RIFF chunk not found");
    return false;
  }

  readU32(IFS); // skip chunk size

  IFS.read(chunkName, 4);

  if (strcmp(chunkName, "WAVE"))
  {
    IFS.close();
    DEBUG_MESSAGE("Invalid RIFF type (must be WAVE)");
    return false;
  }

  bool fmtFound = false;

  while (!fmtFound)
  {
    IFS.read(chunkName, 4);
    if (IFS.eof())
      break;

    uint32_t size = readU32(IFS);
    if (IFS.eof())
      break;

    if (strcmp(chunkName, "fmt "))
    {
      IFS.seekg(size, std::ios_base::cur);
      continue;
    }

    compression = readU16(IFS);

    if (compression != 1)
    {
      IFS.close();
      DEBUG_MESSAGE("Only uncompressed PCM supported");
      return false;
    }

    channels = readU16(IFS);
    rate = readU32(IFS);

    readU32(IFS); // skip byte rate
    readU16(IFS); // skip block align

    bits = readU16(IFS);

    if (bits != 8 && bits != 16 && bits != 32)
    {
      IFS.close();
      DEBUG_MESSAGE("Unsupported bits per sample. 8, 16 or 32 supported");
      return false;
    }

    fmtFound = true;
  }

  if (!fmtFound)
  {
    IFS.close();
    DEBUG_MESSAGE("fmt subchunk not found");
    return false;
  }

  bool dataFound = false;
  IFS.seekg(12, std::ios_base::beg);

  while (!dataFound)
  {
    IFS.read(chunkName, 4);
    if (IFS.eof())
      break;

    dataSize = readU32(IFS);
    if (IFS.eof()) break;

    if (strcmp(chunkName, "data"))
    {
      IFS.seekg(dataSize, std::ios_base::cur);
      continue;
    }

    dataFp = IFS.tellg();
    dataFound = true;
  }

  if (!dataFound)
  {
    IFS.close();
    DEBUG_MESSAGE("data chunk not found");
    return false;
  }

  dataPos = 0;
  allTicks = 0;
  sampleSz = (bits == 8 ? 1 : (bits == 16 ? 2 : 3)) * channels;

  divider = (MAX_FRAME_TACTS * 50) / rate;

  return true;
}

bool C_WavFormat::processTicks(uint64_t ticks)
{
  if (!active)
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

  int samples = (pos - dataPos) / sampleSz;
  int samplesCnt = samples;
  int64_t allRes = 0;

  /*
  while (cnt--)
  {
  	fl.GetBYTE();
  	dataPos++;
  }
  */

  while (samplesCnt--)
  {
    int64_t res = 0;

    switch (bits)
    {
    case 8:
      for (int i = 0; i < channels; i++) res += readU8(IFS);
      dataPos += channels;
      break;

    case 16:
      for (int i = 0; i < channels; i++) res += (short)readU16(IFS);
      dataPos += channels * 2;
      res /= 0x100;
      break;

    case 32:
      for (int i = 0; i < channels; i++) res += (long)readU32(IFS);
      dataPos += channels * 4;
      res /= 0x10000;
      break;
    }

    res /= channels;
    allRes += res;
  }

  allRes /= samples;

  /*
  if (res < 0) res = -res;
  currBit = (res > WAV_THRESHOLD);
  */

  if (allRes < 0) allRes = -allRes;
  currBit = (allRes > WAV_THRESHOLD);

  return true;
}

bool C_WavFormat::getCurrBit(void)
{
  return (active ? currBit : true);
}

void C_WavFormat::stop(void)
{
  active = false;
}

void C_WavFormat::start(void)
{
  active = true;
}

void C_WavFormat::rewind(void)
{
  dataPos = 0;
  allTicks = 0;
  if (IFS.is_open())
    IFS.seekg(0, std::ios_base::beg);
}

unsigned int C_WavFormat::getPosPerc(void)
{
  uint32_t pos = (allTicks / divider) * sampleSz;
  return ((pos >= dataSize) ? 100 : (pos * 100 / dataSize));
}

bool C_WavFormat::isActive(void)
{
  return active;
}
