#ifndef PLATFORM__HARDWARE_H__INCLUDED
#define PLATFORM__HARDWARE_H__INCLUDED

#include <cstdint>
#include <SDL_mouse.h>
#include "hardware_keycode.h"

#define HW_MAKERGB(r, g, b) (((r) << 0x10) | ((g) << 8) | (b))
#define HW_GETR(c) ((c) >> 0x10)
#define HW_GETG(c) (((c) >> 0x8) & 0xFF)
#define HW_GETB(c) ((c) & 0xFF)

#define HW_MOUSE_LMASK SDL_BUTTON_LMASK
#define HW_MOUSE_RMASK SDL_BUTTON_MMASK
#define HW_MOUSE_MMASK SDL_BUTTON_RMASK

enum HardwareSoundDriver {
    HW_SOUND_DRIVER_NONE,
    HW_SOUND_DRIVER_GENERIC,
    HW_SOUND_DRIVER_NATIVE
};

enum HardwareRenderMode {
    HW_RENDER_MODE_1X,
    HW_RENDER_MODE_2X,
    HW_RENDER_MODE_2X_SCANLINES
};

struct HardwareConfig {
    int initialWidth;
    int initialHeight;
    HardwareSoundDriver soundDriver;
    HardwareRenderMode renderMode;
    bool isFullscreen;
};

enum HardwareEventType {
    HW_EVENT_QUIT,
    HW_EVENT_KEYDOWN,
    HW_EVENT_KEYUP,
    HW_EVENT_JOYDOWN,
    HW_EVENT_JOYUP,
    HW_EVENT_MOUSEWHEEL
};

enum HardwareJoystickButton {
    HW_JOYSTICK_UP,
    HW_JOYSTICK_DOWN,
    HW_JOYSTICK_LEFT,
    HW_JOYSTICK_RIGHT,
    HW_JOYSTICK_FIRE
};

struct HardwareEvent {
    HardwareEventType type;
    int keyCode;
    HardwareJoystickButton joyButton;
    int mouseWheelDirection;
};

struct HardwareMouseState {
    int x;
    int y;
    int buttons;
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

    virtual bool pollEvent(HardwareEvent* into) = 0;

    virtual void renderFrame(uint32_t* pixels, int width, int height) = 0; // In ARGB format
    virtual void renderSound(uint32_t* buffer, int samples) = 0; // 2 x int16_t (stereo) for each sample

    virtual uint32_t getElapsedMillis() = 0;
    virtual void delay(uint32_t millis) = 0;

    virtual void getMouseState(HardwareMouseState* mouseState) = 0;

private:

    Hardware(const Hardware&);
    Hardware& operator=(const Hardware&);
};

#endif
