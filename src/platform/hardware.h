#ifndef PLATFORM__HARDWARE_H__INCLUDED
#define PLATFORM__HARDWARE_H__INCLUDED

#include <cstdint>
#include "platform_hardware.h"

enum HardwareSoundDriver {
    SOUND_DRIVER_NONE,
    SOUND_DRIVER_GENERIC,
    SOUND_DRIVER_NATIVE
};

enum HardwareRenderMode {
    RENDER_MODE_1X,
    RENDER_MODE_2X,
    RENDER_MODE_2X_SCANLINES
};

class Hardware {
public:

    Hardware() {}
    virtual ~Hardware() {}

    virtual HardwareRenderMode getRenderMode() = 0;
    virtual void setRenderMode(HardwareRenderMode mode) = 0;

    virtual bool isKeyRepeat() = 0;
    virtual void setKeyRepeat(bool repeat) = 0;

    virtual bool isFullscreen() = 0;
    virtual void setFullscreen(bool fullscreen) = 0;

    virtual bool isSoundEnabled() = 0;
    virtual void setSoundEnabled(bool enabled) = 0;

    virtual void renderFrame(uint32_t* pixels, int width, int height) = 0;
    virtual void renderSound(uint32_t* buffer, int samples) = 0;

private:

    Hardware(const Hardware&);
    Hardware& operator=(const Hardware&);
};

#endif
