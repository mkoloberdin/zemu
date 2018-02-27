#ifndef _LABELS_H_INCLUDED_
#define _LABELS_H_INCLUDED_

#include <cstdint>
#include "zemu.h"
#include <string>
#include <map>

/*
 * TODO: Do it via static class
 */

extern std::map<uint16_t, std::string> labels;

void Labels_Load(const char *fname);

#endif



