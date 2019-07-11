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

#ifdef USE_SDL1
    ZHW_Window* ZHW_Video_CreateWindow(const char* title, int width, int height, bool isFullscreen, bool useFlipSurface) {
        int videoSpec = SDL_SWSURFACE;

        if (isFullscreen) {
            videoSpec |= SDL_FULLSCREEN;
        }

        if (useFlipSurface) {
            videoSpec |= SDL_DOUBLEBUF;
        }

        SDL_Surface* surface = SDL_SetVideoMode(width, height, 32, videoSpec);

        if (!surface) {
            return nullptr;
        }

        ZHW_Window* window = new ZHW_Window;

        window->videoSpec = videoSpec;
        window->useFlipSurface = useFlipSurface;
        window->surface = surface;

        SDL_WM_SetCaption(title, title);
        SDL_ShowCursor(SDL_DISABLE);

        return window;
    }

    void ZHW_Video_ToggleFullScreen(ZHW_Window* window) {
        window->videoSpec ^= SDL_FULLSCREEN;

        if (SDL_WM_ToggleFullScreen(window->surface)) {
            return;
        }

        int width = window->surface->w;
        int height = window->surface->h;

        SDL_FreeSurface(window->surface);
        window->surface = SDL_SetVideoMode(width, height, 32, window->videoSpec);
    }

    void ZHW_Video_BlitWindow(ZHW_Window* window) {
        if (window->useFlipSurface) {
            SDL_Flip(window->surface);
        } else {
            SDL_UpdateRect(window->surface, 0, 0, 0, 0);
        }
    }

    void ZHW_Video_CloseWindow(ZHW_Window* window) {
        if (window->videoSpec & SDL_FULLSCREEN) {
            ZHW_Video_ToggleFullScreen(window);
        }

        SDL_FreeSurface(window->surface);
        delete window;
    }

    void ZHW_Keyboard_EnableKeyRepeat(ZHW_Window* window) {
        SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    }

    void ZHW_Keyboard_DisableKeyRepeat(ZHW_Window* window) {
        SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);
    }
#else
    ZHW_Window* ZHW_Video_CreateWindow(const char* title, int width, int height, bool isFullscreen, bool useFlipSurface) {
        int flags = 0;

        if (isFullscreen) {
            flags |= SDL_WINDOW_FULLSCREEN;
        }

        SDL_Window* nativeWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

        if (!nativeWindow) {
            return nullptr;
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(nativeWindow, -1, 0);

        if (!renderer) {
            SDL_DestroyWindow(nativeWindow);
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

        if (!texture) {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(nativeWindow);
            return nullptr;
        }

        SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

        if (!surface) {
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(nativeWindow);
            return nullptr;
        }

        ZHW_Window* window = new ZHW_Window;

        window->nativeWindow = nativeWindow;
        window->renderer = renderer;
        window->texture = texture;
        window->surface = surface;
        window->isKeyRepeatEnabled = false;

        SDL_ShowCursor(SDL_DISABLE);
        return window;
    }

    void ZHW_Video_ToggleFullScreen(ZHW_Window* window) {
        SDL_SetWindowFullscreen(
            window->nativeWindow,
            (SDL_GetWindowFlags(window->nativeWindow) & SDL_WINDOW_FULLSCREEN) ? 0 : SDL_WINDOW_FULLSCREEN
        );
    }

    void ZHW_Video_BlitWindow(ZHW_Window* window) {
        if (SDL_MUSTLOCK(window->surface) && SDL_LockSurface(window->surface) < 0) {
            return;
        }

        SDL_UpdateTexture(window->texture, nullptr, window->surface->pixels, window->surface->pitch);

        if (SDL_MUSTLOCK(window->surface)) {
            SDL_UnlockSurface(window->surface);
        }

        SDL_RenderClear(window->renderer);
        SDL_RenderCopy(window->renderer, window->texture, nullptr, nullptr);
        SDL_RenderPresent(window->renderer);
    }

    void ZHW_Video_CloseWindow(ZHW_Window* window) {
        if (SDL_GetWindowFlags(window->nativeWindow) & SDL_WINDOW_FULLSCREEN) {
            SDL_SetWindowFullscreen(window->nativeWindow, 0);
        }

        SDL_FreeSurface(window->surface);
        SDL_DestroyTexture(window->texture);
        SDL_DestroyRenderer(window->renderer);
        SDL_DestroyWindow(window->nativeWindow);

        delete window;
    }

    void ZHW_Keyboard_EnableKeyRepeat(ZHW_Window* window) {
        window->isKeyRepeatEnabled = true;
    }

    void ZHW_Keyboard_DisableKeyRepeat(ZHW_Window* window) {
        window->isKeyRepeatEnabled = false;
    }
#endif
