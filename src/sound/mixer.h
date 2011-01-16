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

class C_SoundMixer
{
	public:

	C_SoundMixer();
	~C_SoundMixer();
	void InitBackendSDL(int sdlBufferSize);

#ifndef _WIN32
	void InitBackendOSS(int soundParam);
#else
	void InitBackendWin32(int soundParam);
#endif

	void Init(int mixerMode, bool recordWav, const char *wavFileName);
	void AddSource(C_SndRenderer *source);
	void FlushFrame(bool soundEnabled);
	s_Sample mixBuffer[MIX_BUFFER_SIZE * 2];

	private:

	bool initialized;
	CSndBackend *sndBackend;
	int mixerMode;
	std::vector<C_SndRenderer *> sources;
	uint16_t audioBuffer[MIX_BUFFER_SIZE * 2];
	bool recordWav;
	const char *wavFileName;
	C_File wavFile;
};

extern C_SoundMixer soundMixer;

#endif // ZEMU_SOUND_MIXER_H
