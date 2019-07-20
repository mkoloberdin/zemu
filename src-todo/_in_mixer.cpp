// #include "snd_backend.h"
// #include "snd_backend_default.h"
// #include "snd_backend_oss.h"
// #include "snd_backend_win32.h"
//
// #define AUDIO_NATIVE_BUFFER 1024
// CSndBackend* sndBackend = nullptr;
//
// ----
//
// void C_SoundMixer::InitBackendDefault(int bufferSize) {
//     sndBackend = new CSndBackendDefault(SND_CALLBACK_BUFFER_SIZE * bufferSize);
//     sndBackend->Init();
// }
//
// #ifdef _WIN32
//     void C_SoundMixer::InitBackendWin32(int soundParam) {
//         sndBackend = new CSndBackendWIN32(soundParam, AUDIO_NATIVE_BUFFER);
//         sndBackend->Init();
//     }
// #endif
//
// #ifdef __unix__
//     void C_SoundMixer::InitBackendOSS(int soundParam) {
//         sndBackend = new CSndBackendOSS(soundParam, AUDIO_NATIVE_BUFFER);
//         sndBackend->Init();
//     }
// #endif
