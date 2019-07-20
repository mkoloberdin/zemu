#ifndef _KEYS_H_INCLUDED_
#define _KEYS_H_INCLUDED_

#include "host/keycode.h"

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
    //              //                          //
    {"esc",         STAGE_KEYCODE_ESCAPE,       0},
    {"f1",          STAGE_KEYCODE_F1,           0},
    {"f2",          STAGE_KEYCODE_F2,           0},
    {"f3",          STAGE_KEYCODE_F3,           0},
    {"f4",          STAGE_KEYCODE_F4,           0},
    {"f5",          STAGE_KEYCODE_F5,           0},
    {"f6",          STAGE_KEYCODE_F6,           0},
    {"f7",          STAGE_KEYCODE_F7,           0},
    {"f8",          STAGE_KEYCODE_F8,           0},
    {"f9",          STAGE_KEYCODE_F9,           0},
    {"f10",         STAGE_KEYCODE_F10,          0},
    {"f11",         STAGE_KEYCODE_F11,          0},
    {"f12",         STAGE_KEYCODE_F12,          0},
    {"pause",       STAGE_KEYCODE_PAUSE,        0},
    {"ins",         STAGE_KEYCODE_INSERT,       0},
    {"home",        STAGE_KEYCODE_HOME,         0},
    {"del",         STAGE_KEYCODE_DELETE,       0},
    {"end",         STAGE_KEYCODE_END,          0},
    {"pgup",        STAGE_KEYCODE_PAGEUP,       0},
    {"pgdn",        STAGE_KEYCODE_PAGEDOWN,     0},
    {"`",           '`',                        0},
    {"1",           STAGE_KEYCODE_1,            0},
    {"2",           STAGE_KEYCODE_2,            0},
    {"3",           STAGE_KEYCODE_3,            0},
    {"4",           STAGE_KEYCODE_4,            0},
    {"5",           STAGE_KEYCODE_5,            0},
    {"6",           STAGE_KEYCODE_6,            0},
    {"7",           STAGE_KEYCODE_7,            0},
    {"8",           STAGE_KEYCODE_8,            0},
    {"9",           STAGE_KEYCODE_9,            0},
    {"0",           STAGE_KEYCODE_0,            0},
    {"-",           '-',                        0},
    {"=",           '=',                        0},
    {"\\",          '\\',                       0},
    {"backspace",   STAGE_KEYCODE_BACKSPACE,    0},
    {"tab",         STAGE_KEYCODE_TAB,          0},
    {"q",           STAGE_KEYCODE_q,            0},
    {"w",           STAGE_KEYCODE_w,            0},
    {"e",           STAGE_KEYCODE_e,            0},
    {"r",           STAGE_KEYCODE_r,            0},
    {"t",           STAGE_KEYCODE_t,            0},
    {"y",           STAGE_KEYCODE_y,            0},
    {"u",           STAGE_KEYCODE_u,            0},
    {"i",           STAGE_KEYCODE_i,            0},
    {"o",           STAGE_KEYCODE_o,            0},
    {"p",           STAGE_KEYCODE_p,            0},
    {"[",           '[',                        0},
    {"]",           ']',                        0},
    {"ent",         STAGE_KEYCODE_RETURN,       0},
    {"caps",        STAGE_KEYCODE_CAPSLOCK,     0},
    {"a",           STAGE_KEYCODE_a,            0},
    {"s",           STAGE_KEYCODE_s,            0},
    {"d",           STAGE_KEYCODE_d,            0},
    {"f",           STAGE_KEYCODE_f,            0},
    {"g",           STAGE_KEYCODE_g,            0},
    {"h",           STAGE_KEYCODE_h,            0},
    {"j",           STAGE_KEYCODE_j,            0},
    {"k",           STAGE_KEYCODE_k,            0},
    {"l",           STAGE_KEYCODE_l,            0},
    {";",           ';',                        0},
    {"'",           '\'',                       0},
    {"lshift",      STAGE_KEYCODE_LSHIFT,       0},
    {"z",           STAGE_KEYCODE_z,            0},
    {"x",           STAGE_KEYCODE_x,            0},
    {"c",           STAGE_KEYCODE_c,            0},
    {"v",           STAGE_KEYCODE_v,            0},
    {"b",           STAGE_KEYCODE_b,            0},
    {"n",           STAGE_KEYCODE_n,            0},
    {"m",           STAGE_KEYCODE_m,            0},
    {",",           ',',                        0},
    {".",           '.',                        0},
    {"/",           '/',                        0},
    {"rshift",      STAGE_KEYCODE_RSHIFT,       0},
    {"lctrl",       STAGE_KEYCODE_LCTRL,        0},
    {"lsuper",      STAGE_KEYCODE_LSUPER,       0},
    {"lalt",        STAGE_KEYCODE_LALT,         0},
    {"spc",         STAGE_KEYCODE_SPACE,        0},
    {"ralt",        STAGE_KEYCODE_RALT,         0},
    {"rsuper",      STAGE_KEYCODE_RSUPER,       0},
    {"rctrl",       STAGE_KEYCODE_RCTRL,        0},
    {"up",          STAGE_KEYCODE_UP,           0},
    {"down",        STAGE_KEYCODE_DOWN,         0},
    {"left",        STAGE_KEYCODE_LEFT,         0},
    {"right",       STAGE_KEYCODE_RIGHT,        0},
    {"shift",       STAGE_KEYCODE_LSHIFT,       STAGE_KEYCODE_RSHIFT},
    {"ctrl",        STAGE_KEYCODE_LCTRL,        STAGE_KEYCODE_RCTRL},
    {"alt",         STAGE_KEYCODE_LALT,         STAGE_KEYCODE_RALT},
    {"numlock",     STAGE_KEYCODE_NUMLOCK,      0},
    {"",            0,                          0}
};

s_CfgZxKey cfgZxKeys[] = {
    {"spc", 7,  0b00001,    0},
    {"ss",  7,  0b00010,    1},
    {"m",   7,  0b00100,    2},
    {"n",   7,  0b01000,    3},
    {"b",   7,  0b10000,    4},

    {"ent", 6,  0b00001,    5},
    {"l",   6,  0b00010,    6},
    {"k",   6,  0b00100,    7},
    {"j",   6,  0b01000,    8},
    {"h",   6,  0b10000,    9},

    {"p",   5,  0b00001,    10},
    {"o",   5,  0b00010,    11},
    {"i",   5,  0b00100,    12},
    {"u",   5,  0b01000,    13},
    {"y",   5,  0b10000,    14},

    {"0",   4,  0b00001,    15},
    {"9",   4,  0b00010,    16},
    {"8",   4,  0b00100,    17},
    {"7",   4,  0b01000,    18},
    {"6",   4,  0b10000,    19},

    {"1",   3,  0b00001,    20},
    {"2",   3,  0b00010,    21},
    {"3",   3,  0b00100,    22},
    {"4",   3,  0b01000,    23},
    {"5",   3,  0b10000,    24},

    {"q",   2,  0b00001,    25},
    {"w",   2,  0b00010,    26},
    {"e",   2,  0b00100,    27},
    {"r",   2,  0b01000,    28},
    {"t",   2,  0b10000,    29},

    {"a",   1,  0b00001,    30},
    {"s",   1,  0b00010,    31},
    {"d",   1,  0b00100,    32},
    {"f",   1,  0b01000,    33},
    {"g",   1,  0b10000,    34},

    {"cs",  0,  0b00001,    35},
    {"z",   0,  0b00010,    36},
    {"x",   0,  0b00100,    37},
    {"c",   0,  0b01000,    38},
    {"v",   0,  0b10000,    39},

    {"",    0,  0,          0}
};

#endif
