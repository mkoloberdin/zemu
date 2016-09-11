#ifndef ZEMU_DEFINES_H
#define ZEMU_DEFINES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <cstdint>
#include "platform.h"

#define DEBUG_MESSAGE(msg) printf("%s\n",(msg))

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


