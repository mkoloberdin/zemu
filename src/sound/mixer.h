#ifndef _MIXER_H_INCLUDED_
#define _MIXER_H_INCLUDED_

#include <vector>
#include "zemu_env.h"
#include "snd_renderer.h"

#define MIX_BUFFER_SIZE 1024

class C_SoundMixer {
public:

    C_SoundMixer() {} //-V730
    ~C_SoundMixer();

    void Init(int mixerMode, bool recordWav, const char* wavName);
    void AddSource(C_SndRenderer* source);
    void FlushFrame(bool soundEnabled);
    s_Sample mixBuffer[MIX_BUFFER_SIZE * 2];

private:

    bool initialized = false;
    int mixerMode;
    std::vector<C_SndRenderer*> sources;
    uint32_t audioBuffer[MIX_BUFFER_SIZE];
    PathPtr wavPath;
    PathPtr wavTempPath;
    DataWriterPtr wavDataWriter;
};

extern C_SoundMixer soundMixer;

#endif
