// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include "logger_impl.h"

LoggerImpl::LoggerImpl() {
    buffer[0] = '\0';
}

void LoggerImpl::log(const char* fmt, ...) {
    va_list argPtr;

    va_start(argPtr, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, argPtr);
    va_end(argPtr);

    std::cout << buffer << std::endl;
}
