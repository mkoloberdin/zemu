#include "../defines.h"
#include "../platform_hardware.h"
#include "snd_backend_default.h"

CSndBackendDefault::CSndBackendDefault(unsigned int bufferSize)
{
	Initialize(bufferSize, bufferSize - bufferSize / 3, 8);
}

CSndBackendDefault::CSndBackendDefault(unsigned int bufferSize, unsigned int preBufferSize)
{
	Initialize(bufferSize, preBufferSize, 8);
}

CSndBackendDefault::~CSndBackendDefault()
{
	ZHW_Audio_Close();
	delete[] ringBuffer;
}

void CSndBackendDefault::Init(void)
{
	if (alreadyInited) StrikeError("[CSndBackendDefault::Init] Init called twice");

	ZHW_Audio_Spec desired, obtained;

	desired.freq = SOUND_FREQ;
	desired.format = AUDIO_S16SYS;
	desired.channels = 2;
	desired.samples = SND_CALLBACK_BUFFER_SIZE / desired.channels / 2 /* 16 bit */;
	desired.callback = AudioCallback;
	desired.userdata = this;

	if (ZHW_Audio_Open(&desired, &obtained) < 0) {
		StrikeError("[CSndBackendDefault::Init] Couldn't open audio: %s\n", ZHW_Error_Get());
	}

	ZHW_Audio_Pause(0);
	alreadyInited = true;
}

void CSndBackendDefault::Write(uint8_t* data, unsigned len)
{
	if (!alreadyInited) StrikeError("[CSndBackendDefault::Write] Call Init first");
	if (len <= 0) return;

	unsigned waitCnt = 0;

	for (;;)
	{
		unsigned int dist = CalcDist();

		if (dist <= 4)
		{
			ZHW_Timer_Delay(1);

			waitCnt++;
			if (waitCnt < 2000) continue;

			ZHW_Audio_Lock();

			currentPreAgainCnt = 0;
			audioPtr = 0;
			dataPtr = 0;
			firstRun = true;

			ZHW_Audio_Unlock();
			break;
		}

		waitCnt = 0;

		unsigned int sz = dist - 4;
		if (sz > len) sz = len;

		len -= sz;
		ZHW_Audio_Lock();

		while (sz--)
		{
			ringBuffer[dataPtr] = *(data++);
			dataPtr = (dataPtr + 1) & mask;
		}

		ZHW_Audio_Unlock();
		if (len <= 0) break;
	}
}

void CSndBackendDefault::Initialize(unsigned int bufferSize, unsigned int preBufferSize, unsigned int preAgainCnt)
{
	if (bufferSize < SND_CALLBACK_BUFFER_SIZE*4) {
		StrikeError("[CSndBackendDefault::Initialize] Buffer size must be >= SND_CALLBACK_BUFFER_SIZE*4");
	}

	if (preBufferSize < (bufferSize / 4)) {
		StrikeError("[CSndBackendDefault::Initialize] PreBuffer size must be >= BufferSize/4");
	}

	if (preBufferSize > (bufferSize - bufferSize/4)) {
		StrikeError("[CSndBackendDefault::Initialize] PreBuffer size must be < BufferSize-BufferSize/4");
	}

	unsigned int tmp = bufferSize;
	unsigned int cnt = 0;

	while (tmp)
	{
		if (tmp & 1) cnt++;
		tmp >>= 1;
	}

	if (cnt != 1) {
		StrikeError("[CSndBackendDefault::Initialize] Buffer size must be power of two");
	}

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

unsigned int CSndBackendDefault::CalcDist(void)
{
	if (dataPtr < audioPtr) {
		return (audioPtr - dataPtr);
	} else {
		return (size - dataPtr + audioPtr);
	}
}

void CSndBackendDefault::AudioCallback(void *userData, uint8_t *stream, int len)
{
	if (len <= 0) return;

	CSndBackendDefault* self = (CSndBackendDefault*)userData;

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
		// printf("CSndBackendDefault::AudioCallback - missing audio data\n");
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
		uint8_t* ptr = &self->ringBuffer[self->audioPtr];

		while (sz--) *(stream++) = *(ptr++);
	}
	else
	{
		int sz = self->size - self->audioPtr;
		uint8_t* ptr = &self->ringBuffer[self->audioPtr];

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
