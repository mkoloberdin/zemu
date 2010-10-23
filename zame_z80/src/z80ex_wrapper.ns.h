#ifndef _Z80EX_WRAPPER_H_INCLUDED
#define _Z80EX_WRAPPER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
	#define LIB_EXPORT __declspec(dllexport)
#else
	#define LIB_EXPORT
#endif

#if defined(__SYMBIAN32__)
	typedef unsigned char Z80EX_BYTE;
	typedef signed char Z80EX_SIGNED_BYTE;
	typedef unsigned short Z80EX_WORD;
	typedef unsigned int Z80EX_DWORD;
#elif defined(__GNUC__)
	#include <stdint.h>
	typedef uint8_t Z80EX_BYTE;
	typedef int8_t Z80EX_SIGNED_BYTE;
	typedef uint16_t Z80EX_WORD;
	typedef uint32_t Z80EX_DWORD;
#elif defined(_MSC_VER)
	typedef unsigned __int8 Z80EX_BYTE;
	typedef signed __int8 Z80EX_SIGNED_BYTE;
	typedef unsigned __int16 Z80EX_WORD;
	typedef unsigned __int32 Z80EX_DWORD;
#else
	typedef unsigned char Z80EX_BYTE;
	typedef signed char Z80EX_SIGNED_BYTE;
	typedef unsigned short Z80EX_WORD;
	typedef unsigned int Z80EX_DWORD;
#endif

typedef enum {
	regAF,
	regBC,
	regDE,
	regHL,
	regAF_,
	regBC_,
	regDE_,
	regHL_,
	regIX,
	regIY,
	regPC,
	regSP,
	regI,
	regR,
	regR7,
	regIM /* 0,1 or 2 */,
	regIFF1,
	regIFF2
} Z80_REG_T;

enum Z80EX_DASM_FLAGS {
	WORDS_DEC = 1,
	BYTES_DEC = 2
};

struct s_ContextWrapper;
typedef struct s_ContextWrapper Z80EX_CONTEXT;

typedef Z80EX_BYTE (*z80ex_mread_cb)(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *user_data);
typedef void (*z80ex_mwrite_cb)(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *user_data);
typedef Z80EX_BYTE (*z80ex_pread_cb)(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *user_data);
typedef void (*z80ex_pwrite_cb)(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user_data);
typedef Z80EX_BYTE (*z80ex_intread_cb)(Z80EX_CONTEXT *cpu, void *user_data);
typedef Z80EX_BYTE (*z80ex_dasm_readbyte_cb)(Z80EX_WORD addr, void *user_data);

extern Z80EX_CONTEXT *z80ex_create(
	z80ex_mread_cb mrcb_fn, void *mrcb_data,
	z80ex_mwrite_cb mwcb_fn, void *mwcb_data,
	z80ex_pread_cb prcb_fn, void *prcb_data,
	z80ex_pwrite_cb pwcb_fn, void *pwcb_data,
	z80ex_intread_cb ircb_fn, void *ircb_data
);

extern void z80ex_destroy(Z80EX_CONTEXT *cpu);
extern int z80ex_step(Z80EX_CONTEXT *cpu);
extern Z80EX_BYTE z80ex_last_op_type(Z80EX_CONTEXT *cpu);
extern int z80ex_int(Z80EX_CONTEXT *cpu);
extern void z80ex_reset(Z80EX_CONTEXT *cpu);
extern Z80EX_WORD z80ex_get_reg(Z80EX_CONTEXT *cpu, Z80_REG_T reg);
extern void z80ex_set_reg(Z80EX_CONTEXT *cpu, Z80_REG_T reg, Z80EX_WORD value);
extern int z80ex_int_possible(Z80EX_CONTEXT *cpu);

extern int z80ex_dasm(
	char *output,
	int output_size,
	unsigned flags,
	int *t_states,
	int *t_states2,
	z80ex_dasm_readbyte_cb readbyte_cb,
	Z80EX_WORD addr,
	void *user_data
);

#ifdef __cplusplus
}
#endif

#endif
