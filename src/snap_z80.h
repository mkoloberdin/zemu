#ifndef _SNAP_Z80_H_INCLUDED_
#define _SNAP_Z80_H_INCLUDED_

#include <boost/filesystem.hpp>
#include "devices/mmanager/mmanager.h"
#include "devices/border/border.h"

namespace fs = boost::filesystem;

bool loadZ80Snap(const fs::path& filename, Z80EX_CONTEXT *cpu, C_MemoryManager& mmgr,
                   C_Border& border);
void saveZ80Snap(const fs::path& filename, Z80EX_CONTEXT *cpu, C_MemoryManager& mmgr,
                   C_Border& border);

#endif
