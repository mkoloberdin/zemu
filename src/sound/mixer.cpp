#include <assert.h>
#include <defines.h>
#include "mixer.h"

C_SoundMixer soundMixer;

#define AUDIO_HW_BUFFER 1024
#define MIXER_FULL_VOL_MASK 1
#define MIXER_SMART_MASK 2

C_SoundMixer::C_SoundMixer()
{
  initialized = false;
  sndBackend = nullptr;
  recordWav = false;
}

void C_SoundMixer::InitBackendSDL(int sdlBufferSize)
{
  sndBackend = new CSndBackendSDL(SDLWAVE_CALLBACK_BUFFER_SIZE * sdlBufferSize);
  sndBackend->Init();
}

#if !defined(_WIN32) && !defined(__APPLE__)

void C_SoundMixer::InitBackendOSS(int soundParam)
{
  sndBackend = new CSndBackendOSS(soundParam, AUDIO_HW_BUFFER);
  sndBackend->Init();
}

#else
#ifdef _WIN32

void C_SoundMixer::InitBackendWin32(int soundParam)
{
  sndBackend = new CSndBackendWIN32(soundParam, AUDIO_HW_BUFFER);
  sndBackend->Init();
}

#endif
#endif // !_WIN32 && !__APPLE__

void C_SoundMixer::Init(int mixerModePtr, bool recordWav, const char *wavFileName)
{
  assert(sndBackend != nullptr);

  this->mixerMode = mixerModePtr;
  this->recordWav = recordWav;
  this->WavFileName = env.newDataFilePath(wavFileName);

  if (recordWav) {
    TmpWavFileName = env.newDataFilePath("output.wav.tmp"); // TODO: Use runtime dir instead
    TmpOFS.open(TmpWavFileName,
                 std::ios_base::binary | std::ios_base::trunc);
  }

  initialized = true;
}

C_SoundMixer::~C_SoundMixer()
{
  if (!initialized) {
    return;
  }

  if (recordWav)
  {
    // Close temporary file
    TmpOFS.close();
    
    // TODO: use appropriate library for writing wave files where available
    size_t WavSize = fs::file_size(TmpWavFileName);
    
    fs::ofstream ofs(
      WavFileName,
      std::ios_base::binary | std::ios_base::trunc
    );

    writeU32(ofs, 0x46464952);
    writeU32(ofs, WavSize + 0x40 - 8);
    writeU32(ofs, 0x45564157);
    writeU32(ofs, 0x20746D66);
    writeU32(ofs, 0x10);
    writeU16(ofs, 1);
    writeU16(ofs, 2);
    writeU32(ofs, SOUND_FREQ);
    writeU32(ofs, SOUND_FREQ * 4);
    writeU16(ofs, 4);
    writeU16(ofs, 16);
    writeU32(ofs, 0x61746164);
    writeU32(ofs, WavSize);

    fs::ofstream tmpIfs(
      TmpWavFileName,
      std::ios_base::binary
    );
    
    ofs << tmpIfs.rdbuf();
    
    // copy from temporary file
    tmpIfs.close();
    ofs.close();
    
    fs::remove(TmpWavFileName);
    
    //wavFile.Close();
/*
    // TODO: use appropriate library for writing wave files where available
    long wavSz = C_File::FileSize("output.wav.tmp");

    wavFile.Write(wavFileName);
    wavFile.PutDWORD(0x46464952);
    wavFile.PutDWORD(wavSz + 0x40 - 8);
    wavFile.PutDWORD(0x45564157);
    wavFile.PutDWORD(0x20746D66);
    wavFile.PutDWORD(0x10);
    wavFile.PutWORD(1);
    wavFile.PutWORD(2);
    wavFile.PutDWORD(SOUND_FREQ);
    wavFile.PutDWORD(SOUND_FREQ * 4);
    wavFile.PutWORD(4);
    wavFile.PutWORD(16);
    wavFile.PutDWORD(0x61746164);
    wavFile.PutDWORD(wavSz);

    C_File wavTmp;

    wavTmp.Read("output.wav.tmp");
    while (!wavTmp.Eof()) wavFile.PutBYTE(wavTmp.GetBYTE());

    wavTmp.Close();
    wavFile.Close();
    unlink("output.wav.tmp");
*/
  }
}

void C_SoundMixer::AddSource(C_SndRenderer *source)
{
  sources.push_back(source);
  source->mixBuffer = mixBuffer;
}

void C_SoundMixer::FlushFrame(bool soundEnabled)
{
  assert(initialized);

  int sourcesCount = sources.size();
  if (sourcesCount == 0) return;

  if (!soundEnabled)
  {
    for (int i = 0; i < sourcesCount; i++) {
      sources[i]->samples = 0;
    }

    return;
  }

  unsigned minSamples = sources[0]->samples;
  unsigned maxSamples = sources[0]->samples;

  for (int i = 1; i < sourcesCount; i++)
  {
    unsigned samples = sources[i]->samples;

    if (samples < minSamples) minSamples = samples;
    if (samples > maxSamples) maxSamples = samples;
  }

  if (minSamples)
  {
    s_Sample *p = mixBuffer;

    if (mixerMode & MIXER_SMART_MASK)
    {
      unsigned long divider = 0L;

      for (int i = 0; i < sourcesCount; i++)
      {
        if (sources[i]->activeCnt)
        {
          divider++;
          sources[i]->activeCnt--;
        }
      }

      //*/ lastDivider = divider;

      if (divider)
      {
        p = mixBuffer;

        for (int i = minSamples; i--; p++)
        {
          p->left /= divider;
          p->right /= divider;
        }
      }
    }
    else
    {
      if (sourcesCount > 1)
      {
        for (int i = minSamples; i--; p++)
        {
          p->left /= sourcesCount;
          p->right /= sourcesCount;
        }
      }
    }

    p = mixBuffer;
    uint16_t *o = audioBuffer;

    if (mixerMode & MIXER_FULL_VOL_MASK)
    {
      for (int i = minSamples; i--; p++)
      {
        *(o++) = (uint16_t)((long)p->left - 0x8000L);
        *(o++) = (uint16_t)((long)p->right - 0x8000L);
      }
    }
    else
    {
      for (int i = minSamples; i--; p++)
      {
        *(o++) = (uint16_t)(p->left >> 1);
        *(o++) = (uint16_t)(p->right >> 1);
      }
    }

    if (recordWav)
    {
      o = audioBuffer;

      for (int i = minSamples; i--;)
      {
        writeU16(TmpOFS, *(o++));
        writeU16(TmpOFS, *(o++));
//        WavFile.PutWORD(*(o++));
//        WavFile.PutWORD(*(o++));
      }
    }

    sndBackend->Write((uint8_t *)audioBuffer, minSamples * sizeof(uint16_t) * 2);
  }

  if (maxSamples > minSamples) {
    memmove(mixBuffer, &mixBuffer[minSamples], (maxSamples - minSamples) * sizeof(s_Sample));
  }

  memset(&mixBuffer[maxSamples - minSamples], 0, (MIX_BUFFER_SIZE) * sizeof(s_Sample));

  for (int i = 0; i < sourcesCount; i++) {
    sources[i]->samples -= minSamples;
  }
}
