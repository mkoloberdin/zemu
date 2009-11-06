/*
    Tanks - Tank arcade game
    Copyright (c) 2005 ProZroks

    <<< LICENSE >>>

    ProZroks
    support@prozroks.com

    Last modifed: Restorer (22 Jun, 2005)
*/

/*
    Whatsnew.

    Restorer (22 Jun, 2004)
        v1.0 - initial release
        v1.1 - Linux Port
        v1.2 - SDL port
*/


#ifndef __DEFINES_RULEZ__
#define __DEFINES_RULEZ__ 1

#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifdef __linux__
	#include <limits.h>
#endif

#define _DEBUG(msg) printf("%s\n",(msg))

#ifndef MAX_PATH
	#ifdef PATH_MAX
		#define MAX_PATH PATH_MAX
	#else
		#define MAX_PATH 0x1000
	#endif
#endif

#ifdef _WIN32
	#define SleepX() Sleep(1)
#else
	#define SleepX() sleep(0)
#endif

#ifdef __linux__
	// it seems that stricmp is not ANSI, but strcasecmp is
	#define stricmp(s1,s2) strcasecmp((s1),(s2))
	// it also seems that unistd is need for sleep and some others functions
	#include <unistd.h>
#endif

#ifndef BYTE
	typedef unsigned char BYTE;
#endif

#ifndef WORD
	typedef unsigned short WORD;
#endif

#ifndef DWORD
	typedef unsigned long DWORD;
#endif

#define byte BYTE
#define word WORD
#define dword DWORD

#ifndef M_PI
	#define M_PI 3.1415926535897932
#endif

#ifndef min
	#define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef max
	#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifdef ZEMU_BIG_ENDIAN
	#define DRGB(r,g,b) ( ((b)<<0x18) | ((g)<<0x10) | ((r)<<8) )
	#define GETR(c) (((c)>>8)&0xFF)
	#define GETG(c) (((c)>>0x10)&0xFF)
	#define GETB(c) (((c)>>0x18)&0xFF)
#else
	#define DRGB(r,g,b) (((r)<<0x10)|((g)<<8)|(b))
	#define GETR(c) ((c)>>0x10)
	#define GETG(c) (((c)>>8)&0xFF)
	#define GETB(c) ((c)&0xFF)
#endif

#define GET_R GETR
#define GET_G GETG
#define GET_B GETB

extern char hex[17];

double sqq(double n);
int sgn(int a);
int unhex(char c);
bool ishex(char c);

void AddLog(const char *fmt, ...);
void AddLogN(const char *fmt, ...);
void StrikeError(const char *fmt, ...);
void StrikeMessage(const char *fmt, ...);

char* AllocNstrcpy(const char *str);

#endif
