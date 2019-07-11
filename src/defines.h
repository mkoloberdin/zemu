#ifndef _DEFINES_H_INCLUDED_
#define _DEFINES_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <cstdint>
#include "platform.h"

#define ZEMU_MAKEWORD(H, L) (((H) << 8) | (L))
#define DEBUG_MESSAGE(msg) printf("%s\n", (msg))

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
