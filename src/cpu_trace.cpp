#include "cpu_trace.h"
#include "file.h"
#include "devs.h"

static C_File traceFile;
int cpuTrace_dT = 0;

void CpuTrace_Init(void)
{
	if (params.cpuTraceEnabled && *params.cpuTraceFormat && *params.cpuTraceFileName) {
		traceFile.Write(params.cpuTraceFileName);
	}
}

static void CpuTrace_PutByte(BYTE val)
{
	traceFile.PrintF("%02X", val);
}

static void CpuTrace_PutWord(WORD val)
{
	traceFile.PrintF("%04X", val);
}

/*
 * TODO: Add AF' .. HL', IFF1, IFF2, IM
 */
void CpuTrace_Log(void)
{
	if (!params.cpuTraceEnabled || !(*params.cpuTraceFormat) || !(*params.cpuTraceFileName)) return;

	char *formatPtr = params.cpuTraceFormat;
	char str[MAX_TRACE_FORMAT];

	while (*formatPtr)
	{
		if (*formatPtr == '[')
		{
			formatPtr++;

			char *strPtr = str;
			while (*formatPtr && *formatPtr!=']') *(strPtr++) = (*formatPtr++);
			*strPtr = 0;

			if (*formatPtr == ']')
			{
				formatPtr++;

				if (!strcmp(str, "I")) CpuTrace_PutByte(z80ex_get_reg(cpu, regI));
				else
				if (!strcmp(str, "R")) CpuTrace_PutByte(z80ex_get_reg(cpu, regR));
				else
				if (!strcmp(str, "PC")) CpuTrace_PutWord(z80ex_get_reg(cpu, regPC));
				else
				if (!strcmp(str, "AF")) CpuTrace_PutWord(z80ex_get_reg(cpu, regAF));
				else
				if (!strcmp(str, "BC")) CpuTrace_PutWord(z80ex_get_reg(cpu, regBC));
				else
				if (!strcmp(str, "DE")) CpuTrace_PutWord(z80ex_get_reg(cpu, regDE));
				else
				if (!strcmp(str, "HL")) CpuTrace_PutWord(z80ex_get_reg(cpu, regHL));
				else
				if (!strcmp(str, "IX")) CpuTrace_PutWord(z80ex_get_reg(cpu, regIX));
				else
				if (!strcmp(str, "IY")) CpuTrace_PutWord(z80ex_get_reg(cpu, regIY));
				else
				if (!strcmp(str, "SP")) CpuTrace_PutWord(z80ex_get_reg(cpu, regSP));
				else
				if (!strcmp(str, "DT")) CpuTrace_PutByte(cpuTrace_dT);
				else
				if (!strcmp(str, "M1")) CpuTrace_PutByte(ReadByteDasm(z80ex_get_reg(cpu, regPC), NULL));
				else
				if (!strcmp(str, "M2")) CpuTrace_PutByte(ReadByteDasm(z80ex_get_reg(cpu, regPC) + 1, NULL));
				else traceFile.PrintF("[%s]", str);
			}
			else traceFile.PrintF("[%s", str);
		}
		else traceFile.PutC(*(formatPtr++));
	}

	traceFile.PutC('\n');
}

void CpuTrace_Close(void)
{
	if (params.cpuTraceEnabled && *params.cpuTraceFormat && *params.cpuTraceFileName) {
		traceFile.Close();
	}
}
