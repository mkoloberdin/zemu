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

#include "defines.h"
#include <string.h>
#include <boost/algorithm/string.hpp>
#include <string>

using namespace boost::algorithm;

// TODO: remove file operations to a more appropriate place

template <typename Word>
fs::ifstream& readWord(fs::ifstream& IFS, Word& Value) {
  Value = 0;
  for(unsigned Size = 0; Size < sizeof( Word ); ++Size)
    Value |= IFS.get() << (8 * Size);
  return IFS;
}

template <typename Word>
fs::ofstream& writeWord(fs::ofstream& OFS, Word Value) {
  for(unsigned Size = sizeof( Word ); Size; --Size, Value >>= 8)
    OFS.put(static_cast<char>(Value & 0xff));
  return OFS;
}

uint8_t readU8(fs::ifstream& IFS) {
  uint8_t V;
  readWord(IFS, V);
  return V;
}

void writeU8 (fs::ofstream& OFS, const uint8_t Data) {
  writeWord(OFS, Data);
}

uint16_t readU16(fs::ifstream& IFS) {
  uint16_t V;
  readWord(IFS, V);
  return V;
}

void writeU16 (fs::ofstream& OFS, const uint16_t Data) {
  writeWord(OFS, Data);
}

uint32_t readU32(fs::ifstream& IFS) {
  uint32_t V;
  readWord(IFS, V);
  return V;
}

void writeU32 (fs::ofstream& OFS, const uint32_t Data) {
  writeWord(OFS, Data);
}

std::string lowerCaseExtension(const fs::path& Path) {
  std::string ext = Path.extension().string();
  to_lower(ext);
  return ext;
}


double sqq(double n)
{
  return (n * n);
}

int sgn(int a)
{
  return ((a == 0) ? 0 : ((a > 0) ? 1 : (-1)));
}

char hex[17] = "0123456789ABCDEF";

int unhex(char c)
{
  switch (c)
  {
  case '0':
    return 0;
  case '1':
    return 1;
  case '2':
    return 2;
  case '3':
    return 3;
  case '4':
    return 4;
  case '5':
    return 5;
  case '6':
    return 6;
  case '7':
    return 7;
  case '8':
    return 8;
  case '9':
    return 9;

  case 'a':
  case 'A':
    return 10;

  case 'b':
  case 'B':
    return 11;

  case 'c':
  case 'C':
    return 12;

  case 'd':
  case 'D':
    return 13;

  case 'e':
  case 'E':
    return 14;

  case 'f':
  case 'F':
    return 15;

  default:
    return 0;
  }
}

bool ishex(char c)
{
  return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

int addLogFirst = 1;

void AddLog(const char *fmt, ...)
{
  va_list argptr;
  char buf[0x800];
  FILE *fp;

  va_start(argptr, fmt);
  vsprintf(buf, fmt, argptr);
  va_end(argptr);

  if (addLogFirst)
  {
    fp = fopen("debuglog.txt", "wb");
    addLogFirst = 0;
  }
  else fp = fopen("debuglog.txt", "ab");

  if (fp != nullptr)
  {
    fprintf(fp, "%s\n", buf);
    fclose(fp);
  }
}

void AddLogN(const char *fmt, ...)
{
  va_list argptr;
  char buf[0x800];
  FILE *fp;

  va_start(argptr, fmt);
  vsprintf(buf, fmt, argptr);
  va_end(argptr);

  if (addLogFirst)
  {
    fp = fopen("debuglog.txt", "wb");
    addLogFirst = 0;
  }
  else fp = fopen("debuglog.txt", "ab");

  if (fp != nullptr)
  {
    fprintf(fp, "%s", buf);
    fclose(fp);
  }
}

void StrikeError(const char *fmt, ...)
{
  va_list argptr;
  char buf[0x800];

  va_start(argptr, fmt);
  vsprintf(buf, fmt, argptr);
  va_end(argptr);

#ifdef _WIN32
  fprintf(stderr, "[Error] %s\n", buf);
  MessageBox(HWND_DESKTOP, buf, "Error", 0);
  exit(1);
#else
  fprintf(stderr, "[Error] %s\n", buf);
  exit(1);
#endif
}

void StrikeMessage(const char *fmt, ...)
{
  va_list argptr;
  char buf[0x800];

  va_start(argptr, fmt);
  vsprintf(buf, fmt, argptr);
  va_end(argptr);

#ifdef _WIN32
  MessageBox(HWND_DESKTOP, buf, "Message", 0);
#else
  printf("Message: %s\n", buf);
#endif
}

char *AllocNstrcpy(const char *str)
{
  char *s;

  s = new char[strlen(str) + 1];
  strcpy(s, str);

  return s;
}
