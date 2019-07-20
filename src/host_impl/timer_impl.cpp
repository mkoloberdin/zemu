// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SDL_timer.h>
#include "timer_impl.h"

uint32_t TimerImpl::getElapsedMillis() {
    return SDL_GetTicks();
}

void TimerImpl::wait(uint32_t millis) {
    SDL_Delay(millis);
}
