#ifndef PLATFORM__LOGGER_H__INCLUDED
#define PLATFORM__LOGGER_H__INCLUDED

#include <memory>

class Logger {
public:

    Logger() {}
    virtual ~Logger() {}

    virtual void log(const char* fmt, ...) = 0;

private:

    Logger(const Logger&);
    Logger& operator=(const Logger&);
};

#endif
