#ifndef _SNAP_Z80_H_INCLUDED_
#define _SNAP_Z80_H_INCLUDED_

#include "devices/mmanager/mmanager.h"
#include "devices/border/border.h"

bool load_z80_snap(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border);
bool save_z80_snap(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border);

#endif
