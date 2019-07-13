#ifndef _MIXER_H_INCLUDED_
#define _MIXER_H_INCLUDED_

#include <vector>
#include "../zemu_env.h"
#include "snd_backend.h"
#include "snd_backend_default.h"
#include "snd_backend_oss.h"
#include "snd_backend_win32.h"
#include "snd_renderer.h"

#define MIX_BUFFER_SIZE 1024

class C_SoundMixer {
    public:

    C_SoundMixer() {} //-V730
    ~C_SoundMixer();
    void InitBackendDefault(int bufferSize);

    #ifdef _WIN32
        void InitBackendWin32(int soundParam);
    #endif

    #ifdef __unix__
        void InitBackendOSS(int soundParam);
    #endif

    void Init(int mixerMode, bool recordWav, const char* wavFileName);
    void AddSource(C_SndRenderer* source);
    void FlushFrame(bool soundEnabled);
    s_Sample mixBuffer[MIX_BUFFER_SIZE * 2];

    private:

    bool initialized = false;
    CSndBackend* sndBackend = nullptr;
    int mixerMode;
    std::vector<C_SndRenderer*> sources;
    uint16_t audioBuffer[MIX_BUFFER_SIZE * 2];
    std::unique_ptr<Path> wavFilePath;
    std::unique_ptr<Path> wavFileTempPath;
    std::unique_ptr<FileWriter> wavFileWriter;
};

extern C_SoundMixer soundMixer;

#endif
