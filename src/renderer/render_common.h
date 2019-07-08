#ifndef _RENDER_COMMON_H_INCLUDED_
#define _RENDER_COMMON_H_INCLUDED_

#include "../zemu.h"
#include "../devs.h"

// TODO: add scorpion support (http://www.worldofspectrum.org/rusfaq/index.html)

#define SCREEN_VERT_OFFSET_LINES 32
#define SCREEN_HOR_OFFSET_TACTS 20
#define SCREEN_TACTS_PER_PIXEL 2
#define SCREEN_MAGIC_TACTS 2

#define SCREEN_LINE_TACTS 224
#define SCREEN_HRET_TACTS 32
#define SCREEN_VRET_LINES 16

#define SCREEN_START (SCREEN_LINE_TACTS * (SCREEN_VRET_LINES + SCREEN_VERT_OFFSET_LINES) + SCREEN_MAGIC_TACTS)
#define SCREEN_END (SCREEN_START + HEIGHT * SCREEN_LINE_TACTS)

#endif
