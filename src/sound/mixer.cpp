#include <assert.h>
#include "mixer.h"

CSoundMixer SoundMixer;

#define AUDIO_HW_BUFFER 1024
#define MIXER_HALF_VOL_MASK 1
#define MIXER_SMART_MASK 2

void CSoundMixer::InitBackendSDL(int inSDLBufferSize) {
	sndBackend = new CSndBackendSDL(SDLWAVE_CALLBACK_BUFFER_SIZE * inSDLBufferSize);
	sndBackend->Init();
}

#ifndef _WIN32
void CSoundMixer::InitBackendOSS(int inSoundParam) {
	sndBackend = new CSndBackendOSS(inSoundParam, AUDIO_HW_BUFFER);
	sndBackend->Init();
}

#else
void CSoundMixer::InitBackendWin32(int inSoundParam) {
	sndBackend = new CSndBackendWIN32(inSoundParam, AUDIO_HW_BUFFER);
	sndBackend->Init();
}
#endif // !_WIN32

void CSoundMixer::Init(int *inMixerModePtr, bool inRecordWav, const char *inWavFileName) {
	assert(sndBackend != 0);
	MixerModePtr = inMixerModePtr;
	RecordWav = inRecordWav;
	WavFileName = inWavFileName;
	if (RecordWav) {
		WavFile.Write("output.wav.tmp");
	}
	Initialized = true;
}

CSoundMixer::~CSoundMixer() {
	if (Initialized) {
		if (RecordWav)
		{
			WavFile.Close();

			long wavSz = C_File::FileSize("output.wav.tmp");
// TODO: use appropriate library for writing wave files where available
			WavFile.Write(WavFileName);
			WavFile.PutDWORD(0x46464952);
			WavFile.PutDWORD(wavSz + 0x40 - 8);
			WavFile.PutDWORD(0x45564157);
			WavFile.PutDWORD(0x20746D66);
			WavFile.PutDWORD(0x10);
			WavFile.PutWORD(1);
			WavFile.PutWORD(2);
			WavFile.PutDWORD(44100);
			WavFile.PutDWORD(44100 * 4);
			WavFile.PutWORD(4);
			WavFile.PutWORD(16);
			WavFile.PutDWORD(0x61746164);
			WavFile.PutDWORD(wavSz);

			C_File wavTmp;

			wavTmp.Read("output.wav.tmp");
			while (!wavTmp.Eof()) WavFile.PutBYTE(wavTmp.GetBYTE());

			wavTmp.Close();
			WavFile.Close();

			unlink("output.wav.tmp");
		}
	}
}

void CSoundMixer::AddSource(C_SndRenderer *Source) {
	Sources.push_back(Source);
	Source->mixBuffer = MixBuffer;
}

void CSoundMixer::FlushFrame(bool SoundEnabled) {
	assert(Initialized);
	int nSources = Sources.size();
	if (nSources == 0) return;
	int i;
	if (SoundEnabled)
	{
		unsigned minSamples = Sources[0]->samples;
		unsigned maxSamples = Sources[0]->samples;

		for (i = 1; i < nSources; i++)
		{
			unsigned samples = Sources[i]->samples;

			if (samples < minSamples) minSamples = samples;
			if (samples > maxSamples) maxSamples = samples;
		}

		if (minSamples)
		{
			s_Sample *p = MixBuffer;

			if (*MixerModePtr & MIXER_SMART_MASK)
			{
				if (nSources > 1)
				{
					for (i = minSamples; i--; p++)
					{
						p->left /= nSources;
						p->right /= nSources;
					}
				}
			}
			else
			{
				unsigned long divider = 0L;

				for (i = 0; i < nSources; i++)
				{
					if (Sources[i]->activeCnt)
					{
						divider++;
						Sources[i]->activeCnt--;
					}
				}

				//*/ lastDivider = divider;

				if (divider)
				{
					p = MixBuffer;

					for (i = minSamples; i--; p++)
					{
						p->left /= divider;
						p->right /= divider;
					}
				}
			}

			p = MixBuffer;
			uint16_t *o = AudioBuffer;

			if (*MixerModePtr & MIXER_HALF_VOL_MASK)
			{
				for (i = minSamples; i--; p++)
				{
					*(o++) = (uint16_t)(p->left >> 1);
					*(o++) = (uint16_t)(p->right >> 1);
				}
			}
			else
			{
				for (i = minSamples; i--; p++)
				{
					*(o++) = (uint16_t)((long)p->left - 0x8000L);
					*(o++) = (uint16_t)((long)p->right - 0x8000L);
				}
			}

			if (RecordWav)
			{
				o = AudioBuffer;

				for (i = minSamples; i--;)
				{
					WavFile.PutWORD(*(o++));
					WavFile.PutWORD(*(o++));
				}
			}

			sndBackend->Write((uint8_t*)AudioBuffer, minSamples*sizeof(uint16_t)*2);
		}

		if (maxSamples > minSamples) {
			memmove(MixBuffer, &MixBuffer[minSamples], (maxSamples-minSamples) * sizeof(s_Sample));
		}

		memset(&MixBuffer[maxSamples-minSamples], 0, (MIX_BUFFER_SIZE) * sizeof(s_Sample));

		for (i = 0; i < nSources; i++) {
			Sources[i]->samples -= minSamples;
		}
	}
	else
	{
		for (i = 0; i < nSources; i++) {
			Sources[i]->samples = 0;
		}
	}

}
