#ifndef _CPU_TRACE_H_INCLUDED_
#define _CPU_TRACE_H_INCLUDED_

#include "zemu.h"

void CpuTrace_Init(void);
void CpuTrace_Log(void);
void CpuTrace_Close(void);

extern int cpuTrace_dT;
extern int cpuTrace_intReq;

#endif
