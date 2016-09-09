#include "../defines.h"
#include <SDL.h>
#include "snd_backend_sdl.h"

CSndBackendSDL::CSndBackendSDL(unsigned int bufferSize)
{
  Initialize(bufferSize, bufferSize - bufferSize / 3, 8);
}

CSndBackendSDL::CSndBackendSDL(unsigned int bufferSize, unsigned int preBufferSize)
{
  Initialize(bufferSize, preBufferSize, 8);
}

CSndBackendSDL::~CSndBackendSDL()
{
  SDL_CloseAudio();
  delete[] ringBuffer;
}

void CSndBackendSDL::Init(void)
{
  if (alreadyInited) StrikeError("[CSndBackendSDL::Init] Init called twice");

  SDL_AudioSpec desired, obtained;

  desired.freq = SOUND_FREQ;
  desired.format = AUDIO_S16SYS;
  desired.channels = 2;
  desired.samples = SDLWAVE_CALLBACK_BUFFER_SIZE / desired.channels / 2 /* 16 bit */;
  desired.callback = AudioCallback;
  desired.userdata = this;

  if (SDL_OpenAudio(&desired, &obtained) < 0) StrikeError("[CSndBackendSDL::Init] Couldn't open audio: %s\n", SDL_GetError());
  SDL_PauseAudio(0);

  alreadyInited = true;
}

void CSndBackendSDL::Write(uint8_t *data, unsigned len)
{
  if (!alreadyInited) StrikeError("[CSndBackendSDL::Write] Call Init first");
  if (len <= 0) return;

  unsigned waitCnt = 0;

  for (;;)
  {
    unsigned int dist = CalcDist();

    if (dist <= 4)
    {
      SDL_Delay(1);

      waitCnt++;
      if (waitCnt < 2000) continue;

      SDL_LockAudio();

      currentPreAgainCnt = 0;
      audioPtr = 0;
      dataPtr = 0;
      firstRun = true;

      SDL_UnlockAudio();
      break;
    }

    waitCnt = 0;

    unsigned int sz = dist - 4;
    if (sz > len) sz = len;

    len -= sz;
    SDL_LockAudio();

    while (sz--)
    {
      ringBuffer[dataPtr] = *(data++);
      dataPtr = (dataPtr + 1) & mask;
    }

    SDL_UnlockAudio();
    if (len <= 0) break;
  }
}

void CSndBackendSDL::Initialize(unsigned int bufferSize, unsigned int preBufferSize, unsigned int preAgainCnt)
{
  if (bufferSize < SDLWAVE_CALLBACK_BUFFER_SIZE * 4) StrikeError("[CSndBackendSDL::Initialize] Buffer size must be >= SDLWAVE_CALLBACK_BUFFER_SIZE*4");
  if (preBufferSize < (bufferSize / 4)) StrikeError("[CSndBackendSDL::Initialize] PreBuffer size must be >= BufferSize/4");
  if (preBufferSize > (bufferSize - bufferSize / 4)) StrikeError("[CSndBackendSDL::Initialize] PreBuffer size must be < BufferSize-BufferSize/4");

  unsigned int tmp = bufferSize;
  unsigned int cnt = 0;

  while (tmp)
  {
    if (tmp & 1) cnt++;
    tmp >>= 1;
  }

  if (cnt != 1) StrikeError("[CSndBackendSDL::Initialize] Buffer size must be power of two");

  this->size = bufferSize;
  this->mask = bufferSize - 1;
  this->preBufferSize = preBufferSize;
  this->preAgainCnt = preAgainCnt;

  ringBuffer = new uint8_t[bufferSize];

  audioPtr = 0;
  dataPtr = 0;
  alreadyInited = false;
  firstRun = true;
  currentPreAgainCnt = 0;
}

unsigned int CSndBackendSDL::CalcDist(void)
{
  if (dataPtr < audioPtr) {
    return (audioPtr - dataPtr);
  } else {
    return (size - dataPtr + audioPtr);
  }
}

void CSndBackendSDL::AudioCallback(void *userData, Uint8 *stream, int len)
{
  if (len <= 0) return;

  CSndBackendSDL *self = (CSndBackendSDL *)userData;

  if (self->firstRun)
  {
    if (self->dataPtr < self->preBufferSize)
    {
      while (len--) *(stream++) = 0;
      return;
    }

    self->firstRun = false;
  }

  int dist;

  if (self->audioPtr < self->dataPtr) {
    dist = self->dataPtr - self->audioPtr;
  } else {
    dist = self->size - self->audioPtr + self->dataPtr;
  }

  if (dist <= len)
  {
    // printf("CSndBackendSDL::AudioCallback - missing audio data\n");
    self->currentPreAgainCnt += 2;

    if (self->currentPreAgainCnt > self->preAgainCnt)
    {
      self->currentPreAgainCnt = 0;
      self->audioPtr = 0;
      self->dataPtr = 0;
      self->firstRun = true;
    }

    while (len--) *(stream++) = 0;
    return;
  }

  if (self->currentPreAgainCnt > 0) self->currentPreAgainCnt--;

  if ((self->size - self->audioPtr) >= (unsigned)len)
  {
    int sz = len;
    uint8_t *ptr = &self->ringBuffer[self->audioPtr];

    while (sz--) *(stream++) = *(ptr++);
  }
  else
  {
    int sz = self->size - self->audioPtr;
    uint8_t *ptr = &self->ringBuffer[self->audioPtr];

    if (sz > 0) {
      while (sz--) *(stream++) = *(ptr++);
    }

    sz = len - (self->size - self->audioPtr);
    ptr = self->ringBuffer;

    if (sz > 0) {
      while (sz--) *(stream++) = *(ptr++);
    }
  }

  self->audioPtr = (self->audioPtr + len) & self->mask;
}
