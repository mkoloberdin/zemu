// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "font.h"

C_Font::C_Font(uint8_t* data) {
    pixelsHeight = data[0] + 0x100 * data[1];
    pixelsWidth = data[2] + 0x100 * data[3];
    data += 4;

    pixels = new uint32_t[pixelsWidth * pixelsHeight];

    for (int i = pixelsWidth * pixelsHeight; i--;) {
        int r = *(data++);
        int g = *(data++);
        int b = *(data++);
        pixels[i] = HW_MAKERGB(r, g, b);
    }

    CalcFont();
}

C_Font::~C_Font() {
    delete[] pixels;
}

void C_Font::CalcFont(void) {
    int n;

    uint32_t t1 = pixels[0];
    colorKey = pixels[1];

    for (n = 0; n < 32; n++) {
        off[n] = 1;
        len[n] = 1;
    }

    int x = 0;

    do {
        while (x < pixelsWidth) {
            if (pixels[x] == t1) {
                x++;
            } else {
                break;
            }
        }

        off[n] = x;

        while (x < pixelsWidth) {
            if (pixels[x] != t1) {
                x++;
            } else {
                break;
            }
        }

        len[n] = x - off[n];
        n++;
    } while (n < 0x100 && x < pixelsWidth);

    for (; n < 0x100; n++) {
        off[n] = 1;
        len[n] = 1;
    }

    for (n = 0; n < 0x100; n++) {
        xoff[n] = 0;
        yoff[n] = 0;
    }
}

void C_Font::PrintChar(int x, int y, char c) {
    int n = (uint8_t)c;
    int screenX = x + xoff[n];
    int screenY = y + yoff[n];

    if (screenX >= WIDTH || screenY >= HEIGHT) {
        return;
    }

    int letterX = off[n];
    int letterY = 0;
    int letterW = len[n];
    int letterH = pixelsHeight;

    if (screenX < 0) {
        letterW += screenX;

        if (letterW <= 0) {
            return;
        }

        letterX -= screenX;
        screenX = 0;
    }

    if (screenY < 0) {
        letterH += screenY;

        if (letterH <= 0) {
            return;
        }

        letterY -= screenY;
        screenY = 0;
    }

    if (screenX + letterW >= WIDTH) {
        letterW -= screenX + letterW - WIDTH + 1;

        if (letterW <= 0) {
            return;
        }
    }

    if (screenY + letterH >= HEIGHT) {
        letterH -= screenY + letterH - HEIGHT + 1;

        if (letterH <= 0) {
            return;
        }
    }

    for (int y = letterH; y--;) {
        for (int x = letterW; x--;) {
            uint32_t c = pixels[(letterY + y) * pixelsWidth + letterX + x];

            if (c != colorKey) {
                screen[(screenY + y) * WIDTH + screenX + x] = c;
            }
        }
    }
}

void C_Font::PrintString(int x, int y, const char* str) {
    if (!str) {
        return;
    }

    #ifdef _WIN32
        while (*str) {
            PrintChar(x, y, *str);
            x += len[(uint8_t)(*str)];
            str++;
        }
    #else
        while (*str) {
            uint8_t ch = (uint8_t)(*str);

            if (ch >= 0xD0) {
                str++;

                if (!(*str)) {
                    break;
                }

                ch = ((uint8_t)(*str)) + (ch == 0xD1 ? 0x70 : 0x30);
            }

            PrintChar(x, y, ch);
            x += len[ch];
            str++;
        }
    #endif
}

int C_Font::Height(void) {
    return pixelsHeight;
}

int C_Font::StrLenPx(const char* str) {
    int l = 0;

    #ifdef _WIN32
        while (*str) {
            l += len[(uint8_t)(*str)];
            str++;
        }
    #else
        while (*str) {
            uint8_t ch = (uint8_t)(*str);

            if (ch >= 0xD0) {
                str++;

                if (!(*str)) {
                    break;
                }

                ch = ((uint8_t)(*str)) + (ch == 0xD1 ? 0x70 : 0x30);
            }

            l += len[ch];
            str++;
        }
    #endif

    return l;
}

void C_Font::CopySym(char from, char to) {
    off[(uint8_t)to] = off[(uint8_t)from];
    len[(uint8_t)to] = len[(uint8_t)from];
}

void C_Font::SetSymOff(char c, int x, int y) {
    xoff[(uint8_t)c] = x;
    yoff[(uint8_t)c] = y;
}
