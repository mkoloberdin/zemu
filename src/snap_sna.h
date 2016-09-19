#ifndef _SNAP_SNA_H_INCLUDED_
#define _SNAP_SNA_H_INCLUDED_

#include <boost/filesystem.hpp>
#include "devices/mmanager/mmanager.h"
#include "devices/border/border.h"
#include "devices/trdos/trdos.h"

namespace fs = boost::filesystem;

bool loadSnaSnap(const fs::path& filename, Z80EX_CONTEXT *cpu, C_MemoryManager& mmgr,
                 C_Border& border);
void saveSnaSnap(const fs::path& filename, Z80EX_CONTEXT *cpu, C_MemoryManager& mmgr,
                 C_Border& border);

#endif
