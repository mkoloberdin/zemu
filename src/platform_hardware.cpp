#include "platform_hardware.h"

int ZHW_Core_Init(bool withAudio) {
    int spec = SDL_INIT_VIDEO;

    if (withAudio) {
        spec |= SDL_INIT_AUDIO;
    }

    return SDL_Init(spec);
}

int ZHW_Joystick_Init() {
    int result = SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    if (result >= 0) {
        SDL_JoystickEventState(SDL_ENABLE);
    }

    return result;
}

int ZHW_Video_CreateSpec(bool isFullscreen, bool useFlipSurface) {
    int spec = SDL_SWSURFACE;

    if (isFullscreen) {
        spec |= SDL_FULLSCREEN;
    }

    if (useFlipSurface) {
        spec |= SDL_DOUBLEBUF;
    }

    return spec;
}

ZHW_Video_Surface* ZHW_Video_CreateRenderSurface(int width, int height, int videoSpec, const char *title) {
    SDL_Surface* surface = SDL_SetVideoMode(width, height, 32, videoSpec);

    if (surface != NULL) {
        SDL_WM_SetCaption(title, title);
        SDL_ShowCursor(SDL_DISABLE);
    }

    return surface;
}

ZHW_Video_Surface* ZHW_Video_ToggleFullScreen(ZHW_Video_Surface *surface, int *videoSpec) {
    #if defined(_WIN32) || defined(__APPLE__)
        *videoSpec ^= SDL_FULLSCREEN;
        SDL_FreeSurface(surface);
        return SDL_SetVideoMode(surface->w, surface->h, 32, *videoSpec);
    #else
        SDL_WM_ToggleFullScreen(surface);
        return surface;
    #endif
}

void ZHW_Video_BlitRenderSurface(ZHW_Video_Surface *surface, bool useFlipSurface) {
    if (useFlipSurface) {
        SDL_Flip(surface);
    } else {
        SDL_UpdateRect(surface, 0, 0, 0, 0);
    }
}

void ZHW_Keyboard_EnableKeyRepeat() {
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void ZHW_Keyboard_DisableKeyRepeat() {
    SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);
}
