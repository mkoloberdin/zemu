#include <stdio.h>
#define Z80EX_WRAPPER_SELF_INCLUDE
#include "z80ex_wrapper.h"
#include "lib/defs.h"
#include "lib_z80/cpu.h"

#ifdef _MSC_VER
	#define snprintf _snprintf
#endif

typedef struct s_ContextWrapper
{
	s_Cpu *cpu;

	z80ex_mread_cb mrcb_fn;
	z80ex_mwrite_cb mwcb_fn;
	z80ex_pread_cb prcb_fn;
	z80ex_pwrite_cb pwcb_fn;
	z80ex_intread_cb ircb_fn;

	void *mrcb_data;
	void *mwcb_data;
	void *prcb_data;
	void *pwcb_data;
	void *ircb_data;
} s_ContextWrapper;

byte wrap_read(word addr, bool m1, void *data)
{
	s_ContextWrapper *context = (s_ContextWrapper*)data;
	return context->mrcb_fn(context, addr, m1, context->mrcb_data);
}

void wrap_write(word addr, byte val, void *data)
{
	s_ContextWrapper *context = (s_ContextWrapper*)data;
	context->mwcb_fn(context, addr, val, context->mwcb_data);
}

byte wrap_in(word port, void *data)
{
	s_ContextWrapper *context = (s_ContextWrapper*)data;
	return context->prcb_fn(context, port, context->prcb_data);
}

void wrap_out(word port, byte val, void *data)
{
	s_ContextWrapper *context = (s_ContextWrapper*)data;
	context->pwcb_fn(context, port, val, context->pwcb_data);
}

byte wrap_int(void *data)
{
	s_ContextWrapper *context = (s_ContextWrapper*)data;
	return context->ircb_fn(context, context->ircb_data);
}

LIB_EXPORT Z80EX_CONTEXT *z80ex_create(
	z80ex_mread_cb mrcb_fn, void *mrcb_data,
	z80ex_mwrite_cb mwcb_fn, void *mwcb_data,
	z80ex_pread_cb prcb_fn, void *prcb_data,
	z80ex_pwrite_cb pwcb_fn, void *pwcb_data,
	z80ex_intread_cb ircb_fn, void *ircb_data
) {
	s_ContextWrapper *context = ALLOC_MEM(s_ContextWrapper);

	context->mrcb_fn = mrcb_fn;
	context->mwcb_fn = mwcb_fn;
	context->prcb_fn = prcb_fn;
	context->pwcb_fn = pwcb_fn;
	context->ircb_fn = ircb_fn;

	context->mrcb_data = mrcb_data;
	context->mwcb_data = mwcb_data;
	context->prcb_data = prcb_data;
	context->pwcb_data = pwcb_data;
	context->ircb_data = ircb_data;

	context->cpu = Cpu::new(
		wrap_read, context,
		wrap_write, context,
		wrap_in, context,
		wrap_out, context,
		wrap_int, context
	);

	return context;
}

LIB_EXPORT void z80ex_destroy(Z80EX_CONTEXT *context)
{
	Cpu::free(context->cpu);
	free(context);
}

LIB_EXPORT int z80ex_step(Z80EX_CONTEXT *context)
{
	return (int)Cpu::tick(context->cpu);
}

LIB_EXPORT Z80EX_BYTE z80ex_last_op_type(Z80EX_CONTEXT *context)
{
	return context->cpu->prefix;
}

LIB_EXPORT int z80ex_int(Z80EX_CONTEXT *context)
{
	return (int)Cpu::do_int(context->cpu);
}

LIB_EXPORT void z80ex_reset(Z80EX_CONTEXT *context)
{
	Cpu::reset(context->cpu);
}

LIB_EXPORT Z80EX_WORD z80ex_get_reg(Z80EX_CONTEXT *context, Z80_REG_T reg)
{
	switch (reg)
	{
		case regAF:
			return Cpu::get_reg(context->cpu, CPU_AF);

		case regBC:
			return Cpu::get_reg(context->cpu, CPU_BC);

		case regDE:
			return Cpu::get_reg(context->cpu, CPU_DE);

		case regHL:
			return Cpu::get_reg(context->cpu, CPU_HL);

		case regAF_:
			return Cpu::get_reg(context->cpu, CPU_AF_);

		case regBC_:
			return Cpu::get_reg(context->cpu, CPU_BC_);

		case regDE_:
			return Cpu::get_reg(context->cpu, CPU_DE_);

		case regHL_:
			return Cpu::get_reg(context->cpu, CPU_HL_);

		case regIX:
			return Cpu::get_reg(context->cpu, CPU_IX);

		case regIY:
			return Cpu::get_reg(context->cpu, CPU_IY);

		case regPC:
			return Cpu::get_reg(context->cpu, CPU_PC);

		case regSP:
			return Cpu::get_reg(context->cpu, CPU_SP);

		case regI:
			return Cpu::get_reg(context->cpu, CPU_I);

		case regR:
			return Cpu::get_reg(context->cpu, CPU_R);

		case regR7:
			return 0;

		case regIM:
			return Cpu::get_reg(context->cpu, CPU_IM);

		case regIFF1:
			return Cpu::get_reg(context->cpu, CPU_IFF1);

		case regIFF2:
			return Cpu::get_reg(context->cpu, CPU_IFF2);
	}

	return 0;
}

LIB_EXPORT void z80ex_set_reg(Z80EX_CONTEXT *context, Z80_REG_T reg, Z80EX_WORD value)
{
	switch (reg)
	{
		case regAF:
			Cpu::set_reg(context->cpu, CPU_AF, value);
			break;

		case regBC:
			Cpu::set_reg(context->cpu, CPU_BC, value);
			break;

		case regDE:
			Cpu::set_reg(context->cpu, CPU_DE, value);
			break;

		case regHL:
			Cpu::set_reg(context->cpu, CPU_HL, value);
			break;

		case regAF_:
			Cpu::set_reg(context->cpu, CPU_AF_, value);
			break;

		case regBC_:
			Cpu::set_reg(context->cpu, CPU_BC_, value);
			break;

		case regDE_:
			Cpu::set_reg(context->cpu, CPU_DE_, value);
			break;

		case regHL_:
			Cpu::set_reg(context->cpu, CPU_HL_, value);
			break;

		case regIX:
			Cpu::set_reg(context->cpu, CPU_IX, value);
			break;

		case regIY:
			Cpu::set_reg(context->cpu, CPU_IY, value);
			break;

		case regPC:
			Cpu::set_reg(context->cpu, CPU_PC, value);
			break;

		case regSP:
			Cpu::set_reg(context->cpu, CPU_SP, value);
			break;

		case regI:
			Cpu::set_reg(context->cpu, CPU_I, value);
			break;

		case regR:
			Cpu::set_reg(context->cpu, CPU_R, value);
			break;

		case regR7:
			break;

		case regIM:
			Cpu::set_reg(context->cpu, CPU_IM, value);
			break;

		case regIFF1:
			Cpu::set_reg(context->cpu, CPU_IFF1, value);
			break;

		case regIFF2:
			Cpu::set_reg(context->cpu, CPU_IFF2, value);
			break;
	}
}

LIB_EXPORT int z80ex_int_possible(Z80EX_CONTEXT *context)
{
	return !context->cpu->is_noint;
}

LIB_EXPORT int z80ex_dasm(
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
