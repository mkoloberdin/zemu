#ifndef _ZEMU_ENV_H_INCLUDED_
#define _ZEMU_ENV_H_INCLUDED_

#include <z80ex.h>
#include "defines.h"
#include "appenv.h"

#ifndef Z80EX_ZAME_WRAPPER
  #define Z80EX_CONTEXT_PARAM Z80EX_CONTEXT *cpu,
#else
  #define Z80EX_CONTEXT_PARAM
#endif

extern AppEnv env;

#endif // _ZEMU_ENV_H_INCLUDED_
