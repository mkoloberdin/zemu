#ifndef _PLATFORM_H_INCLUDED_
#define _PLATFORM_H_INCLUDED_

#include <ZEmuConfig.h>

#ifdef _WIN32
    #include <windows.h>

    #ifdef _MSC_VER
        #define strcasecmp _stricmp
        #define stricmp _stricmp
        #define snprintf _snprintf
        #define vsnprintf _vsnprintf
    #endif
#endif

// #define AUTO_DEHRUST

#endif
