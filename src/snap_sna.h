#ifndef _SNAP_SNA_H_INCLUDED_
#define _SNAP_SNA_H_INCLUDED_

#include "devices/mmanager/mmanager.h"
#include "devices/border/border.h"
#include "devices/trdos/trdos.h"

bool load_sna_snap(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border);
void save_sna_snap(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border);

#endif
