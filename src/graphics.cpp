// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "graphics.h"

void OutputGimpImage(int x, int y, s_GimpImage* img) {
    uint8_t* o = img->data;

    if (img->bpp == 3) {
        for (unsigned int i = img->height; i--;) {
            for (unsigned int j = img->width; j--;) {
                int r = *(o++);
                int g = *(o++);
                int b = *(o++);
                screen[(y + i) * WIDTH + x + j] = STAGE_MAKERGB(r, g, b);
            }
        }
    } else if (img->bpp == 4) {
        for (unsigned int i = img->height; i--;) {
            for (unsigned int j = img->width; j--;) {
                int r = *(o++);
                int g = *(o++);
                int b = *(o++);
                int a = *(o++);

                if (a > 128) {
                    screen[(y + i) * WIDTH + x + j] = STAGE_MAKERGB(r, g, b);
                }
            }
        }
    }
}

void Bar(int x1, int y1, int x2, int y2, int c) {
    int sx = std::min(x1, x2);
    int sy = std::min(y1, y2);
    int ex = std::max(x1, x2);
    int ey = std::max(y1, y2);

    if (sx >= WIDTH || sy >= HEIGHT || ex < 0 || ey < 0) {
        return;
    }

    sx = std::max(0, sx);
    sy = std::max(0, sy);
    ex = std::min(WIDTH - 1, ex);
    ey = std::min(HEIGHT - 1, ey);

    for (int y = sy; y <= ey; y++) {
        for (int x = sx; x <= sy; x++) {
            screen[y * WIDTH + x] = c;
        }
    }
}
