#ifndef __SDLWAVEPLAYER_H__
#define __SDLWAVEPLAYER_H__

#define SDLWAVE_CALLBACK_BUFFER_SIZE 4096

class SDLWavePlayer
{
	public:

	SDLWavePlayer(unsigned int bufferSize);
	SDLWavePlayer(unsigned int bufferSize, unsigned int preBufferSize);
	virtual ~SDLWavePlayer();

	void Init(void);
	void Write(BYTE* data, unsigned len);

	protected:

	void Initialize(unsigned int bufferSize, unsigned int preBufferSize, unsigned int preAgainCnt);
	unsigned int CalcDist(void);

	static void AudioCallback(void *userData, Uint8 *stream, int len);

	bool alreadyInited;
	volatile bool firstRun;

	unsigned int size, mask, preBufferSize, preAgainCnt;
	volatile unsigned int audioPtr, dataPtr;
	unsigned int currentPreAgainCnt;

	BYTE* ringBuffer;

	private:

	SDLWavePlayer(const SDLWavePlayer& f);
	SDLWavePlayer& operator=(const SDLWavePlayer& f);
};

#endif /* __SDLWAVEPLAYER_H__ */
