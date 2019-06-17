#include <platform/platform.h>
#include "platform.h"

#include "zemu.h"

// TODO: Get rid of "params"

#if defined(__APPLE__)
#include <SDL_Thread.h>
#include <SDL_Mutex.h>
#endif

int actualWidth;
int actualHeight;

SDL_Surface *screen, *realScreen;

int PITCH, REAL_PITCH;

#if defined(__APPLE__)
SDL_Thread *updateScreenThread = nullptr;
SDL_sem *updateScreenThreadSem;
volatile bool updateScreenThreadActive = true;

int UpdateScreenThreadFunc(void *param);
#endif

struct s_SdlItem {
    int eventType;

    bool (*func)(SDL_Event &);
};

s_SdlItem hnd_sdl[MAX_HANDLERS];

int cnt_sdl = 0;

void AttachSDLHandler(int eventType, bool (*func)(SDL_Event &)) {
    if (cnt_sdl >= MAX_HANDLERS) StrikeError("Increase MAX_HANDLERS");

    s_SdlItem item;
    item.eventType = eventType;
    item.func = func;

    hnd_sdl[cnt_sdl++] = item;
}

#ifdef _WIN32

HICON windows_icon;
HWND hwnd;

#include <SDL_syswm.h>
#include "windows/resource.h"

void windows_init() {
    HINSTANCE handle = ::GetModuleHandle(nullptr);
    windows_icon = ::LoadIcon(handle, MAKEINTRESOURCE(IDI_ICON1));

    if (windows_icon == nullptr) {
        StrikeError("Error: %d\n", GetLastError());
    }

    SDL_SysWMinfo wminfo;
    SDL_VERSION(&wminfo.version);

    if (int sdl_error = SDL_GetWMInfo(&wminfo) != 1) {
        StrikeError("SDL_GetWMInfo() returned %d\n", sdl_error);
    }

    hwnd = wminfo.window;
    ::SetClassLongPtr(hwnd, GCLP_HICON, (LONG_PTR) windows_icon);
}

void windows_cleanup() {
    ::DestroyIcon(windows_icon);
}

#endif // _WIN32

#if defined(__APPLE__)
int UpdateScreenThreadFunc(void *param)
{
    while (updateScreenThreadActive) {
        SDL_SemWait(updateScreenThreadSem);

        if (params.useFlipSurface) SDL_Flip(realScreen);
        else SDL_UpdateRect(realScreen, 0, 0, 0, 0);
    }

    return 0;
}
#endif

SDLPlatform::SDLPlatform(uint32_t sdl_init_flags, const char *title) {

    if (SDL_Init(sdl_init_flags) < 0)
        StrikeError("Unable to init SDL: %s\n", SDL_GetError());

    videoSpec = SDL_SWSURFACE;
    if (params.fullscreen) videoSpec |= SDL_FULLSCREEN;
    if (params.useFlipSurface) videoSpec |= SDL_DOUBLEBUF;

    int actualWidth = WIDTH;
    int actualHeight = HEIGHT;

    if (params.scale2x) {
        actualWidth *= 2;
        actualHeight *= 2;
    }

    realScreen = SDL_SetVideoMode(actualWidth, actualHeight, 32, videoSpec);

    if (realScreen == nullptr)
        StrikeError("Unable to set requested video mode: %s\n", SDL_GetError());
    REAL_PITCH = realScreen->pitch / 4;

    if (params.scale2x) {
        SDL_PixelFormat *fmt = realScreen->format;

        screen = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, fmt->BitsPerPixel, fmt->Rmask,
                                      fmt->Gmask, fmt->Bmask, 0);
        if (screen == nullptr)
            StrikeError("Unable to create screen surface: %s\n", SDL_GetError());

        PITCH = screen->pitch / 4;
    } else {
        screen = realScreen;
        PITCH = REAL_PITCH;
    }

    SDL_WM_SetCaption(title, title);
    SDL_ShowCursor(SDL_DISABLE);


    SDL_PixelFormat *fmt = screen->format;

    this->scrSurf[0] = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, fmt->BitsPerPixel, fmt->Rmask,
                                            fmt->Gmask, fmt->Bmask, 0);
    if (this->scrSurf[0] == nullptr)
        StrikeError("Unable to create primary surface: %s\n", SDL_GetError());

    this->scrSurf[1] = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, fmt->BitsPerPixel, fmt->Rmask,
                                            fmt->Gmask, fmt->Bmask, 0);
    if (this->scrSurf[1] == nullptr)
        StrikeError("Unable to create secondary surface: %s\n", SDL_GetError());

#if defined(__APPLE__)
    updateScreenThreadSem = SDL_CreateSemaphore(0);
    updateScreenThread = SDL_CreateThread(UpdateScreenThreadFunc, nullptr);
#endif

#ifdef _WIN32
    windows_init();
#endif
}

SDLPlatform::~SDLPlatform() {
#if defined(__APPLE__)
    if (updateScreenThread) {
        updateScreenThreadActive = false;
        SDL_SemPost(updateScreenThreadSem);
        SDL_WaitThread(updateScreenThread, nullptr);
    }
#endif

    //// FIXME: These two lines (first commented out) were in zemu.cpp in between ^ and v
    // // TryFreeLongImage();
    // TryFreeAvgImage();

    if (videoSpec & SDL_FULLSCREEN) {
        videoSpec ^= SDL_FULLSCREEN;
        SDL_FreeSurface(realScreen);
        realScreen = SDL_SetVideoMode(actualWidth, actualHeight, 32, videoSpec);
    }

    SDL_FreeSurface(this->scrSurf[0]);
    SDL_FreeSurface(this->scrSurf[1]);
    SDL_FreeSurface(realScreen);

    if (params.scale2x) SDL_FreeSurface(screen);

#ifdef _WIN32
    windows_cleanup();
#endif
    SDL_Quit();
}

void SDLPlatform::toggleFullscreen() {
#ifndef _WIN32
    SDL_WM_ToggleFullScreen(realScreen);
#else
    videoSpec ^= SDL_FULLSCREEN;
    SDL_FreeSurface(realScreen);
    realScreen = SDL_SetVideoMode(actualWidth, actualHeight, 32, videoSpec);
    REAL_PITCH = realScreen->pitch / 4;

    if (!params.scale2x) {
        screen = realScreen;
        PITCH = REAL_PITCH;
    }
#endif
}

// FIXME: Make platform-neutral [?]
void SDLPlatform::antiFlicker(int surfNumber) {

    // AntiFlicker(renderSurf, scrSurf[sn]);
    // FIXME:
    SDL_Surface *copyFrom = renderSurf;
    SDL_Surface *copyTo = this->scrSurf[surfNumber];

    int i, j;
    uint8_t *s1, *s2, *sr;
    uint8_t *s1w, *s2w, *srw;

    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) return;
    }
    if (SDL_MUSTLOCK(this->scrSurf[0])) {
        if (SDL_LockSurface(this->scrSurf[0]) < 0) return;
    }
    if (SDL_MUSTLOCK(this->scrSurf[1])) {
        if (SDL_LockSurface(this->scrSurf[1]) < 0) return;
    }

    if (this->doCopyOfSurfaces) {
        s1 = (uint8_t *) copyFrom->pixels;
        s2 = (uint8_t *) copyTo->pixels;

        for (i = HEIGHT; i--;) {
            uint32_t *s1dw = (uint32_t *) s1;
            uint32_t *s2dw = (uint32_t *) s2;

            for (j = WIDTH; j--;) {
                *(s2dw++) = *(s1dw++);
            }

            s1 += copyFrom->pitch;
            s2 += copyTo->pitch;
        }

        this->doCopyOfSurfaces = false;
    }

    sr = (uint8_t *) screen->pixels;
    s1 = (uint8_t *) this->scrSurf[0]->pixels;
    s2 = (uint8_t *) this->scrSurf[1]->pixels;

    for (i = HEIGHT; i--;) {
        srw = sr;
        s1w = s1;
        s2w = s2;

        for (j = WIDTH; j--;) {
#if defined(ZEMU_BIG_ENDIAN) || defined(__APPLE__)
            *(srw++) = 0;
            s1w++;
            s2w++;
#endif

            *srw = (uint8_t) (((unsigned int) (*s1w) + (unsigned int) (*s2w)) >> 1);
            srw++;
            s1w++;
            s2w++;

            *srw = (uint8_t) (((unsigned int) (*s1w) + (unsigned int) (*s2w)) >> 1);
            srw++;
            s1w++;
            s2w++;

            *srw = (uint8_t) (((unsigned int) (*s1w) + (unsigned int) (*s2w)) >> 1);
            srw++;
            s1w++;
            s2w++;

#if !defined(ZEMU_BIG_ENDIAN) && !defined(__APPLE__)
            *(srw++) = 0;
            s1w++;
            s2w++;
#endif
        }

        sr += screen->pitch;
        s1 += this->scrSurf[0]->pitch;
        s2 += this->scrSurf[1]->pitch;
    }

    if (SDL_MUSTLOCK(this->scrSurf[1])) SDL_UnlockSurface(this->scrSurf[1]);
    if (SDL_MUSTLOCK(this->scrSurf[0])) SDL_UnlockSurface(this->scrSurf[0]);
    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
}

void SDLPlatform::updateScreen() {
    if (!params.scale2x) {
        // do not use threading here, because realScreen == screen

        if (params.useFlipSurface) SDL_Flip(realScreen);
        else SDL_UpdateRect(realScreen, 0, 0, 0, 0);

        return;
    }

#if defined(__APPLE__)
    if (SDL_SemValue(updateScreenThreadSem)) {
        return;
    }
#endif

    if (SDL_MUSTLOCK(realScreen)) {
        if (SDL_LockSurface(realScreen) < 0) return;
    }

    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            if (SDL_MUSTLOCK(realScreen)) SDL_UnlockSurface(realScreen);
            return;
        }
    }

    if (params.scanlines) {
        for (int i = HEIGHT - 1; i >= 0; i--) {
            int *line = (int *) screen->pixels + i * PITCH + WIDTH - 1;
            int *lineA = (int *) realScreen->pixels + (i * 2) * REAL_PITCH + (WIDTH * 2 - 1);
            int *lineB = (int *) realScreen->pixels + (i * 2 + 1) * REAL_PITCH + (WIDTH * 2 - 1);

            for (int j = WIDTH; j--;) {
                int c = *(line--);
                int dc = (c & 0xFEFEFE) >> 1;

                *(lineA--) = c;
                *(lineA--) = c;
                *(lineB--) = dc;
                *(lineB--) = dc;
            }
        }
    } else {
        for (int i = HEIGHT - 1; i >= 0; i--) {
            int *line = (int *) screen->pixels + i * PITCH + WIDTH - 1;
            int *lineA = (int *) realScreen->pixels + (i * 2) * REAL_PITCH + (WIDTH * 2 - 1);
            int *lineB = (int *) realScreen->pixels + (i * 2 + 1) * REAL_PITCH + (WIDTH * 2 - 1);

            for (int j = WIDTH; j--;) {
                int c = *(line--);

                *(lineA--) = c;
                *(lineA--) = c;
                *(lineB--) = c;
                *(lineB--) = c;
            }
        }
    }

    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
    if (SDL_MUSTLOCK(realScreen)) SDL_UnlockSurface(realScreen);

#if defined(__APPLE__)
    SDL_SemPost(updateScreenThreadSem);
#else
    if (params.useFlipSurface) SDL_Flip(realScreen);
    else SDL_UpdateRect(realScreen, 0, 0, 0, 0);
#endif
}

int SDLPlatform::processEvents() {

    int ev = 0; // Return event number
    int key;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return 1; // Exit

        if (event.type == SDL_KEYUP) {
            key = event.key.keysym.sym;
            if (key == SDLK_ESCAPE)
                ev = 2; // Raise quitMode
        }

        int i = cnt_sdl;
        s_SdlItem *ptr_sdl = hnd_sdl;

        while (i) {
            if (ptr_sdl->eventType == event.type) {
                if (ptr_sdl->func(event)) {
                    break;
                }
            }

            ptr_sdl++;
            i--;
        }
    }
    return ev;
}

const PixBuf *SDLPlatform::getPixBuf() {
    if (params.antiFlicker) {
        renderSurf = scrSurf[renderSurfNum];
        renderSurfNum = 1 - renderSurfNum;
    } else renderSurf = screen;

    if (renderActive && SDL_MUSTLOCK(renderSurf)) {
        if (SDL_LockSurface(renderSurf) < 0) {
            printf("Can't lock surface\n");
            return nullptr;
        }
    }
    pixBuf.pixels = (uint8_t *) (renderSurf->pixels);
    pixBuf.pitch = renderSurf->pitch;
    return &pixBuf;
}

void SDLPlatform::releasePixBuf() {
    if ((renderActive)) {
        if (SDL_MUSTLOCK(renderSurf))
            SDL_UnlockSurface(renderSurf);
        if (antiFlickerActive)
            antiFlicker(renderSurfNum); // FIXME: antiFlicker() should use this parameter directly.
    }
}
