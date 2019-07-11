// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "render_16c.h"
#include "render_common.h"

void Render16c(unsigned long nextClk) {
    int scrA;
    int scrB;
    int b1;
    int b2;
    int b3;
    int b4;

    #include "render_common_a.h"

    if ((dev_mman.port7FFD & 8) ^ screensHack) {
        scrA = RAM_BANK7;
        scrB = RAM_BANK6;
    } else {
        scrA = RAM_BANK5;
        scrB = RAM_BANK4;
    }

    b1 = dev_mman.ram[ scrB + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];
    b2 = dev_mman.ram[ scrA + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];
    b3 = dev_mman.ram[ scrB + 0x2000 + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];
    b4 = dev_mman.ram[ scrA + 0x2000 + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];

    // IiGRBgrb

    *(scr++) = colors[((b1 & 0x40) >> 3) | (b1 & 7)];
    *(scr++) = colors[((b1 & 0x80) >> 4) | ((b1 & 0x38) >> 3)];
    *(scr++) = colors[((b2 & 0x40) >> 3) | (b2 & 7)];
    *(scr++) = colors[((b2 & 0x80) >> 4) | ((b2 & 0x38) >> 3)];
    *(scr++) = colors[((b3 & 0x40) >> 3) | (b3 & 7)];
    *(scr++) = colors[((b3 & 0x80) >> 4) | ((b3 & 0x38) >> 3)];
    *(scr++) = colors[((b4 & 0x40) >> 3) | (b4 & 7)];
    *(scr++) = colors[((b4 & 0x80) >> 4) | ((b4 & 0x38) >> 3)];

    #include "render_common_b.h"
}
