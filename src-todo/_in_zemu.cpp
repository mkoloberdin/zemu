// int PITCH;
// int REAL_PITCH;
//
// ZHW_Window* window;
// int actualWidth;
// int actualHeight;

// ----

// void Action_Fullscreen(void) {
//     ZHW_Video_ToggleFullScreen(window);
//
//     realScreen = window->surface;
//     REAL_PITCH = realScreen->pitch / 4;
//
//     if (!params.scale2x) {
//         screen = realScreen;
//         PITCH = REAL_PITCH;
//     }
// }

// ----

// at main

//         if (ZHW_Core_Init(params.sound && params.sndBackend == SND_BACKEND_DEFAULT) < 0) {
//             StrikeError("Unable to init: %s\n", ZHW_Error_Get());
//         }

//         #ifdef _WIN32
//             windows_init();
//             atexit(windows_cleanup);
//         #else
            atexit(ZHW_Core_Quit);
//         #endif

//        int actualWidth = WIDTH;
//        int actualHeight = HEIGHT;
//
//        if (params.scale2x) {
//            actualWidth *= 2;
//            actualHeight *= 2;
//        }

//        window = ZHW_Video_CreateWindow("ZEmu", actualWidth, actualHeight, params.fullscreen, params.useFlipSurface);

//        if (window == nullptr) {
//            StrikeError("Unable to create window: %s\n", ZHW_Error_Get());
//        }

//         realScreen = window->surface;
//         REAL_PITCH = realScreen->pitch / 4;
//
//         if (params.scale2x) {
//             screen = ZHW_Video_CreateSurface(WIDTH, HEIGHT, realScreen);
//
//             if (screen == nullptr) {
//                 StrikeError("Unable to create screen surface: %s\n", ZHW_Error_Get());
//             }
//
//             PITCH = screen->pitch / 4;
//         } else {
//             screen = realScreen;
//             PITCH = REAL_PITCH;
//         }

//        C_JoystickManager::Instance()->Init();

// ----

// at FreeAll

//    if (upadteScreenThread) {
//        updateScreenThreadActive = false;
//        ZHW_Mutex_SemPost(updateScreenThreadSem);
//        ZHW_Thread_Wait(upadteScreenThread, nullptr);
//    }

//    if (params.scale2x) {
//        ZHW_Video_FreeSurface(screen);
//    }
//
//    ZHW_Video_CloseWindow(window);

// ----

void InitAudio(void) {
    if (params.sndBackend == SND_BACKEND_DEFAULT) {
        soundMixer.InitBackendDefault(params.audioBufferSize);
    }
    #ifdef _WIN32
        else if (params.sndBackend == SND_BACKEND_WIN32) {
            soundMixer.InitBackendWin32(params.soundParam);
        }
    #endif
    #ifdef __unix__
        else if (params.sndBackend == SND_BACKEND_OSS) {
            soundMixer.InitBackendOSS(params.soundParam);
        }
    #endif

    soundMixer.Init(params.mixerMode, recordWav, wavFileName);
}

// ----

// ZHW_Thread *upadteScreenThread = nullptr;
// ZHW_Mutex_Sem *updateScreenThreadSem;
// volatile bool updateScreenThreadActive = true;
// int UpdateScreenThreadFunc(void *param);

// ----

// void InitSurfaces(void) {
//     scrSurf[0] = ZHW_Video_CreateSurface(WIDTH, HEIGHT, screen);
//
//     if (scrSurf[0] == nullptr) {
//         StrikeError("Unable to create primary surface: %s\n", ZHW_Error_Get());
//     }
//
//     scrSurf[1] = ZHW_Video_CreateSurface(WIDTH, HEIGHT, screen);
//
//     if (scrSurf[1] == nullptr) {
//         StrikeError("Unable to create secondary surface: %s\n", ZHW_Error_Get());
//     }
// }

// ----

// in InitAll

//    updateScreenThreadSem = ZHW_Mutex_CreateSemaphore(0);
//    upadteScreenThread = ZHW_Thread_Create(UpdateScreenThreadFunc, nullptr);

// ----

// int UpdateScreenThreadFunc(void* param) {
//     while (updateScreenThreadActive) {
//         ZHW_Mutex_SemWait(updateScreenThreadSem);
//         ZHW_Video_BlitWindow(window);
//     }
//
//     return 0;
// }

// // Tries to update screen.
// // If screen is already updating, do nothing, therefore this function must be called in a loop,
// // to ensure that screen is really updated
// void UpdateScreen(void) {
//     if (!params.scale2x) {
//         // do not use threading here, because realScreen == screen
//         ZHW_Video_BlitWindow(window);
//         return;
//     }
//
//     if (ZHW_Mutex_SemValue(updateScreenThreadSem)) {
//         return;
//     }
//
//     if (!ZHW_VIDEO_LOCKSURFACE(realScreen)) {
//         return;
//     }
//
//     if (!ZHW_VIDEO_LOCKSURFACE(screen)) {
//         ZHW_VIDEO_UNLOCKSURFACE(realScreen);
//         return;
//     }
//
//     if (params.scanlines) {
//         for (int i = HEIGHT - 1; i >= 0; i--) {
//             int* line = (int*)screen->pixels + i * PITCH + WIDTH-1;
//             int* lineA = (int*)realScreen->pixels + (i * 2) * REAL_PITCH + (WIDTH * 2 - 1);
//             int* lineB = (int*)realScreen->pixels + (i * 2 + 1) * REAL_PITCH + (WIDTH * 2 - 1);
//
//             for (int j = WIDTH; j--;) {
//                 int c = *(line--);
//                 int dc = (c & 0xFEFEFE) >> 1;
//
//                 *(lineA--) = c;
//                 *(lineA--) = c;
//                 *(lineB--) = dc;
//                 *(lineB--) = dc;
//             }
//         }
//     } else {
//         for (int i = HEIGHT - 1; i >= 0; i--) {
//             int* line = (int*)screen->pixels + i * PITCH + WIDTH-1;
//             int* lineA = (int*)realScreen->pixels + (i * 2) * REAL_PITCH + (WIDTH * 2 - 1);
//             int* lineB = (int*)realScreen->pixels + (i * 2 + 1) * REAL_PITCH + (WIDTH * 2 - 1);
//
//             for (int j = WIDTH; j--;) {
//                 int c = *(line--);
//
//                 *(lineA--) = c;
//                 *(lineA--) = c;
//                 *(lineB--) = c;
//                 *(lineB--) = c;
//             }
//         }
//     }
//
//     ZHW_VIDEO_UNLOCKSURFACE(screen);
//     ZHW_VIDEO_UNLOCKSURFACE(realScreen);
//
//     ZHW_Mutex_SemPost(updateScreenThreadSem);
// }

// ----

#ifdef _WIN32

// HICON windows_icon;
// HWND hwnd;

#include "windows/resource.h"

// void windows_init() {
//     HINSTANCE handle = ::GetModuleHandle(nullptr);
//     windows_icon = ::LoadIcon(handle, MAKEINTRESOURCE(IDI_ICON1));
//
//     if (windows_icon == nullptr) {
//         StrikeError("Error: %d\n", GetLastError());
//     }
//
//     ZHW_SysWm_Info wminfo;
//     ZHW_VERSION_FILL(&wminfo.version);
//
//     if (int wmerror = ZHW_SysWm_GetInfo(&wminfo) != 1) {
//         StrikeError("ZHW_SysWm_GetInfo() returned %d\n", wmerror);
//     }
//
//     hwnd = wminfo.window;
//     ::SetClassLongPtr(hwnd, GCLP_HICON, (LONG_PTR)windows_icon);
// }

// void windows_cleanup() {
//     ZHW_Core_Quit();
//     ::DestroyIcon(windows_icon);
// }

#endif // _WIN32
