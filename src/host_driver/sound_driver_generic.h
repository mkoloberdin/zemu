#ifndef HOST_IMPL__DRIVER__SOUND_DRIVER_GENERIC_H__INCLUDED
#define HOST_IMPL__DRIVER__SOUND_DRIVER_GENERIC_H__INCLUDED

#include "sound_driver.h"

class SoundDriverGeneric : public SoundDriver {
public:

    SoundDriverGeneric(int soundFreq, int bufferScale, int preBufferFragments, int fragmentSamplesScale);
    ~SoundDriverGeneric();

    void render(uint32_t* buffer, int samples);

private:

    int bufferSize;
    int bufferSizeMask;
    int preBufferSize;
    uint8_t* ringBuffer = nullptr;
    volatile int emptyFills = 0;
    volatile bool isInitialLoop = true;
    volatile int playPosition = 0;
    volatile int writePosition = 0;

    void fillStream(uint8_t* stream, int len);

    friend void soundDriverGenericAudioCallback(void* userData, uint8_t* stream, int len);
};

#endif
