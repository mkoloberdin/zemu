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

#ifdef PLATFORM_WIN
	//---------------
	// WIN32-specific
	//---------------

	#include <windows.h>
	#define _I386
	#define OS_WINDOWS
#else
	//------------------
	// x86Linux-specific
	//------------------

	#define _LINUX
	#define _I386
	#define OS_LINUX
#endif

//------
// Other
//------

// #define AUTO_DEHRUST

#endif
