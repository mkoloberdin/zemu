#ifndef HOST_DRIVER__SOUND_DRIVER_OSS_H__INCLUDED
#define HOST_DRIVER__SOUND_DRIVER_OSS_H__INCLUDED

#ifdef __unix__

#include "sound_driver.h"

class SoundDriverOss : public SoundDriver {
public:

    SoundDriverOss(int soundFreq, int maxFragments, int sizeSelector);
    ~SoundDriverOss();

    void render(uint32_t* buffer, int samples);

private:

    void ioctlApply(unsigned long request, int value, const char* errorMessage);

    int audioDescriptor;
};

#endif

#endif
