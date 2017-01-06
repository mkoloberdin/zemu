#ifndef ZEMU_PLATFORM_H
#define ZEMU_PLATFORM_H

class IPlatform {
public:
    virtual ~IPlatform() {};
    virtual void toggleFullscreen() = 0;
    virtual void antiFlicker(int surfNumber) = 0;
    virtual void updateScreen() = 0;

    virtual int processEvents() = 0;

    // FIXME:
    virtual void *getScrSurf() = 0;

    //virtual void unlockRenderSurface() = 0;
    //virtual void lockRenderSurface() = 0;

    //virtual void render() = 0;

};

#endif //ZEMU_PLATFORM_H
