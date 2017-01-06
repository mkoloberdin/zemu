#ifndef ZEMU_SDL_PLATFORM_H
#define ZEMU_SDL_PLATFORM_H

#include "defines.h"
#include <SDL.h>

#include "platform/platform.h"

extern SDL_Surface *screen;

extern int PITCH, REAL_PITCH;

extern bool doCopyOfSurfaces;

void AttachSDLHandler(int eventType, bool (*func)(SDL_Event &));


class SDLPlatform : public IPlatform {

public:

    SDLPlatform(const char *title);

    virtual ~SDLPlatform();

    virtual void toggleFullscreen() override;

    virtual void antiFlicker(int surfNumber) override;

    virtual void updateScreen() override;

    virtual int processEvents() override;

    virtual const PixBuf *getPixBuf() override;

    virtual void releasePixBuf() override;
    //virtual void *getScrSurf() override { return this->scrSurf; }
    //void unlockRenderSurface() = 0;
    //void lockRenderSurface() = 0;

    //void render();

private:
    SDL_Surface *renderSurf;
    int renderSurfNum = 0;
    SDL_Surface *scrSurf[2];
};


#endif //ZEMU_SDL_PLATFORM_H
