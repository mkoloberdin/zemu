#ifndef _TRACE_H_INCLUDED_
#define _TRACE_H_INCLUDED_ 1

#include "zemu.h"

/*
 * TODO: Do it via static class
 */

void CpuTrace_Init(void);
void CpuTrace_Log(void);
void CpuTrace_Close(void);

extern int cpuTrace_dT;

#endif
