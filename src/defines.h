#ifndef ZEMU_DEFINES_H
#define ZEMU_DEFINES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <cstdint>
#include <boost/filesystem.hpp>
#include <z80ex.h>
#include "platform.h"

namespace fs = boost::filesystem;

#ifndef Z80EX_ZAME_WRAPPER
  #define Z80EX_CONTEXT_PARAM Z80EX_CONTEXT *cpu,
#else
  #define Z80EX_CONTEXT_PARAM
#endif

// TODO: remove file operations to more appropriate place
// TODO: handle errors/exceptions properly

template <typename Word>
fs::ifstream& readWord(fs::ifstream& IFS, Word& Value);

template <typename Word>
fs::ofstream& writeWord(fs::ofstream& OFS, Word Value);

uint8_t readU8(fs::ifstream& IFS);
void writeU8 (fs::ofstream& OFS, const uint8_t Data);

uint16_t readU16(fs::ifstream& IFS);
void writeU16 (fs::ofstream& OFS, const uint16_t Data);

uint32_t readU32(fs::ifstream& IFS);
void writeU32 (fs::ofstream& OFS, const uint32_t Data);

std::string lowerCaseExtension(const fs::path& Path);

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
