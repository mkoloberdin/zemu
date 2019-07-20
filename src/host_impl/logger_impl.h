#ifndef HOST_IMPL__LOGGER_IMPL_H__INCLUDED
#define HOST_IMPL__LOGGER_IMPL_H__INCLUDED

#include "host/logger.h"

class LoggerImpl : public Logger {
public:

    LoggerImpl();

    void log(const char* fmt, ...);

private:

    char buffer[0x1000];
};

#endif
