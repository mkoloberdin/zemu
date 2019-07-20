#ifndef STAGE__KEYCODE_H__INCLUDED
#define STAGE__KEYCODE_H__INCLUDED

#include "ZEmuConfig.h"

#ifdef USE_SDL1
    #include <SDL_keysym.h>
#else
    #include <SDL_keycode.h>
#endif

#define STAGE_KEYCODE_ESCAPE SDLK_ESCAPE
#define STAGE_KEYCODE_F1 SDLK_F1
#define STAGE_KEYCODE_F2 SDLK_F2
#define STAGE_KEYCODE_F3 SDLK_F3
#define STAGE_KEYCODE_F4 SDLK_F4
#define STAGE_KEYCODE_F5 SDLK_F5
#define STAGE_KEYCODE_F6 SDLK_F6
#define STAGE_KEYCODE_F7 SDLK_F7
#define STAGE_KEYCODE_F8 SDLK_F8
#define STAGE_KEYCODE_F9 SDLK_F9
#define STAGE_KEYCODE_F10 SDLK_F10
#define STAGE_KEYCODE_F11 SDLK_F11
#define STAGE_KEYCODE_F12 SDLK_F12
#define STAGE_KEYCODE_PAUSE SDLK_PAUSE
#define STAGE_KEYCODE_INSERT SDLK_INSERT
#define STAGE_KEYCODE_HOME SDLK_HOME
#define STAGE_KEYCODE_DELETE SDLK_DELETE
#define STAGE_KEYCODE_END SDLK_END
#define STAGE_KEYCODE_PAGEUP SDLK_PAGEUP
#define STAGE_KEYCODE_PAGEDOWN SDLK_PAGEDOWN
#define STAGE_KEYCODE_1 SDLK_1
#define STAGE_KEYCODE_2 SDLK_2
#define STAGE_KEYCODE_3 SDLK_3
#define STAGE_KEYCODE_4 SDLK_4
#define STAGE_KEYCODE_5 SDLK_5
#define STAGE_KEYCODE_6 SDLK_6
#define STAGE_KEYCODE_7 SDLK_7
#define STAGE_KEYCODE_8 SDLK_8
#define STAGE_KEYCODE_9 SDLK_9
#define STAGE_KEYCODE_0 SDLK_0
#define STAGE_KEYCODE_BACKSPACE SDLK_BACKSPACE
#define STAGE_KEYCODE_TAB SDLK_TAB
#define STAGE_KEYCODE_q SDLK_q
#define STAGE_KEYCODE_w SDLK_w
#define STAGE_KEYCODE_e SDLK_e
#define STAGE_KEYCODE_r SDLK_r
#define STAGE_KEYCODE_t SDLK_t
#define STAGE_KEYCODE_y SDLK_y
#define STAGE_KEYCODE_u SDLK_u
#define STAGE_KEYCODE_i SDLK_i
#define STAGE_KEYCODE_o SDLK_o
#define STAGE_KEYCODE_p SDLK_p
#define STAGE_KEYCODE_RETURN SDLK_RETURN
#define STAGE_KEYCODE_CAPSLOCK SDLK_CAPSLOCK
#define STAGE_KEYCODE_a SDLK_a
#define STAGE_KEYCODE_s SDLK_s
#define STAGE_KEYCODE_d SDLK_d
#define STAGE_KEYCODE_f SDLK_f
#define STAGE_KEYCODE_g SDLK_g
#define STAGE_KEYCODE_h SDLK_h
#define STAGE_KEYCODE_j SDLK_j
#define STAGE_KEYCODE_k SDLK_k
#define STAGE_KEYCODE_l SDLK_l
#define STAGE_KEYCODE_LSHIFT SDLK_LSHIFT
#define STAGE_KEYCODE_z SDLK_z
#define STAGE_KEYCODE_x SDLK_x
#define STAGE_KEYCODE_c SDLK_c
#define STAGE_KEYCODE_v SDLK_v
#define STAGE_KEYCODE_b SDLK_b
#define STAGE_KEYCODE_n SDLK_n
#define STAGE_KEYCODE_m SDLK_m
#define STAGE_KEYCODE_RSHIFT SDLK_RSHIFT
#define STAGE_KEYCODE_LCTRL SDLK_LCTRL
#define STAGE_KEYCODE_LALT SDLK_LALT
#define STAGE_KEYCODE_SPACE SDLK_SPACE
#define STAGE_KEYCODE_RALT SDLK_RALT
#define STAGE_KEYCODE_RCTRL SDLK_RCTRL
#define STAGE_KEYCODE_UP SDLK_UP
#define STAGE_KEYCODE_DOWN SDLK_DOWN
#define STAGE_KEYCODE_LEFT SDLK_LEFT
#define STAGE_KEYCODE_RIGHT SDLK_RIGHT

#ifdef USE_SDL1
    #define STAGE_KEYCODE_LSUPER SDLK_LSUPER
    #define STAGE_KEYCODE_RSUPER SDLK_RSUPER
    #define STAGE_KEYCODE_NUMLOCK SDLK_NUMLOCK
#else
    #define STAGE_KEYCODE_LSUPER SDLK_LGUI
    #define STAGE_KEYCODE_RSUPER SDLK_RGUI
    #define STAGE_KEYCODE_NUMLOCK SDLK_NUMLOCKCLEAR
#endif

#endif