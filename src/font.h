#ifndef _FONT_H_INCLUDED_
#define _FONT_H_INCLUDED_

#include "zemu.h"

class C_Font {
public:

    C_Font(uint8_t* data);
    C_Font(ZHW_Video_Surface* surf);
    ~C_Font();

    void PrintChar(int x, int y, char c);
    void PrintString(int x, int y, const char* str);
    int StrLenPx(const char* str);

    int Height(void);

    void CopySym(char from, char to);
    void SetSymOff(char c, int x, int y);

private:

    ZHW_Video_Surface* surf;
    int spitch;
    int off[0x100];
    int len[0x100];
    int xoff[0x100];
    int yoff[0x100];

    void CalcFont(void);

    C_Font(const C_Font&);
    C_Font& operator=(const C_Font&);
};

#endif
