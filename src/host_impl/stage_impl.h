#ifndef HOST_IMPL__STAGE_IMPL_H__INCLUDED
#define HOST_IMPL__STAGE_IMPL_H__INCLUDED

#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>
#include <map>
#include "ZEmuConfig.h"
#include "host/stage.h"
#include "host/logger.h"
#include "host_driver/sound_driver.h"

#ifdef _WIN32
    #include <windows.h>
#endif

class StageImpl : public Stage {
public:
    StageImpl(const StageConfig& stageConfig, Logger* logger);
    virtual ~StageImpl();

    StageRenderMode getRenderMode();
    void setRenderMode(StageRenderMode mode);

    bool isKeyRepeat();
    void setKeyRepeat(bool keyRepeat);

    bool isFullscreen();
    void setFullscreen(bool fullscreen);

    bool isSoundEnabled();
    void setSoundEnabled(bool soundEnabled);

    bool pollEvent(StageEvent* into);
    void getRelativeMouseState(StageMouseState* into);

    void renderFrame(uint32_t* pixels, int width, int height);
    void renderSound(uint32_t* buffer, int samples);

private:

    int hints;
    volatile StageRenderMode renderMode;
    volatile int lastFrameWidth;
    volatile int lastFrameHeight;
    bool keyRepeat = false;
    bool fullscreen;
    bool soundEnabled;
    bool joystickEnabled;
    int joystickAxisThreshold;
    int joystickPressedButtonsMask = 0;
    int joystickPendingButtonsMask = 0;
    bool isMouseGrabbed = false;
    SDL_Surface* nativeSurface = nullptr;
    SDL_Event nativeEvent;

    volatile bool isRenderThreadActive = true;
    volatile bool isRenderThreadPixelsConsumed = true;
    SDL_sem* renderThreadPixelsReadySem = nullptr;
    SDL_Thread* renderThread = nullptr;
    uint32_t* volatile renderThreadPixels = nullptr;

    std::unique_ptr<SoundDriver> soundDriver;

    #ifdef USE_SDL1
        std::map<int, SDL_Joystick*> openedJoysticksMap;
    #else
        std::map<SDL_JoystickID, SDL_Joystick*> openedJoysticksMap;
    #endif

    #ifndef USE_SDL1
        std::string stageTitle;
        SDL_Window* nativeWindow = nullptr;
        SDL_Renderer* nativeRenderer = nullptr;
        SDL_Texture* nativeTexture = nullptr;
    #endif

    #ifdef _WIN32
        HICON windowsIcon = nullptr;
    #endif

    bool processPendingJoystickButtons(StageEvent* into);
    bool processPendingSingleJoystickButton(StageEvent* into, StageJoystickButton joyButton);
    void refreshVideoSubsystem();
    void renderThreadLoop();
    void renderThreadUpdateSurface1x();
    void renderThreadUpdateSurface2x();
    void renderThreadUpdateSurface2xScanlines();

    friend int stageImplRenderThreadFunction(void* data);
};

#endif
