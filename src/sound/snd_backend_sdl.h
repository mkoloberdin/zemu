#ifndef ZEMU_SND_BACKEND_SDL_H
#define ZEMU_SND_BACKEND_SDL_H

#include <SDL.h>
#include "snd_backend.h"

#define SDLWAVE_CALLBACK_BUFFER_SIZE 4096

class CSndBackendSDL : public CSndBackend {
public:

  CSndBackendSDL(unsigned int bufferSize);
  CSndBackendSDL(unsigned int bufferSize, unsigned int preBufferSize);
  ~CSndBackendSDL();

  void Init(void);
  void Write(uint8_t *data, unsigned len);

protected:

  void Initialize(unsigned int bufferSize, unsigned int preBufferSize, unsigned int preAgainCnt);
  unsigned int CalcDist(void);

  static void AudioCallback(void *userData, Uint8 *stream, int len);

  bool alreadyInited;
  volatile bool firstRun;

  unsigned int size, mask, preBufferSize, preAgainCnt;
  volatile unsigned int audioPtr, dataPtr;
  unsigned int currentPreAgainCnt;

  uint8_t *ringBuffer;

private:

  CSndBackendSDL(const CSndBackendSDL &f);
  CSndBackendSDL &operator=(const CSndBackendSDL &f);
};

#endif // ZEMU_SND_BACKEND_SDL_H
