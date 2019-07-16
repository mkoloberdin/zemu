#ifndef _LABELS_H_INCLUDED_
#define _LABELS_H_INCLUDED_

#include "zemu.h"
#include <string>
#include <map>

extern std::map<uint16_t, std::string> labels;
void Labels_Load(const char* fname);

#endif
