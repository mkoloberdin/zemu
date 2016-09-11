#ifndef _KEYS_H_INCLUDED_
#define _KEYS_H_INCLUDED_

#include <SDL.h>

struct s_CfgHostKey
{
  const char *cfgname;
  int keysym;
  int keysymadd;
};

struct s_CfgZxKey
{
  const char *cfgname;
  int portnum;
  int bitmask;
  int inum;
};

s_CfgHostKey cfgHostKeys[] = {
  {"esc",       SDLK_ESCAPE,		0},
  {"f1",        SDLK_F1,			0},
  {"f2",        SDLK_F2,			0},
  {"f3",        SDLK_F3,			0},
  {"f4",        SDLK_F4,			0},
  {"f5",        SDLK_F5,			0},
  {"f6",        SDLK_F6,			0},
  {"f7",        SDLK_F7,			0},
  {"f8",        SDLK_F8,			0},
  {"f9",        SDLK_F9,			0},
  {"f10",       SDLK_F10,			0},
  {"f11",       SDLK_F11,			0},
  {"f12",       SDLK_F12,			0},
  {"pause",     SDLK_PAUSE,		0},
  {"ins",       SDLK_INSERT,		0},
  {"home",      SDLK_HOME,		0},
  {"del",       SDLK_DELETE,		0},
  {"end",       SDLK_END,			0},
  {"pgup",      SDLK_PAGEUP,		0},
  {"pgdn",      SDLK_PAGEDOWN,	0},
  {"`",         '`',				0},
  {"1",         SDLK_1,			0},
  {"2",         SDLK_2,			0},
  {"3",         SDLK_3,			0},
  {"4",         SDLK_4,			0},
  {"5",         SDLK_5,			0},
  {"6",         SDLK_6,			0},
  {"7",         SDLK_7,			0},
  {"8",         SDLK_8,			0},
  {"9",         SDLK_9,			0},
  {"0",         SDLK_0,			0},
  {"-",         '-',				0},
  {"=",         '=',				0},
  {"\\",        '\\',				0},
  {"backspace", SDLK_BACKSPACE,	0},
  {"tab",       SDLK_TAB,			0},
  {"q",         SDLK_q,			0},
  {"w",         SDLK_w,			0},
  {"e",         SDLK_e,			0},
  {"r",         SDLK_r,			0},
  {"t",         SDLK_t,			0},
  {"y",         SDLK_y,			0},
  {"u",         SDLK_u,			0},
  {"i",         SDLK_i,			0},
  {"o",         SDLK_o,			0},
  {"p",         SDLK_p,			0},
  {"[",         '[',				0},
  {"]",         ']',				0},
  {"ent",       SDLK_RETURN,		0},
  {"caps",      SDLK_CAPSLOCK,	0},
  {"a",         SDLK_a,			0},
  {"s",         SDLK_s,			0},
  {"d",         SDLK_d,			0},
  {"f",         SDLK_f,			0},
  {"g",         SDLK_g,			0},
  {"h",         SDLK_h,			0},
  {"j",         SDLK_j,			0},
  {"k",         SDLK_k,			0},
  {"l",         SDLK_l,			0},
  {";",         ';',				0},
  {"'",         '\'',				0},
  {"lshift",    SDLK_LSHIFT,		0},
  {"z",         SDLK_z,			0},
  {"x",         SDLK_x,			0},
  {"c",         SDLK_c,			0},
  {"v",         SDLK_v,			0},
  {"b",         SDLK_b,			0},
  {"n",         SDLK_n,			0},
  {"m",         SDLK_m,			0},
  {",",         ',',				0},
  {".",         '.',				0},
  {"/",         '/',				0},
  {"rshift",    SDLK_RSHIFT,		0},
  {"lctrl",     SDLK_LCTRL,		0},
  {"lsuper",    SDLK_LSUPER,		0},
  {"lalt",      SDLK_LALT,		0},
  {"spc",       SDLK_SPACE,		0},
  {"ralt",      SDLK_RALT,		0},
  {"rsuper",    SDLK_RSUPER,		0},
  {"rctrl",     SDLK_RCTRL,		0},
  {"up",        SDLK_UP,			0},
  {"down",      SDLK_DOWN,		0},
  {"left",      SDLK_LEFT,		0},
  {"right",     SDLK_RIGHT,		0},
  {"shift",     SDLK_LSHIFT,		SDLK_RSHIFT},
  {"ctrl",      SDLK_LCTRL,		SDLK_RCTRL},
  {"alt",       SDLK_LALT,		SDLK_RALT},
  {"numlock",   SDLK_NUMLOCK,		0},
  {"",          0}
};

s_CfgZxKey cfgZxKeys[] = {
  {"spc", 7, 0b00001,  0},
  { "ss", 7, 0b00010,  1},
  {  "m", 7, 0b00100,  2},
  {  "n", 7, 0b01000,  3},
  {  "b", 7, 0b10000,  4},

  {"ent", 6, 0b00001,  5},
  {  "l", 6, 0b00010,  6},
  {  "k", 6, 0b00100,  7},
  {  "j", 6, 0b01000,  8},
  {  "h", 6, 0b10000,  9},

  {  "p", 5, 0b00001, 10},
  {  "o", 5, 0b00010, 11},
  {  "i", 5, 0b00100, 12},
  {  "u", 5, 0b01000, 13},
  {  "y", 5, 0b10000, 14},

  {  "0", 4, 0b00001, 15},
  {  "9", 4, 0b00010, 16},
  {  "8", 4, 0b00100, 17},
  {  "7", 4, 0b01000, 18},
  {  "6", 4, 0b10000, 19},

  {  "1", 3, 0b00001, 20},
  {  "2", 3, 0b00010, 21},
  {  "3", 3, 0b00100, 22},
  {  "4", 3, 0b01000, 23},
  {  "5", 3, 0b10000, 24},

  {  "q", 2, 0b00001, 25},
  {  "w", 2, 0b00010, 26},
  {  "e", 2, 0b00100, 27},
  {  "r", 2, 0b01000, 28},
  {  "t", 2, 0b10000, 29},

  {  "a", 1, 0b00001, 30},
  {  "s", 1, 0b00010, 31},
  {  "d", 1, 0b00100, 32},
  {  "f", 1, 0b01000, 33},
  {  "g", 1, 0b10000, 34},

  { "cs", 0, 0b00001, 35},
  {  "z", 0, 0b00010, 36},
  {  "x", 0, 0b00100, 37},
  {  "c", 0, 0b01000, 38},
  {  "v", 0, 0b10000, 39},

  {   "", 0, 0}
};

#endif
