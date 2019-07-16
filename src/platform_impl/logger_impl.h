#ifndef PLATFORM_IMPL__LOGGER_IMPL_H__INCLUDED
#define PLATFORM_IMPL__LOGGER_IMPL_H__INCLUDED

#include "platform/logger.h"

class LoggerImpl : public Logger {
    public:

    LoggerImpl();

    void log(const char* fmt, ...);

    private:

    char buffer[0x1000];
};

#endif
