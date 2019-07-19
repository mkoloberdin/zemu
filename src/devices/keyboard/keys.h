#ifndef _KEYS_H_INCLUDED_
#define _KEYS_H_INCLUDED_

#include "platform/hardware.h"

struct s_CfgHostKey {
    const char* cfgname;
    int keysym;
    int keysymadd;
};

struct s_CfgZxKey {
    const char* cfgname;
    int portnum;
    int bitmask;
    int inum;
};

s_CfgHostKey cfgHostKeys[] = {
    {"esc",         HW_KEYCODE_ESCAPE,      0},
    {"f1",          HW_KEYCODE_F1,          0},
    {"f2",          HW_KEYCODE_F2,          0},
    {"f3",          HW_KEYCODE_F3,          0},
    {"f4",          HW_KEYCODE_F4,          0},
    {"f5",          HW_KEYCODE_F5,          0},
    {"f6",          HW_KEYCODE_F6,          0},
    {"f7",          HW_KEYCODE_F7,          0},
    {"f8",          HW_KEYCODE_F8,          0},
    {"f9",          HW_KEYCODE_F9,          0},
    {"f10",         HW_KEYCODE_F10,         0},
    {"f11",         HW_KEYCODE_F11,         0},
    {"f12",         HW_KEYCODE_F12,         0},
    {"pause",       HW_KEYCODE_PAUSE,       0},
    {"ins",         HW_KEYCODE_INSERT,      0},
    {"home",        HW_KEYCODE_HOME,        0},
    {"del",         HW_KEYCODE_DELETE,      0},
    {"end",         HW_KEYCODE_END,         0},
    {"pgup",        HW_KEYCODE_PAGEUP,      0},
    {"pgdn",        HW_KEYCODE_PAGEDOWN,    0},
    {"`",           '`',                    0},
    {"1",           HW_KEYCODE_1,           0},
    {"2",           HW_KEYCODE_2,           0},
    {"3",           HW_KEYCODE_3,           0},
    {"4",           HW_KEYCODE_4,           0},
    {"5",           HW_KEYCODE_5,           0},
    {"6",           HW_KEYCODE_6,           0},
    {"7",           HW_KEYCODE_7,           0},
    {"8",           HW_KEYCODE_8,           0},
    {"9",           HW_KEYCODE_9,           0},
    {"0",           HW_KEYCODE_0,           0},
    {"-",           '-',                    0},
    {"=",           '=',                    0},
    {"\\",          '\\',                   0},
    {"backspace",   HW_KEYCODE_BACKSPACE,   0},
    {"tab",         HW_KEYCODE_TAB,         0},
    {"q",           HW_KEYCODE_q,           0},
    {"w",           HW_KEYCODE_w,           0},
    {"e",           HW_KEYCODE_e,           0},
    {"r",           HW_KEYCODE_r,           0},
    {"t",           HW_KEYCODE_t,           0},
    {"y",           HW_KEYCODE_y,           0},
    {"u",           HW_KEYCODE_u,           0},
    {"i",           HW_KEYCODE_i,           0},
    {"o",           HW_KEYCODE_o,           0},
    {"p",           HW_KEYCODE_p,           0},
    {"[",           '[',                    0},
    {"]",           ']',                    0},
    {"ent",         HW_KEYCODE_RETURN,      0},
    {"caps",        HW_KEYCODE_CAPSLOCK,    0},
    {"a",           HW_KEYCODE_a,           0},
    {"s",           HW_KEYCODE_s,           0},
    {"d",           HW_KEYCODE_d,           0},
    {"f",           HW_KEYCODE_f,           0},
    {"g",           HW_KEYCODE_g,           0},
    {"h",           HW_KEYCODE_h,           0},
    {"j",           HW_KEYCODE_j,           0},
    {"k",           HW_KEYCODE_k,           0},
    {"l",           HW_KEYCODE_l,           0},
    {";",           ';',                    0},
    {"'",           '\'',                   0},
    {"lshift",      HW_KEYCODE_LSHIFT,      0},
    {"z",           HW_KEYCODE_z,           0},
    {"x",           HW_KEYCODE_x,           0},
    {"c",           HW_KEYCODE_c,           0},
    {"v",           HW_KEYCODE_v,           0},
    {"b",           HW_KEYCODE_b,           0},
    {"n",           HW_KEYCODE_n,           0},
    {"m",           HW_KEYCODE_m,           0},
    {",",           ',',                    0},
    {".",           '.',                    0},
    {"/",           '/',                    0},
    {"rshift",      HW_KEYCODE_RSHIFT,      0},
    {"lctrl",       HW_KEYCODE_LCTRL,       0},
    {"lsuper",      HW_KEYCODE_LSUPER,      0},
    {"lalt",        HW_KEYCODE_LALT,        0},
    {"spc",         HW_KEYCODE_SPACE,       0},
    {"ralt",        HW_KEYCODE_RALT,        0},
    {"rsuper",      HW_KEYCODE_RSUPER,      0},
    {"rctrl",       HW_KEYCODE_RCTRL,       0},
    {"up",          HW_KEYCODE_UP,          0},
    {"down",        HW_KEYCODE_DOWN,        0},
    {"left",        HW_KEYCODE_LEFT,        0},
    {"right",       HW_KEYCODE_RIGHT,       0},
    {"shift",       HW_KEYCODE_LSHIFT,      HW_KEYCODE_RSHIFT},
    {"ctrl",        HW_KEYCODE_LCTRL,       HW_KEYCODE_RCTRL},
    {"alt",         HW_KEYCODE_LALT,        HW_KEYCODE_RALT},
    {"numlock",     HW_KEYCODE_NUMLOCK,     0},
    {"",            0,                      0}
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

    {   "", 0, 0,       0}
};

#endif
