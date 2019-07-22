#ifndef HOST_DRIVER__SOUND_DRIVER_H__INCLUDED
#define HOST_DRIVER__SOUND_DRIVER_H__INCLUDED

#include <cstdint>

class SoundDriver {
public:

    SoundDriver() {}
    virtual ~SoundDriver() {}

    virtual void render(uint32_t* buffer, int samples) = 0;

private:

    SoundDriver(const SoundDriver&);
    SoundDriver& operator=(const SoundDriver&);
};

#endif
