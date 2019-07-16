#ifndef _PLATFORM_H_INCLUDED_
#define _PLATFORM_H_INCLUDED_

#include <ZEmuConfig.h>

#ifdef _WIN32
    #include <windows.h>

    #ifdef _MSC_VER
        #define strcasecmp _stricmp
        #define snprintf _snprintf
        #define vsnprintf _vsnprintf
    #endif
#else
    #include <limits.h>
    #include <unistd.h>
#endif

#ifndef MAX_PATH
    #ifdef PATH_MAX
        #define MAX_PATH PATH_MAX
    #else
        #define MAX_PATH 0x1000
    #endif
#endif

#endif
