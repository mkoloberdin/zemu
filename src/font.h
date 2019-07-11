#ifndef _FONT_H_INCLUDED_
#define _FONT_H_INCLUDED_

#include "zemu.h"

class C_Font {
    public:

    C_Font();
    ~C_Font();

    void Init(uint8_t* data);
    void Init(ZHW_Video_Surface* surf);

    void PrintChar(int x, int y, char c);
    void PrintString(int x, int y, const char* str);
    int StrLenPx(const char* str);

    int Height(void);

    void CopySym(char from, char to);
    void SetSymOff(char c, int x, int y);

    private:

    ZHW_Video_Surface* surf = nullptr;
    int spitch = 0;
    int off[0x100] = {0};
    int len[0x100] = {0};
    int xoff[0x100] = {0};
    int yoff[0x100] = {0};

    void CalcFont(void);
};

#endif
