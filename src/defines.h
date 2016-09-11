#ifndef ZEMU_DEFINES_H
#define ZEMU_DEFINES_H

#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <cstdint>

#ifdef __unix__
  #include <limits.h>
#endif

#define DEBUG_MESSAGE(msg) printf("%s\n",(msg))

#ifndef MAX_PATH
  #ifdef PATH_MAX
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

#endif // ZEMU_DEFINES_H


