#ifndef HOST_DRIVER__SOUND_DRIVER_WIN32_H__INCLUDED
#define HOST_DRIVER__SOUND_DRIVER_WIN32_H__INCLUDED

#ifdef _WIN32

#include "sound_driver.h"

class SoundDriverWin32 : public SoundDriver {
public:

    SoundDriverWin32(int soundFreq, int wqSize);
    ~SoundDriverWin32();

    void render(uint32_t* buffer, int samples);

private:

    static const int MAX_WQSIZE = 32;
    static const int MAX_DSPIECE = (40000 * 4 / 20);

    int wqSize;
    HWAVEOUT hwo = 0;
    WAVEHDR wq[MAX_WQSIZE];
    uint8_t wbuffer[MAX_WQSIZE * MAX_DSPIECE];
    int wqHead;
    int wqTail;
};

#endif

#endif
