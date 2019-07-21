#ifndef HOST__STAGE_H__INCLUDED
#define HOST__STAGE_H__INCLUDED

#include <cstdint>
#include <string>
#include <SDL_mouse.h>
#include "ZEmuConfig.h"
#include "keycode.h"

#if defined(USE_SDL1) && defined(__APPLE__)
    #define STAGE_MAKERGB(r, g, b) (((b) << 0x18) | ((g) << 0x10) | ((r) << 8))
    #define STAGE_GETR(c) (((c) >> 8) & 0xFF)
    #define STAGE_GETG(c) (((c) >> 0x10) & 0xFF)
    #define STAGE_GETB(c) ((c) >> 0x18)

    #define STAGE_FLIPPED_ARGB
#else
    #define STAGE_MAKERGB(r, g, b) (((r) << 0x10) | ((g) << 8) | (b))
    #define STAGE_GETR(c) ((c) >> 0x10)
    #define STAGE_GETG(c) (((c) >> 0x8) & 0xFF)
    #define STAGE_GETB(c) ((c) & 0xFF)

    #ifdef ZEMU_BIG_ENDIAN
        #define STAGE_FLIPPED_ARGB
    #endif
#endif

#define STAGE_MOUSE_LMASK SDL_BUTTON_LMASK
#define STAGE_MOUSE_RMASK SDL_BUTTON_MMASK
#define STAGE_MOUSE_MMASK SDL_BUTTON_RMASK

enum StageSoundDriver {
    STAGE_SOUND_DRIVER_NONE,
    STAGE_SOUND_DRIVER_GENERIC,
    STAGE_SOUND_DRIVER_WIN32,
    STAGE_SOUND_DRIVER_OSS
};

enum StageRenderMode {
    STAGE_RENDER_MODE_1X,
    STAGE_RENDER_MODE_2X,
    STAGE_RENDER_MODE_2X_SCANLINES
};

enum StageHint {
    STAGE_HINT_FLIP_SURFACE = 1 // Has effect only for SDL1
};

struct StageConfig {
    int hints = 0; // Implementation-specific
    std::string title;

    bool joystickEnabled = true;
    int joystickAxisThreshold = 3200;

    StageRenderMode renderMode = STAGE_RENDER_MODE_1X;
    int desiredFrameWidth = 0;
    int desiredFrameHeight = 0;
    bool fullscreen = false;

    StageSoundDriver soundDriver = STAGE_SOUND_DRIVER_GENERIC;
    bool soundEnabled = true;
    int soundFreq = 44100;
    int soundParams[3] = {0}; // Implementation-specific

    #ifdef _WIN32
        WORD windowsIconResource = 0;
    #endif
};

enum StageEventType {
    STAGE_EVENT_QUIT,
    STAGE_EVENT_KEYDOWN,
    STAGE_EVENT_KEYUP,
    STAGE_EVENT_JOYDOWN,
    STAGE_EVENT_JOYUP,
    STAGE_EVENT_MOUSEWHEEL
};

enum StageJoystickButton {
    STAGE_JOYSTICK_UP = 0,
    STAGE_JOYSTICK_DOWN = 1,
    STAGE_JOYSTICK_LEFT = 2,
    STAGE_JOYSTICK_RIGHT = 3,
    STAGE_JOYSTICK_FIRE = 4
};

struct StageEvent {
    StageEventType type;
    int keyCode;
    StageJoystickButton joyButton;
    int mouseWheelDirection;
};

struct StageMouseState {
    int x;
    int y;
    int buttons;
};

class Stage {
public:

    Stage() {}
    virtual ~Stage() {}

    virtual StageRenderMode getRenderMode() = 0;
    virtual void setRenderMode(StageRenderMode mode) = 0;

    virtual bool isKeyRepeat() = 0;
    virtual void setKeyRepeat(bool keyRepeat) = 0;

    virtual bool isFullscreen() = 0;
    virtual void setFullscreen(bool fullscreen) = 0;

    virtual bool isSoundEnabled() = 0;
    virtual void setSoundEnabled(bool soundEnabled) = 0;

    virtual bool pollEvent(StageEvent* into) = 0;
    virtual void getMouseState(StageMouseState* into) = 0;

    virtual void renderFrame(uint32_t* pixels, int width, int height) = 0; // In ARGB format
    virtual void renderSound(uint32_t* buffer, int samples) = 0; // 2 x int16_t (stereo) for each sample

private:

    Stage(const Stage&);
    Stage& operator=(const Stage&);
};

#endif
