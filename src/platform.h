#ifndef _PLATFORM_H_INCLUDED_
#define _PLATFORM_H_INCLUDED_

// Platform-specific includes and defines

// + Fix for FreeBSD (by breeze)

#ifdef __FreeBSD__
	#define __linux__
#endif

// - Fix for FreeBSD (by breeze)

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
#endif

//------
// Other
//------

// #define AUTO_DEHRUST

#endif
