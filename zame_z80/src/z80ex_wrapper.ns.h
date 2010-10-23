#ifndef _Z80EX_WRAPPER_H_INCLUDED
#define _Z80EX_WRAPPER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

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
	regBC = 0,
	regDE = 1,
	regHL = 2,
	regAF = 3,
	regIX = 4,
	regIY = 5,
	regSP = 6,
	regPC = 7,
	regMP = 8,
	regBC_ = 9,
	regDE_ = 10,
	regHL_ = 11,
	regAF_ = 12,
	regI = 13,
	regR = 14,
	regIFF1 = 15,
	regIFF2 = 16,
	regIM = 17
} Z80_REG_T;

enum Z80EX_DASM_FLAGS {
	WORDS_DEC = 1,
	BYTES_DEC = 2
};

typedef Z80EX_BYTE (*z80ex_mread_cb)(Z80EX_WORD addr, bool m1_state, void *user_data);
typedef void (*z80ex_mwrite_cb)(Z80EX_WORD addr, Z80EX_BYTE value, void *user_data);
typedef Z80EX_BYTE (*z80ex_pread_cb)(Z80EX_WORD port, void *user_data);
typedef void (*z80ex_pwrite_cb)(Z80EX_WORD port, Z80EX_BYTE value, void *user_data);
typedef Z80EX_BYTE (*z80ex_intread_cb)(void *user_data);
typedef Z80EX_BYTE (*z80ex_dasm_readbyte_cb)(Z80EX_WORD addr, void *user_data);

#ifndef CPU_ALREADY_DEFINED
	struct s_Cpu
	{
		Z80EX_WORD regs[18];

		bool is_halted;
		bool is_opcode;
		bool is_noint;
		bool is_reset_pv;
		bool is_read_int;

		z80ex_mread_cb ptr_read;
		z80ex_mwrite_cb ptr_write;
		z80ex_pread_cb ptr_in;
		z80ex_pwrite_cb ptr_out;
		z80ex_intread_cb ptr_read_int;

		void *data_read;
		void *data_write;
		void *data_in;
		void *data_out;
		void *data_read_int;

		unsigned (* tick)(struct s_Cpu *self);
		void * (* optable)(struct s_Cpu *self);
		Z80EX_BYTE prefix;
		unsigned long tstate;

		int8_t tmp_int8;
		int16_t tmp_int16;
		int32_t tmp_int32;
		Z80EX_BYTE tmp_byte;
		Z80EX_BYTE tmp_byte_b;
		Z80EX_WORD tmp_word;
		Z80EX_WORD tmp_word_b;
		Z80EX_DWORD tmp_dword;
	};
#endif

typedef struct s_Cpu Z80EX_CONTEXT;

extern s_Cpu * Cpu::new(
	z80ex_mread_cb mrcb_fn, void *mrcb_data,
	z80ex_mwrite_cb mwcb_fn, void *mwcb_data,
	z80ex_pread_cb prcb_fn, void *prcb_data,
	z80ex_pwrite_cb pwcb_fn, void *pwcb_data,
	z80ex_intread_cb ircb_fn, void *ircb_data
);

extern void Cpu::free(s_Cpu *self);
extern unsigned Cpu::do_int(s_Cpu *self);
extern void Cpu::reset(s_Cpu *self);
extern Z80EX_WORD Cpu::get_reg(s_Cpu *self, int reg);
extern void Cpu::set_reg(s_Cpu *self, int reg, Z80EX_WORD value);

#define z80ex_create Cpu::new
#define z80ex_destroy Cpu::free
#define z80ex_int Cpu::do_int
#define z80ex_reset Cpu::reset
#define z80ex_get_reg Cpu::get_reg
#define z80ex_set_reg Cpu::set_reg

extern unsigned z80ex_step(Z80EX_CONTEXT *cpu);

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
