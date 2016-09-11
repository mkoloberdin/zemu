#ifndef _PLATFORM_H_INCLUDED_
#define _PLATFORM_H_INCLUDED_

#include <ZEmuConfig.h>

// Platform-specific includes and defines

//---------------
// SDL Predefined
//---------------

// FIXME: WORD_SWAP_BYTES - unused. Remove?
#ifdef ZEMU_BIG_ENDIAN
  #define WORD_SWAP_BYTES(a) ((((a)&0xFF)<<8)|((a)>>8))
#endif

#ifdef __unix__
  #include <limits.h>
#endif

#ifdef _WIN32
//---------------
// WIN32-specific
//---------------
#include <windows.h>

#ifdef _MSC_VER
  #define strcasecmp _stricmp
  #define stricmp _stricmp
  #define snprintf _snprintf
  #define vsnprintf _vsnprintf
#endif // _MSC_VER

#endif // _WIN32

#ifndef MAX_PATH  // defined on Windows
  #ifdef PATH_MAX // defined on Unix
    #define MAX_PATH PATH_MAX
  #else
    #define MAX_PATH 0x1000
  #endif
#endif

// FIXME: SleepX() - unused. Remove?
#ifdef _WIN32
  #define SleepX() Sleep(1)
#else
  #define SleepX() sleep(0)
#endif

#ifdef __unix__
  #define stricmp(s1,s2) strcasecmp((s1),(s2))
  #include <unistd.h>
#endif

#ifndef M_PI
  #define M_PI 3.1415926535897932
#endif

#ifdef __APPLE__
  #define DRGB(r,g,b) (((b)<<0x18)|((g)<<0x10)|((r)<<8))
  #define GETB(c) ((c)>>0x18)
  #define GETG(c) (((c)>>0x10)&0xFF)
  #define GETR(c) (((c)>>8)&0xFF)
#else
  #define DRGB(r,g,b) (((r)<<0x10)|((g)<<8)|(b))
  #define GETR(c) ((c)>>0x10)
  #define GETG(c) (((c)>>8)&0xFF)
  #define GETB(c) ((c)&0xFF)
#endif

#define GET_R GETR
#define GET_G GETG
#define GET_B GETB

//------
// Other
//------

// #define AUTO_DEHRUST

#endif
