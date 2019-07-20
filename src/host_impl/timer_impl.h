#ifndef HOST_IMPL__TIMER_IMPL_H__INCLUDED
#define HOST_IMPL__TIMER_IMPL_H__INCLUDED

#include "host/timer.h"

class TimerImpl : public Timer {
public:

    uint32_t getElapsedMillis();
    void wait(uint32_t millis);
};

#endif
