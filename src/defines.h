#ifndef _DEFINES_H_INCLUDED_
#define _DEFINES_H_INCLUDED_

#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <cstdint>

#ifndef _WIN32
    #include <limits.h>
#endif

#define DEBUG_MESSAGE(msg) printf("%s\n", (msg))

#ifndef MAX_PATH
    #ifdef PATH_MAX
        #define MAX_PATH PATH_MAX
    #else
        #define MAX_PATH 0x1000
    #endif
#endif

#ifndef _WIN32
    #define stricmp(s1, s2) strcasecmp((s1), (s2))
    #include <unistd.h>
#endif

#ifndef M_PI
    #define M_PI 3.1415926535897932
#endif

#define ZEMU_MAKEWORD(H, L) (((H) << 8) | (L))

extern char hex[17];

double sqq(double n);
int sgn(int a);
int unhex(char c);
bool ishex(char c);

void AddLog(const char* fmt, ...);
void AddLogN(const char* fmt, ...);
void StrikeError(const char* fmt, ...);
void StrikeMessage(const char* fmt, ...);

char* AllocNstrcpy(const char* str);

#endif
