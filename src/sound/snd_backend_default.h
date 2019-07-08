#ifndef _SND_BACKEND_DEFAULT_H_INCLUDED_
#define _SND_BACKEND_DEFAULT_H_INCLUDED_

#include "snd_backend.h"

#define SND_CALLBACK_BUFFER_SIZE 4096

class CSndBackendDefault : public CSndBackend {
	public:

	CSndBackendDefault(unsigned int bufferSize);
	CSndBackendDefault(unsigned int bufferSize, unsigned int preBufferSize);
	~CSndBackendDefault();

	void Init(void);
	void Write(uint8_t* data, unsigned len);

	protected:

	void Initialize(unsigned int bufferSize, unsigned int preBufferSize, unsigned int preAgainCnt);
	unsigned int CalcDist(void);

	static void AudioCallback(void *userData, uint8_t *stream, int len);

	bool alreadyInited;
	volatile bool firstRun;

	unsigned int size, mask, preBufferSize, preAgainCnt;
	volatile unsigned int audioPtr, dataPtr;
	unsigned int currentPreAgainCnt;

	uint8_t* ringBuffer;

	private:

	CSndBackendDefault(const CSndBackendDefault& f);
	CSndBackendDefault& operator=(const CSndBackendDefault& f);
};

#endif
