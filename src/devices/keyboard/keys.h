#ifndef _KEYS_H_INCLUDED_
#define _KEYS_H_INCLUDED_

#include "../../platform_hardware.h"
#include "../../bin.h"

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
    {"esc",         ZHW_KEY_ESCAPE,     0},
    {"f1",          ZHW_KEY_F1,         0},
    {"f2",          ZHW_KEY_F2,         0},
    {"f3",          ZHW_KEY_F3,         0},
    {"f4",          ZHW_KEY_F4,         0},
    {"f5",          ZHW_KEY_F5,         0},
    {"f6",          ZHW_KEY_F6,         0},
    {"f7",          ZHW_KEY_F7,         0},
    {"f8",          ZHW_KEY_F8,         0},
    {"f9",          ZHW_KEY_F9,         0},
    {"f10",         ZHW_KEY_F10,        0},
    {"f11",         ZHW_KEY_F11,        0},
    {"f12",         ZHW_KEY_F12,        0},
    {"pause",       ZHW_KEY_PAUSE,      0},
    {"ins",         ZHW_KEY_INSERT,     0},
    {"home",        ZHW_KEY_HOME,       0},
    {"del",         ZHW_KEY_DELETE,     0},
    {"end",         ZHW_KEY_END,        0},
    {"pgup",        ZHW_KEY_PAGEUP,     0},
    {"pgdn",        ZHW_KEY_PAGEDOWN,   0},
    {"`",           '`',                0},
    {"1",           ZHW_KEY_1,          0},
    {"2",           ZHW_KEY_2,          0},
    {"3",           ZHW_KEY_3,          0},
    {"4",           ZHW_KEY_4,          0},
    {"5",           ZHW_KEY_5,          0},
    {"6",           ZHW_KEY_6,          0},
    {"7",           ZHW_KEY_7,          0},
    {"8",           ZHW_KEY_8,          0},
    {"9",           ZHW_KEY_9,          0},
    {"0",           ZHW_KEY_0,          0},
    {"-",           '-',                0},
    {"=",           '=',                0},
    {"\\",          '\\',               0},
    {"backspace",   ZHW_KEY_BACKSPACE,  0},
    {"tab",         ZHW_KEY_TAB,        0},
    {"q",           ZHW_KEY_q,          0},
    {"w",           ZHW_KEY_w,          0},
    {"e",           ZHW_KEY_e,          0},
    {"r",           ZHW_KEY_r,          0},
    {"t",           ZHW_KEY_t,          0},
    {"y",           ZHW_KEY_y,          0},
    {"u",           ZHW_KEY_u,          0},
    {"i",           ZHW_KEY_i,          0},
    {"o",           ZHW_KEY_o,          0},
    {"p",           ZHW_KEY_p,          0},
    {"[",           '[',                0},
    {"]",           ']',                0},
    {"ent",         ZHW_KEY_RETURN,     0},
    {"caps",        ZHW_KEY_CAPSLOCK,   0},
    {"a",           ZHW_KEY_a,          0},
    {"s",           ZHW_KEY_s,          0},
    {"d",           ZHW_KEY_d,          0},
    {"f",           ZHW_KEY_f,          0},
    {"g",           ZHW_KEY_g,          0},
    {"h",           ZHW_KEY_h,          0},
    {"j",           ZHW_KEY_j,          0},
    {"k",           ZHW_KEY_k,          0},
    {"l",           ZHW_KEY_l,          0},
    {";",           ';',                0},
    {"'",           '\'',               0},
    {"lshift",      ZHW_KEY_LSHIFT,     0},
    {"z",           ZHW_KEY_z,          0},
    {"x",           ZHW_KEY_x,          0},
    {"c",           ZHW_KEY_c,          0},
    {"v",           ZHW_KEY_v,          0},
    {"b",           ZHW_KEY_b,          0},
    {"n",           ZHW_KEY_n,          0},
    {"m",           ZHW_KEY_m,          0},
    {",",           ',',                0},
    {".",           '.',                0},
    {"/",           '/',                0},
    {"rshift",      ZHW_KEY_RSHIFT,     0},
    {"lctrl",       ZHW_KEY_LCTRL,      0},
    {"lsuper",      ZHW_KEY_LSUPER,     0},
    {"lalt",        ZHW_KEY_LALT,       0},
    {"spc",         ZHW_KEY_SPACE,      0},
    {"ralt",        ZHW_KEY_RALT,       0},
    {"rsuper",      ZHW_KEY_RSUPER,     0},
    {"rctrl",       ZHW_KEY_RCTRL,      0},
    {"up",          ZHW_KEY_UP,         0},
    {"down",        ZHW_KEY_DOWN,       0},
    {"left",        ZHW_KEY_LEFT,       0},
    {"right",       ZHW_KEY_RIGHT,      0},
    {"shift",       ZHW_KEY_LSHIFT,     ZHW_KEY_RSHIFT},
    {"ctrl",        ZHW_KEY_LCTRL,      ZHW_KEY_RCTRL},
    {"alt",         ZHW_KEY_LALT,       ZHW_KEY_RALT},
    {"numlock",     ZHW_KEY_NUMLOCK,    0},
    {"",            0,                  0}
};

s_CfgZxKey cfgZxKeys[] = {
    {"spc", 7, BIN(0x00001),  0},
    { "ss", 7, BIN(0x00010),  1},
    {  "m", 7, BIN(0x00100),  2},
    {  "n", 7, BIN(0x01000),  3},
    {  "b", 7, BIN(0x10000),  4},

    {"ent", 6, BIN(0x00001),  5},
    {  "l", 6, BIN(0x00010),  6},
    {  "k", 6, BIN(0x00100),  7},
    {  "j", 6, BIN(0x01000),  8},
    {  "h", 6, BIN(0x10000),  9},

    {  "p", 5, BIN(0x00001), 10},
    {  "o", 5, BIN(0x00010), 11},
    {  "i", 5, BIN(0x00100), 12},
    {  "u", 5, BIN(0x01000), 13},
    {  "y", 5, BIN(0x10000), 14},

    {  "0", 4, BIN(0x00001), 15},
    {  "9", 4, BIN(0x00010), 16},
    {  "8", 4, BIN(0x00100), 17},
    {  "7", 4, BIN(0x01000), 18},
    {  "6", 4, BIN(0x10000), 19},

    {  "1", 3, BIN(0x00001), 20},
    {  "2", 3, BIN(0x00010), 21},
    {  "3", 3, BIN(0x00100), 22},
    {  "4", 3, BIN(0x01000), 23},
    {  "5", 3, BIN(0x10000), 24},

    {  "q", 2, BIN(0x00001), 25},
    {  "w", 2, BIN(0x00010), 26},
    {  "e", 2, BIN(0x00100), 27},
    {  "r", 2, BIN(0x01000), 28},
    {  "t", 2, BIN(0x10000), 29},

    {  "a", 1, BIN(0x00001), 30},
    {  "s", 1, BIN(0x00010), 31},
    {  "d", 1, BIN(0x00100), 32},
    {  "f", 1, BIN(0x01000), 33},
    {  "g", 1, BIN(0x10000), 34},

    { "cs", 0, BIN(0x00001), 35},
    {  "z", 0, BIN(0x00010), 36},
    {  "x", 0, BIN(0x00100), 37},
    {  "c", 0, BIN(0x01000), 38},
    {  "v", 0, BIN(0x10000), 39},

    {   "", 0, 0}
};

#endif
