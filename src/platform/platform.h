#ifndef ZEMU_PLATFORM_H
#define ZEMU_PLATFORM_H

#include <stdint.h>

typedef struct {
    uint8_t *pixels;
    int pitch;
} PixBuf;

class IPlatform {
public:
    virtual ~IPlatform() {};

    virtual void toggleFullscreen() = 0;

    virtual void antiFlicker(int surfNumber) = 0;

    void antiFlickerOn() {
        this->antiFlickerActive = true;
        this->doCopyOfSurfaces = true;
    }

    void antiFlickerOff() { this->antiFlickerActive = false; }

    void renderOn() { this->renderActive = true; }

    void renderOff() { this->renderActive = false; }

    bool isRenderOn() { return this->renderActive; }

    virtual const PixBuf *getPixBuf() = 0;

    virtual void releasePixBuf() = 0;

    virtual void updateScreen() = 0;

    virtual int processEvents() = 0;

    // FIXME:
    //virtual void *getScrSurf() = 0;

    //virtual void unlockRenderSurface() = 0;
    //virtual void lockRenderSurface() = 0;

    //virtual void render() = 0;

protected:
    // helper flags for anti-flicker
    bool antiFlickerActive = false;
    bool doCopyOfSurfaces = false;

    bool renderActive;

    PixBuf pixBuf;
};

#endif //ZEMU_PLATFORM_H
