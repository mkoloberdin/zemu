#ifndef ZEMU_SOUND_MIXER_H
#define ZEMU_SOUND_MIXER_H

#include <vector>
#include "../file.h"
#include "snd_backend.h"
#include "snd_backend_sdl.h"
#include "snd_backend_oss.h"
#include "snd_backend_win32.h"
#include "snd_renderer.h"

#define MIX_BUFFER_SIZE 1024

class CSoundMixer {
public:
	CSoundMixer() { Initialized = false; sndBackend = 0; RecordWav = 0; }
	~CSoundMixer();
	void InitBackendSDL(int inSDLBufferSize);
#ifndef _WIN32
	void InitBackendOSS(int inSoundParam);
#else
	void InitBackendWin32(int inSoundParam);
#endif
	void Init(int *inMixerModePtr, bool inRecordWav, const char *inWavFileName);
	void AddSource(C_SndRenderer *Source);
	void FlushFrame(bool SoundEnabled);
	s_Sample MixBuffer[MIX_BUFFER_SIZE * 2];

private:
	bool Initialized;
	CSndBackend *sndBackend;
	int *MixerModePtr;
	std::vector<C_SndRenderer *> Sources;
	uint16_t AudioBuffer[MIX_BUFFER_SIZE * 2];
	bool RecordWav;
	const char *WavFileName;
	C_File WavFile;
};

extern CSoundMixer SoundMixer;

#endif // ZEMU_SOUND_MIXER_H
