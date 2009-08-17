#include "../defines.h"
#include <SDL.h>
#include "sdlwaveplayer.h"

SDLWavePlayer::SDLWavePlayer(unsigned int bufferSize)
{
	Initialize(bufferSize, bufferSize - bufferSize / 3, 8);
}

SDLWavePlayer::SDLWavePlayer(unsigned int bufferSize, unsigned int preBufferSize)
{
	Initialize(bufferSize, preBufferSize, 8);
}

SDLWavePlayer::~SDLWavePlayer()
{
	delete[] ringBuffer;
}

void SDLWavePlayer::Init(void)
{
	if (alreadyInited) StrikeError("[SDLWavePlayer::Init] Init called twice");

	SDL_AudioSpec desired, obtained;

	desired.freq = 44100;
	desired.format = AUDIO_S16SYS;
	desired.channels = 2;
	desired.samples = SDLWAVE_CALLBACK_BUFFER_SIZE / desired.channels / 2 /* 16 bit */;
	desired.callback = AudioCallback;
	desired.userdata = this;

	if (SDL_OpenAudio(&desired, &obtained) < 0) StrikeError("[SDLWavePlayer::Init] Couldn't open audio: %s\n", SDL_GetError());
	SDL_PauseAudio(0);

	alreadyInited = true;
}

void SDLWavePlayer::Write(BYTE* data, unsigned len)
{
	if (!alreadyInited) StrikeError("[SDLWavePlayer::Write] Call Init first");
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

void SDLWavePlayer::Initialize(unsigned int bufferSize, unsigned int preBufferSize, unsigned int preAgainCnt)
{
	if (bufferSize < SDLWAVE_CALLBACK_BUFFER_SIZE*4) StrikeError("[SDLWavePlayer::Initialize] Buffer size must be >= SDLWAVE_CALLBACK_BUFFER_SIZE*4");
	if (preBufferSize < (bufferSize / 4)) StrikeError("[SDLWavePlayer::Initialize] PreBuffer size must be >= BufferSize/4");
	if (preBufferSize > (bufferSize - bufferSize/4)) StrikeError("[SDLWavePlayer::Initialize] PreBuffer size must be < BufferSize-BufferSize/4");

	unsigned int tmp = bufferSize;
	unsigned int cnt = 0;

	while (tmp)
	{
		if (tmp & 1) cnt++;
		tmp >>= 1;
	}

	if (cnt != 1) StrikeError("[SDLWavePlayer::Initialize] Buffer size must be power of two");

	this->size = bufferSize;
	this->mask = bufferSize - 1;
	this->preBufferSize = preBufferSize;
	this->preAgainCnt = preAgainCnt;

	ringBuffer = new BYTE[bufferSize];

	audioPtr = 0;
	dataPtr = 0;
	alreadyInited = false;
	firstRun = true;
	currentPreAgainCnt = 0;
}

unsigned int SDLWavePlayer::CalcDist(void)
{
	if (dataPtr < audioPtr) {
		return (audioPtr - dataPtr);
	} else {
		return (size - dataPtr + audioPtr);
	}
}

void SDLWavePlayer::AudioCallback(void *userData, Uint8 *stream, int len)
{
	if (len <= 0) return;

	SDLWavePlayer* self = (SDLWavePlayer*)userData;

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

	if (dist < len)
	{
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
		BYTE* ptr = &self->ringBuffer[self->audioPtr];

		while (sz--) *(stream++) = *(ptr++);
	}
	else
	{
		int sz = self->size - self->audioPtr;
		BYTE* ptr = &self->ringBuffer[self->audioPtr];

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
