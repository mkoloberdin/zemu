#ifndef _PLATFORM_HARDWARE_H_INCLUDED_
#define _PLATFORM_HARDWARE_H_INCLUDED_

#include <ZEmuConfig.h>
#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>

#ifdef _WIN32
    #include <SDL_syswm.h>
#endif

/*
 * Macro
 */

#define ZHW_VERSION_FILL SDL_VERSION

#define ZHW_VIDEO_LOCKSURFACE(surface) (!SDL_MUSTLOCK(surface) || SDL_LockSurface(surface) >= 0)
#define ZHW_VIDEO_UNLOCKSURFACE(surface) if (SDL_MUSTLOCK(surface)) { SDL_UnlockSurface(surface); }

#ifdef USE_SDL1
    #define ZHW_EVENT_WHEELDIRECTION(event) (event.button.button)
    #define ZHW_EVENT_OKKEY(window, event) (1)

    #ifdef __APPLE__
        #define ZHW_VIDEO_MAKERGB(r, g, b) (((b) << 0x18) | ((g) << 0x10) | ((r) << 8))
        #define ZHW_VIDEO_GETR(c) (((c) >> 8) & 0xFF)
        #define ZHW_VIDEO_GETG(c) (((c) >> 0x10) & 0xFF)
        #define ZHW_VIDEO_GETB(c) ((c) >> 0x18)
    #else
        #define ZHW_VIDEO_MAKERGB(r, g, b) (((r) << 0x10) | ((g) << 8) | (b))
        #define ZHW_VIDEO_GETR(c) ((c) >> 0x10)
        #define ZHW_VIDEO_GETG(c) (((c) >> 8) & 0xFF)
        #define ZHW_VIDEO_GETB(c) ((c) & 0xFF)
    #endif
#else
    #define ZHW_EVENT_WHEELDIRECTION(event) (event.wheel.direction)
    #define ZHW_EVENT_OKKEY(window, event) ((window)->isKeyRepeatEnabled || (event).key.repeat == 0)

    #define ZHW_VIDEO_MAKERGB(r, g, b) (((r) << 0x10) | ((g) << 8) | (b))
    #define ZHW_VIDEO_GETR(c) ((c) >> 0x10)
    #define ZHW_VIDEO_GETG(c) (((c) >> 0x8) & 0xFF)
    #define ZHW_VIDEO_GETB(c) ((c) & 0xFF)
#endif

/*
 * Structures and types
 */

#ifdef USE_SDL1
    typedef struct {
        int videoSpec;
        bool useFlipSurface;
        SDL_Surface *surface;
    } ZHW_Window;
#else
    typedef struct {
        SDL_Window *nativeWindow;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Surface *surface;
        bool isKeyRepeatEnabled;
    } ZHW_Window;
#endif

#define ZHW_Audio_Spec SDL_AudioSpec
#define ZHW_Event SDL_Event
#define ZHW_Joystick SDL_Joystick
#define ZHW_Mutex_Sem SDL_sem
#define ZHW_Thread SDL_Thread
#define ZHW_Video_PixelFormat SDL_PixelFormat
#define ZHW_Video_Rect SDL_Rect
#define ZHW_Video_Surface SDL_Surface

#ifdef USE_SDL1
    typedef int ZHW_Keyboard_KeyCode;
#else
    typedef SDL_Keycode ZHW_Keyboard_KeyCode;
#endif

#ifdef _WIN32
    #define ZHW_SysWm_Info SDL_SysWMinfo
#endif

/*
 * Functions
 */

#define ZHW_Audio_Close SDL_CloseAudio
#define ZHW_Audio_Lock SDL_LockAudio
#define ZHW_Audio_Open SDL_OpenAudio
#define ZHW_Audio_Pause SDL_PauseAudio
#define ZHW_Audio_Unlock SDL_UnlockAudio
#define ZHW_Core_Quit SDL_Quit
#define ZHW_Error_Get SDL_GetError
#define ZHW_Event_Poll SDL_PollEvent
#define ZHW_Joystick_EventState SDL_JoystickEventState
#define ZHW_Joystick_GetNum SDL_NumJoysticks
#define ZHW_Joystick_Open SDL_JoystickOpen
#define ZHW_Moyse_GetRelativeState SDL_GetRelativeMouseState
#define ZHW_Mutex_CreateSemaphore SDL_CreateSemaphore
#define ZHW_Mutex_SemPost SDL_SemPost
#define ZHW_Mutex_SemValue SDL_SemValue
#define ZHW_Mutex_SemWait SDL_SemWait
#define ZHW_Thread_Wait SDL_WaitThread
#define ZHW_Timer_Delay SDL_Delay
#define ZHW_Timer_GetTicks SDL_GetTicks
#define ZHW_Video_BlitSurface SDL_BlitSurface
#define ZHW_Video_FillRect SDL_FillRect
#define ZHW_Video_FreeSurface SDL_FreeSurface

#define ZHW_Video_CreateSurface(width, height, baseSurface) SDL_CreateRGBSurface(SDL_SWSURFACE, \
    (width), \
    (height), \
    (baseSurface)->format->BitsPerPixel, \
    (baseSurface)->format->Rmask, \
    (baseSurface)->format->Gmask, \
    (baseSurface)->format->Bmask, \
    0)

#ifdef USE_SDL1
    #define ZHW_Thread_Create SDL_CreateThread
    #define ZHW_Video_SetColorKey(surface, key) SDL_SetColorKey((surface), SDL_SRCCOLORKEY, (key))

    #ifdef _WIN32
        #define ZHW_SysWm_GetInfo(window, info) SDL_GetWMInfo(info)
    #endif
#else
    #define ZHW_Thread_Create(fn, data) SDL_CreateThread((fn), NULL, (data))
    #define ZHW_Video_SetColorKey(surface, key) SDL_SetColorKey((surface), 1, (key))

    #ifdef _WIN32
        #define ZHW_SysWm_GetInfo SDL_GetWindowWMInfo
    #endif
#endif

int ZHW_Core_Init(bool withAudio);
int ZHW_Joystick_Init();
ZHW_Window* ZHW_Video_CreateWindow(const char *title, int width, int height, bool isFullscreen, bool useFlipSurface);
void ZHW_Video_ToggleFullScreen(ZHW_Window *window);
void ZHW_Video_BlitWindow(ZHW_Window *window);
void ZHW_Video_CloseWindow(ZHW_Window *window);
void ZHW_Keyboard_EnableKeyRepeat(ZHW_Window *window);
void ZHW_Keyboard_DisableKeyRepeat(ZHW_Window *window);

/*
 * Constants
 */

#define ZHW_EVENT_JOYAXISMOTION SDL_JOYAXISMOTION
#define ZHW_EVENT_JOYBUTTONDOWN SDL_JOYBUTTONDOWN
#define ZHW_EVENT_JOYBUTTONUP SDL_JOYBUTTONUP
#define ZHW_EVENT_KEYDOWN SDL_KEYDOWN
#define ZHW_EVENT_KEYUP SDL_KEYUP
#define ZHW_EVENT_QUIT SDL_QUIT
#define ZHW_MOUSE_LMASK SDL_BUTTON_LMASK
#define ZHW_MOUSE_MMASK SDL_BUTTON_MMASK
#define ZHW_MOUSE_RMASK SDL_BUTTON_RMASK

#ifdef USE_SDL1
    #define ZHW_EVENT_MOUSEWHEEL SDL_MOUSEBUTTONDOWN
    #define ZHW_MOUSE_WHEELDOWN SDL_BUTTON_WHEELDOWN
    #define ZHW_MOUSE_WHEELUP SDL_BUTTON_WHEELUP
#else
    #define ZHW_EVENT_MOUSEWHEEL SDL_MOUSEWHEEL
    #define ZHW_MOUSE_WHEELDOWN SDL_MOUSEWHEEL_NORMAL
    #define ZHW_MOUSE_WHEELUP SDL_MOUSEWHEEL_FLIPPED
#endif

/*
 * Keyboard constants
 */

#define ZHW_KEY_ESCAPE SDLK_ESCAPE
#define ZHW_KEY_F1 SDLK_F1
#define ZHW_KEY_F2 SDLK_F2
#define ZHW_KEY_F3 SDLK_F3
#define ZHW_KEY_F4 SDLK_F4
#define ZHW_KEY_F5 SDLK_F5
#define ZHW_KEY_F6 SDLK_F6
#define ZHW_KEY_F7 SDLK_F7
#define ZHW_KEY_F8 SDLK_F8
#define ZHW_KEY_F9 SDLK_F9
#define ZHW_KEY_F10 SDLK_F10
#define ZHW_KEY_F11 SDLK_F11
#define ZHW_KEY_F12 SDLK_F12
#define ZHW_KEY_PAUSE SDLK_PAUSE
#define ZHW_KEY_INSERT SDLK_INSERT
#define ZHW_KEY_HOME SDLK_HOME
#define ZHW_KEY_DELETE SDLK_DELETE
#define ZHW_KEY_END SDLK_END
#define ZHW_KEY_PAGEUP SDLK_PAGEUP
#define ZHW_KEY_PAGEDOWN SDLK_PAGEDOWN
#define ZHW_KEY_1 SDLK_1
#define ZHW_KEY_2 SDLK_2
#define ZHW_KEY_3 SDLK_3
#define ZHW_KEY_4 SDLK_4
#define ZHW_KEY_5 SDLK_5
#define ZHW_KEY_6 SDLK_6
#define ZHW_KEY_7 SDLK_7
#define ZHW_KEY_8 SDLK_8
#define ZHW_KEY_9 SDLK_9
#define ZHW_KEY_0 SDLK_0
#define ZHW_KEY_BACKSPACE SDLK_BACKSPACE
#define ZHW_KEY_TAB SDLK_TAB
#define ZHW_KEY_q SDLK_q
#define ZHW_KEY_w SDLK_w
#define ZHW_KEY_e SDLK_e
#define ZHW_KEY_r SDLK_r
#define ZHW_KEY_t SDLK_t
#define ZHW_KEY_y SDLK_y
#define ZHW_KEY_u SDLK_u
#define ZHW_KEY_i SDLK_i
#define ZHW_KEY_o SDLK_o
#define ZHW_KEY_p SDLK_p
#define ZHW_KEY_RETURN SDLK_RETURN
#define ZHW_KEY_CAPSLOCK SDLK_CAPSLOCK
#define ZHW_KEY_a SDLK_a
#define ZHW_KEY_s SDLK_s
#define ZHW_KEY_d SDLK_d
#define ZHW_KEY_f SDLK_f
#define ZHW_KEY_g SDLK_g
#define ZHW_KEY_h SDLK_h
#define ZHW_KEY_j SDLK_j
#define ZHW_KEY_k SDLK_k
#define ZHW_KEY_l SDLK_l
#define ZHW_KEY_LSHIFT SDLK_LSHIFT
#define ZHW_KEY_z SDLK_z
#define ZHW_KEY_x SDLK_x
#define ZHW_KEY_c SDLK_c
#define ZHW_KEY_v SDLK_v
#define ZHW_KEY_b SDLK_b
#define ZHW_KEY_n SDLK_n
#define ZHW_KEY_m SDLK_m
#define ZHW_KEY_RSHIFT SDLK_RSHIFT
#define ZHW_KEY_LCTRL SDLK_LCTRL
#define ZHW_KEY_LALT SDLK_LALT
#define ZHW_KEY_SPACE SDLK_SPACE
#define ZHW_KEY_RALT SDLK_RALT
#define ZHW_KEY_RCTRL SDLK_RCTRL
#define ZHW_KEY_UP SDLK_UP
#define ZHW_KEY_DOWN SDLK_DOWN
#define ZHW_KEY_LEFT SDLK_LEFT
#define ZHW_KEY_RIGHT SDLK_RIGHT

#ifdef USE_SDL1
    #define ZHW_KEY_LSUPER SDLK_LSUPER
    #define ZHW_KEY_RSUPER SDLK_RSUPER
    #define ZHW_KEY_NUMLOCK SDLK_NUMLOCK
#else
    #define ZHW_KEY_LSUPER SDLK_LGUI
    #define ZHW_KEY_RSUPER SDLK_RGUI
    #define ZHW_KEY_NUMLOCK SDLK_NUMLOCKCLEAR
#endif

#endif
