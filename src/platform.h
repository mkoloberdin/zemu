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
#endif // _MSC_VER

#endif // _WIN32

#ifdef ZEMU_BIG_ENDIAN
	#define WORD_SWAP_BYTES(a) ((((a)&0xFF)<<8)|((a)>>8))
#endif

// #define AUTO_DEHRUST

#endif
