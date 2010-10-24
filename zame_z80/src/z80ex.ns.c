#include <stdio.h>
#include "lib/defs.h"
#include "lib_z80/cpu.h"

#define Z80EX_SELF_INCLUDE
#include "z80ex.h"
#include "z80ex_dasm.h"

#ifdef _MSC_VER
	#define snprintf _snprintf
#endif

int z80ex_step(Z80EX_CONTEXT *cpu)
{
	return (int)Cpu::tick(cpu);
}

extern int z80ex_int(Z80EX_CONTEXT *cpu)
{
	return (int)Cpu::do_int(cpu);
}

int z80ex_dasm(
	char *output,
	int output_size,
	unsigned flags,
	int *t_states,
	int *t_states2,
	z80ex_dasm_readbyte_cb readbyte_cb,
	Z80EX_WORD addr,
	void *user_data
) {
	Z80EX_BYTE val;
	*t_states = 1;
	*t_states2 = 0;

	val = readbyte_cb(addr, user_data);
	snprintf(output, output_size, "%02X [todo]", val);

	return 1;
}
