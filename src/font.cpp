#include "font.h"

SDL_Surface *ExtractImage(uint8_t *data)
{
  SDL_Surface *surf;
  int hgt, wdt, i, j, r, g, b, spitch;
  SDL_PixelFormat *fmt = screen->format;

  hgt = data[0] + 0x100 * data[1];
  wdt = data[2] + 0x100 * data[3];
  data += 4;

  surf = SDL_CreateRGBSurface(SDL_SWSURFACE, wdt, hgt, fmt->BitsPerPixel, fmt->Rmask,
                              fmt->Gmask, fmt->Bmask, 0);
  spitch = surf->pitch / 4;

  if (SDL_MUSTLOCK(surf)) {
    if (SDL_LockSurface(surf) < 0) return nullptr;
  }

  for (i = 0; i < hgt; i++)
  {
    for (j = 0; j < wdt; j++)
    {
      r = *(data++);
      g = *(data++);
      b = *(data++);
      ((int *)surf->pixels)[i * spitch + j] = DRGB(r, g, b);
    }
  }

  if (SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
  return surf;
}

SDL_Surface *CopySurfaceX(SDL_Surface *src)
{
  SDL_Rect s, d;
  SDL_PixelFormat *fmt;
  SDL_Surface *dst;

  fmt = screen->format;

  s.x = 0;
  s.y = 0;
  s.w = src->w;
  s.h = src->h;

  d.x = 0;
  d.y = 0;

  dst = SDL_CreateRGBSurface(SDL_SWSURFACE, src->w, src->h, fmt->BitsPerPixel,
                             fmt->Rmask, fmt->Gmask, fmt->Bmask, 0);
  SDL_BlitSurface(src, &s, dst, &d);

  return dst;
}

//------------------------------------------------------------------------------------------------------------------------------

C_Font::C_Font()
{
  surf = nullptr;
}

C_Font::~C_Font()
{
  if (surf) SDL_FreeSurface(surf);
}

void C_Font::Init(SDL_Surface *surf)
{
  this->surf = CopySurfaceX(surf);
  CalcFont();
}

void C_Font::Init(uint8_t *data)
{
  surf = ExtractImage(data);
  CalcFont();
}

void C_Font::CalcFont(void)
{
  int x, n, h, w, t1, t2;

  if (SDL_MUSTLOCK(surf)) {
    if (SDL_LockSurface(surf) < 0) return;
  }

  spitch = surf->pitch / 4;
  h = surf->h;
  w = surf->w;

  t1 = ((int *)surf->pixels)[0];
  t2 = ((int *)surf->pixels)[1];

  for (n = 0; n < 32; n++)
  {
    off[n] = 1;
    len[n] = 1;
  }

  x = 0;

  do
  {
    while (x < w) if (((int *)surf->pixels)[x] == t1) x++;
      else break;
    off[n] = x;

    while (x < w) if (((int *)surf->pixels)[x] != t1) x++;
      else break;
    len[n] = x - off[n];

    n++;
  } while (n < 0x100 && x < w);

  for (; n < 0x100; n++)
  {
    off[n] = 1;
    len[n] = 1;
  }

  for (n = 0; n < 0x100; n++)
  {
    xoff[n] = 0;
    yoff[n] = 0;
  }

  if (SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
  SDL_SetColorKey(surf, SDL_SRCCOLORKEY, t2);
}

void C_Font::PrintChar(int x, int y, char c)
{
  int n = (uint8_t)c;
  SDL_Rect s, d;

  s.x = off[n];
  s.y = 0;
  s.w = len[n];
  s.h = surf->h;

  d.x = x + xoff[n];
  d.y = y + yoff[n];

  SDL_BlitSurface(surf, &s, screen, &d);
}

void C_Font::PrintString(int x, int y, const char *str)
{
  if (!str) return;

#ifdef _WIN32
  while (*str)
  {
    PrintChar(x, y, *str);
    x += len[(uint8_t)(*str)];
    str++;
  }
#else
  while (*str)
  {
    uint8_t ch = (uint8_t) * str;

    if (ch >= 0xD0)
    {
      str++;
      ch = ((uint8_t) * str) + (ch == 0xD1 ? 0x70 : 0x30);
    }

    PrintChar(x, y, ch);
    x += len[ch];
    str++;
  }
#endif
}

int C_Font::Height(void)
{
  return (surf->h);
}

int C_Font::StrLenPx(const char *str)
{
  int l = 0;
  while (*str) {
    l += len[(uint8_t)(*str)];
    str++;
  }
  return l;
}

void C_Font::CopySym(char from, char to)
{
  off[(uint8_t)to] = off[(uint8_t)from];
  len[(uint8_t)to] = len[(uint8_t)from];
}

void C_Font::SetSymOff(char c, int x, int y)
{
  xoff[(uint8_t)c] = x;
  yoff[(uint8_t)c] = y;
}
