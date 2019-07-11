#include "graphics.h"

void OutputGimpImage(int x, int y, s_GimpImage* img) {
    int i;
    int j;
    int r;
    int g;
    int b;
    int a;
    uint8_t* o;

    if (!ZHW_VIDEO_LOCKSURFACE(screen)) {
        return;
    }

    o = img->data;

    if (img->bpp == 3) {
        for (i = 0; i < (int)img->height; i++) {
            for (j = 0; j < (int)img->width; j++) {
                r = *(o++);
                g = *(o++);
                b = *(o++);
                ((int*)screen->pixels)[(y + i) * PITCH + x + j] = ZHW_VIDEO_MAKERGB(r, g, b);
            }
        }
    } else if (img->bpp == 4) {
        for (i = 0; i < (int)img->height; i++) {
            for (j = 0; j < (int)img->width; j++) {
                r = *(o++);
                g = *(o++);
                b = *(o++);
                a = *(o++);

                if (a > 128) {
                    ((int*)screen->pixels)[(y + i) * PITCH + x + j] = ZHW_VIDEO_MAKERGB(r, g, b);
                }
            }
        }
    }

    ZHW_VIDEO_UNLOCKSURFACE(screen);
}

void Bar(int x1, int y1, int x2, int y2, int c) {
    int sw;
    ZHW_Video_Rect r;

    if (x2 < x1) {
        sw = x1;
        x1 = x2;
        x2 = sw;
    }

    if (y2 < y1) {
        sw = y1;
        y1 = y2;
        y2 = sw;
    }

    if (x2 < 0 || y2 < 0 || x1 >= WIDTH || y1 >= HEIGHT || !ZHW_VIDEO_LOCKSURFACE(screen)) {
        return;
    }

    r.x = x1;
    r.y = y1;
    r.w = x2 - x1 + 1;
    r.h = y2 - y1 + 1;

    ZHW_Video_FillRect(screen, &r, c);
    ZHW_VIDEO_UNLOCKSURFACE(screen);
}
