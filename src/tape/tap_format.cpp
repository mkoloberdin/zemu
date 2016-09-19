#include "tap_format.h"

#define TAPE_STATE_STOP 0
#define TAPE_STATE_PLAY 1
#define TAPE_STATE_PILOT_TONE 2
#define TAPE_STATE_PILOT_TONE_LW 3
#define TAPE_STATE_PILOT_TONE_HW 4
#define TAPE_STATE_PILOT_END_LW 5
#define TAPE_STATE_PILOT_END_HW 6
#define TAPE_STATE_NEXT_BYTE 7
#define TAPE_STATE_NEXT_BIT 8
#define TAPE_STATE_BIT_LW 9
#define TAPE_STATE_BIT_HW 10
#define TAPE_STATE_DELAY 11

C_TapFormat::C_TapFormat()
{
  state = TAPE_STATE_STOP;
  tapeBit = 1;
  counter = 0;
  Data = nullptr;
  size = 0;

  blockPos = 0;
  blockSize = 0;
  posInBlock = 0;
  currentByte = 0;
  delay = 0;
}

C_TapFormat::~C_TapFormat()
{
  if (Data != nullptr) delete[] Data;
}

uint8_t C_TapFormat::data(long pos)
{
  return ((pos >= 0 && pos < size) ? Data[pos] : 0);
}

bool C_TapFormat::load(const fs::path& fname)
{
  if(!fs::exists(fname))
    return false;
  size = fs::file_size(fname);

  if (Data != nullptr) delete[] Data;
  Data = new uint8_t[size];

  fs::ifstream ifs(fname, std::ios_base::binary);
  ifs.read((char *)Data, size);
  
  blockPos = 0;
  blockSize = 0;
  posInBlock = 0;

  return true;
}

bool C_TapFormat::processTicks(uint64_t ticks)
{
  for (;;)
  {
    switch (state)
    {
    case TAPE_STATE_STOP:
      tapeBit = 1;
      return true;

    case TAPE_STATE_PLAY:
      if (blockPos >= size)
      {
        state = TAPE_STATE_STOP;
        break;
      }

      blockSize = data(blockPos) + 0x100 * data(blockPos + 1);
      posInBlock = -1;
      state = TAPE_STATE_PILOT_TONE;
      counter = data(blockPos + 2) ? 1611 : 4032;
      break;

    case TAPE_STATE_PILOT_TONE:
      tapeBit = 0;
      state = TAPE_STATE_PILOT_TONE_LW;
      return true;

    case TAPE_STATE_PILOT_TONE_LW:
      if (ticks < 2168) return false;
      tapeBit = 1;
      state = TAPE_STATE_PILOT_TONE_HW;
      return true;

    case TAPE_STATE_PILOT_TONE_HW:
      if (ticks < 2168) return false;
      tapeBit = 0;
      counter--;
      state = (counter <= 0 ? TAPE_STATE_PILOT_END_LW : TAPE_STATE_PILOT_TONE_LW);
      return true;

    case TAPE_STATE_PILOT_END_LW:
      if (ticks < 667) return false;
      tapeBit = 1;
      state = TAPE_STATE_PILOT_END_HW;
      return true;

    case TAPE_STATE_PILOT_END_HW:
      if (ticks < 735) return false;
      tapeBit = 0;
      state = TAPE_STATE_NEXT_BYTE;
      return true;

    case TAPE_STATE_NEXT_BYTE:
      posInBlock++;

      if (posInBlock > blockSize)
      {
        blockPos += blockSize + 2;
        state = TAPE_STATE_DELAY;
      }
      else
      {
        currentByte = data(blockPos + 2 + posInBlock);
        counter = 8;
        state = TAPE_STATE_NEXT_BIT;
      }
      break;

    case TAPE_STATE_NEXT_BIT:
    {
      counter--;

      if (counter < 0)
      {
        state = TAPE_STATE_NEXT_BYTE;
        break;
      }

      delay = ((currentByte & 128) ? 1710 : 855);
      currentByte = (currentByte << 1) & 0xFF;

      state = TAPE_STATE_BIT_LW;
      break;
    }

    case TAPE_STATE_BIT_LW:
      if (ticks < (uint64_t)delay) return false;
      tapeBit = 1;
      state = TAPE_STATE_BIT_HW;
      return true;

    case TAPE_STATE_BIT_HW:
      if (ticks < (uint64_t)delay) return false;
      tapeBit = 0;
      state = TAPE_STATE_NEXT_BIT;
      return true;

    case TAPE_STATE_DELAY:
      if (ticks < 2168 * 1000) return false;
      state = TAPE_STATE_PLAY;
      return true;

    default:
      return false;
    }
  }
}

bool C_TapFormat::getCurrBit(void)
{
  return tapeBit;
}

void C_TapFormat::stop(void)
{
  state = TAPE_STATE_STOP;
}

void C_TapFormat::start(void)
{
  if (Data != nullptr) state = TAPE_STATE_PLAY;
}

void C_TapFormat::rewind(void)
{
  if (state != TAPE_STATE_STOP) state = TAPE_STATE_PLAY;

  blockPos = 0;
  blockSize = 0;
  posInBlock = 0;
}

unsigned int C_TapFormat::getPosPerc(void)
{
  if (blockPos >= size) return 100;
  return (long)(blockPos + posInBlock) * 100L / size;
}

bool C_TapFormat::isActive(void)
{
  return (state != TAPE_STATE_STOP);
}
