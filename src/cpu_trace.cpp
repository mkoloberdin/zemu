#include <cstdint>
#include <boost/format.hpp>
#include <zemu_env.h>
#include "cpu_trace.h"
#include "devs.h"

using boost::format;

static fs::ofstream TraceOfs;
int cpuTrace_dT = 0;
int cpuTrace_intReq = 0;

void CpuTrace_Init(void)
{
  if (params.cpuTraceEnabled && *params.cpuTraceFormat && *params.cpuTraceFileName) {
    TraceOfs.open(env.newDataFilePath(params.cpuTraceFileName), std::ios_base::trunc);
  }
}

static void CpuTrace_PutByte(uint8_t val)
{
  TraceOfs << format("%02X") % val;
}

static void CpuTrace_PutWord(uint16_t val)
{
  TraceOfs << format("%04X") % val;
}

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
      while (*formatPtr && *formatPtr != ']') *(strPtr++) = (*formatPtr++);
      *strPtr = 0;

      if (*formatPtr == ']')
      {
        formatPtr++;

        if (!strcmp(str, "I")) CpuTrace_PutByte(z80ex_get_reg(cpu, regI));
        else if (!strcmp(str, "R")) CpuTrace_PutByte(z80ex_get_reg(cpu, regR));
        else if (!strcmp(str, "PC")) CpuTrace_PutWord(z80ex_get_reg(cpu, regPC));
        else if (!strcmp(str, "AF")) CpuTrace_PutWord(z80ex_get_reg(cpu, regAF));
        else if (!strcmp(str, "BC")) CpuTrace_PutWord(z80ex_get_reg(cpu, regBC));
        else if (!strcmp(str, "DE")) CpuTrace_PutWord(z80ex_get_reg(cpu, regDE));
        else if (!strcmp(str, "HL")) CpuTrace_PutWord(z80ex_get_reg(cpu, regHL));
        else if (!strcmp(str, "AF'")) CpuTrace_PutWord(z80ex_get_reg(cpu, regAF_));
        else if (!strcmp(str, "BC'")) CpuTrace_PutWord(z80ex_get_reg(cpu, regBC_));
        else if (!strcmp(str, "DE'")) CpuTrace_PutWord(z80ex_get_reg(cpu, regDE_));
        else if (!strcmp(str, "HL'")) CpuTrace_PutWord(z80ex_get_reg(cpu, regHL_));
        else if (!strcmp(str, "IX")) CpuTrace_PutWord(z80ex_get_reg(cpu, regIX));
        else if (!strcmp(str, "IY")) CpuTrace_PutWord(z80ex_get_reg(cpu, regIY));
        else if (!strcmp(str, "SP")) CpuTrace_PutWord(z80ex_get_reg(cpu, regSP));
        else if (!strcmp(str, "DT")) CpuTrace_PutByte(cpuTrace_dT);
        else if (!strcmp(str, "INTR")) CpuTrace_PutByte(cpuTrace_intReq);
        else if (!strcmp(str, "IFF1")) CpuTrace_PutByte(z80ex_get_reg(cpu, regIFF1));
        else if (!strcmp(str, "IFF2")) CpuTrace_PutByte(z80ex_get_reg(cpu, regIFF2));
        else if (!strcmp(str, "IM")) CpuTrace_PutByte(z80ex_get_reg(cpu, regIM));
        else if (!strcmp(str, "M1"))
          CpuTrace_PutByte(ReadByteDasm(z80ex_get_reg(cpu, regPC), nullptr));
        else if (!strcmp(str, "M2"))
          CpuTrace_PutByte(ReadByteDasm(z80ex_get_reg(cpu, regPC) + 1, nullptr));
        else if (!strcmp(str, "M3"))
          CpuTrace_PutByte(ReadByteDasm(z80ex_get_reg(cpu, regPC) + 2, nullptr));
        else if (!strcmp(str, "M4"))
          CpuTrace_PutByte(ReadByteDasm(z80ex_get_reg(cpu, regPC) + 3, nullptr));
        else TraceOfs << "[" << str << "]";
      }
      else TraceOfs << "[" << str;
    }
    else TraceOfs << *(formatPtr++);
  }

  TraceOfs << std::endl;
}

void CpuTrace_Close(void)
{
  if (params.cpuTraceEnabled && *params.cpuTraceFormat && *params.cpuTraceFileName) {
    TraceOfs.close();
  }
}
