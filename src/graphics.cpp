// FIXME:
#include "platform/sdl/platform.h"

#include "graphics.h"

void OutputGimpImage(int x, int y, s_GimpImage *img)
{
  int i, j, r, g, b, a;
  uint8_t *o;

  if (SDL_MUSTLOCK(screen)) {
    if (SDL_LockSurface(screen) < 0) return;
  }

  o = img->data;

  if (img->bpp == 3)
  {
    for (i = 0; i < (int)img->height; i++)
    {
      for (j = 0; j < (int)img->width; j++)
      {
        r = *(o++);
        g = *(o++);
        b = *(o++);
        ((int *)screen->pixels)[(y + i)*PITCH + x + j] = DRGB(r, g, b);
      }
    }
  }
  else if (img->bpp == 4)
  {
    for (i = 0; i < (int)img->height; i++)
    {
      for (j = 0; j < (int)img->width; j++)
      {
        r = *(o++);
        g = *(o++);
        b = *(o++);
        a = *(o++);
        if (a > 128)((int *)screen->pixels)[(y + i)*PITCH + x + j] = DRGB(r, g, b);
      }
    }
  }

  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
}

void Bar(int x1, int y1, int x2, int y2, int c)
{
  int sw;
  SDL_Rect r;

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
  if (x2 < 0 || y2 < 0 || x1 >= WIDTH || y1 >= HEIGHT) return;

  if (SDL_MUSTLOCK(screen)) {
    if (SDL_LockSurface(screen) < 0) return;
  }

  r.x = x1;
  r.y = y1;
  r.w = x2 - x1 + 1;
  r.h = y2 - y1 + 1;
  SDL_FillRect(screen, &r, c);

  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
}
