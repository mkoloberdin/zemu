#ifndef _PLATFORM_H_INCLUDED_
#define _PLATFORM_H_INCLUDED_

// Platform-specific includes and defines

//---------------
// SDL Predefined
//---------------

#if (SDL_BYTEORDER - SDL_LIL_ENDIAN)
	#define ZEMU_BIG_ENDIAN
	#define WORD_SWAP_BYTES(a) ((((a)&0xFF)<<8)|((a)>>8))
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

//------
// Other
//------

// #define AUTO_DEHRUST

#endif
