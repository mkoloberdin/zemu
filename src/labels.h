#ifndef _LABELS_H_INCLUDED_
#define _LABELS_H_INCLUDED_

#include "zemu.h"
#include <string>
#include <list>

/*
 * TODO: Do it via static class
 */

struct s_LabelItem
{
	WORD addr;
	std::string label;
};

extern std::list<s_LabelItem> labels;
void Labels_Load(const char *fname);

#endif
