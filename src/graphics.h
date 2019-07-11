#ifndef _GRAPHICS_H_INCLUDED_
#define _GRAPHICS_H_INCLUDED_

#include "defines.h"
#include "zemu.h"

struct s_GimpImage {
    unsigned int width;
    unsigned int height;
    unsigned int bpp; // 3 - RGB, 4 - RGBA
    unsigned char data[];
};

void OutputGimpImage(int x, int y, s_GimpImage* img);
void Bar(int x1, int y1, int x2, int y2, int c);

#endif
