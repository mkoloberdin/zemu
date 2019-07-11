// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "render_speccy.h"
#include "render_common.h"

void RenderSpeccy(unsigned long nextClk) {
    int ci;
    int cp;
    int bt;

    #include "render_common_a.h"

    zxScreen = ((dev_mman.port7FFD & 8) ^ screensHack) ? RAM_BANK7 : RAM_BANK5;

    if (!attributesHack) {
        bt = dev_mman.ram[ zxScreen + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];
        cl = dev_mman.ram[ zxScreen + 0x1800 + ((zxLine & 0xF8) << 2) + pos ];

        if (flashColor) {
            ci = colors[((cl & 64) >> 3) | (cl & 7)];
            cp = (cl >> 3) & 7;

            if (cp) {
                cp = colors[cp];

                int r = ((unsigned int)ZHW_VIDEO_GETR(ci) + (unsigned int)ZHW_VIDEO_GETR(cp)) >> 1;
                int g = ((unsigned int)ZHW_VIDEO_GETG(ci) + (unsigned int)ZHW_VIDEO_GETG(cp)) >> 1;
                int b = ((unsigned int)ZHW_VIDEO_GETB(ci) + (unsigned int)ZHW_VIDEO_GETB(cp)) >> 1;

                ci = ZHW_VIDEO_MAKERGB(r, g, b);
            }

            cp = ZHW_VIDEO_MAKERGB(0, 0, 0);
        } else {
            if ((frames & 32) && (cl & 128)) {
                cp = colors[((cl & 64) >> 3) | (cl & 7)];
                ci = colors[((cl & 64) >> 3) | ((cl >> 3) & 7)];
            } else {
                ci = colors[((cl & 64) >> 3) | (cl & 7)];
                cp = colors[((cl & 64) >> 3) | ((cl >> 3) & 7)];
            }
        }
    } else if (attributesHack == 1) {
        bt = dev_mman.ram[ zxScreen + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];

        if (((zxLine >> 3) & 1) ^ (pos & 1)) {
            ci = ZHW_VIDEO_MAKERGB(0, 0, 0);
            cp = ZHW_VIDEO_MAKERGB(192, 192, 192);
        } else {
            ci = ZHW_VIDEO_MAKERGB(64, 64, 64);
            cp = ZHW_VIDEO_MAKERGB(255, 255, 255);
        }
    } else { // attributesHack == 2
        bt = (((zxLine & 6) == 0 || (zxLine & 6) == 6) ? 0x00 : 0x3C);
        cl = dev_mman.ram[ zxScreen + 0x1800 + ((zxLine & 0xF8) << 2) + pos ];

        if ((frames & 32) && (cl & 128)) {
            cp = colors[((cl & 64) >> 3) | (cl & 7)];
            ci = colors[((cl & 64) >> 3) | ((cl >> 3) & 7)];
        } else {
            ci = colors[((cl & 64) >> 3) | (cl & 7)];
            cp = colors[((cl & 64) >> 3) | ((cl >> 3) & 7)];
        }
    }

    *(scr++) = (bt & 128 ? ci : cp);
    *(scr++) = (bt &  64 ? ci : cp);
    *(scr++) = (bt &  32 ? ci : cp);
    *(scr++) = (bt &  16 ? ci : cp);
    *(scr++) = (bt &   8 ? ci : cp);
    *(scr++) = (bt &   4 ? ci : cp);
    *(scr++) = (bt &   2 ? ci : cp);
    *(scr)   = (bt &   1 ? ci : cp);

    #include "render_common_b.h"
}
