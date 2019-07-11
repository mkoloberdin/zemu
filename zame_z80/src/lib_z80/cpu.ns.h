/*
 * MIT License (http://www.opensource.org/licenses/mit-license.php)
 *
 * Copyright (c) 2009-2019, Viachaslau Tratsiak (aka restorer)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "../lib/defs.h"

#define FLAG_C  (0x01) // carry
#define FLAG_N  (0x02) // subtract
#define FLAG_PV (0x04) // parity or overflow
#define FLAG_3  (0x08) // copy of bit 3
#define FLAG_H  (0x10) // half carry
#define FLAG_5  (0x20) // copy of bit 5
#define FLAG_Z  (0x40) // zero
#define FLAG_S  (0x80) // sign

#define FLAG_C_M16 (0x10000)
#define FLAG_C_S16 (0x10)
#define FLAG_C_M8 (0x100)
#define FLAG_C_S8 (8)

#define FLAG_H_M16 (0x1000)
#define FLAG_H_S16 (8)

#define FLAG_PV_C8 (0x80)
#define FLAG_PV_IFF_S8 (2)

#define FLAG_C_TO_H (4)
#define FLAG_H_TO_C (4)
#define FLAG_N_TO_5 (4)
#define FLAG_S_TO_N (6)

#ifdef IS_BIG_ENDIAN
    #define REG_LO (1)
    #define REG_HI (0)
#else
    #define REG_LO (0)
    #define REG_HI (1)
#endif

#define CPU_BC   (0)
#define CPU_DE   (1)
#define CPU_HL   (2)
#define CPU_AF   (3)
#define CPU_IX   (4)
#define CPU_IY   (5)
#define CPU_SP   (6)
#define CPU_PC   (7)
#define CPU_MP   (8)
#define CPU_BC_  (9)
#define CPU_DE_  (10)
#define CPU_HL_  (11)
#define CPU_AF_  (12)
#define CPU_I    (13)
#define CPU_R    (14)
#define CPU_IFF1 (15)
#define CPU_IFF2 (16)
#define CPU_IM   (17)
#define CPU_LAST (18)

#define REG_A(cpu)    (*(((byte*)cpu->regs) + (CPU_AF * 2) + REG_HI))
#define REG_F(cpu)    (*(((byte*)cpu->regs) + (CPU_AF * 2) + REG_LO))
#define REG_B(cpu)    (*(((byte*)cpu->regs) + (CPU_BC * 2) + REG_HI))
#define REG_C(cpu)    (*(((byte*)cpu->regs) + (CPU_BC * 2) + REG_LO))
#define REG_D(cpu)    (*(((byte*)cpu->regs) + (CPU_DE * 2) + REG_HI))
#define REG_E(cpu)    (*(((byte*)cpu->regs) + (CPU_DE * 2) + REG_LO))
#define REG_H(cpu)    (*(((byte*)cpu->regs) + (CPU_HL * 2) + REG_HI))
#define REG_L(cpu)    (*(((byte*)cpu->regs) + (CPU_HL * 2) + REG_LO))
#define REG_IXH(cpu)  (*(((byte*)cpu->regs) + (CPU_IX * 2) + REG_HI))
#define REG_IXL(cpu)  (*(((byte*)cpu->regs) + (CPU_IX * 2) + REG_LO))
#define REG_IYH(cpu)  (*(((byte*)cpu->regs) + (CPU_IY * 2) + REG_HI))
#define REG_IYL(cpu)  (*(((byte*)cpu->regs) + (CPU_IY * 2) + REG_LO))
#define REG_SPH(cpu)  (*(((byte*)cpu->regs) + (CPU_SP * 2) + REG_HI))
#define REG_SPL(cpu)  (*(((byte*)cpu->regs) + (CPU_SP * 2) + REG_LO))
#define REG_PCH(cpu)  (*(((byte*)cpu->regs) + (CPU_PC * 2) + REG_HI))
#define REG_PCL(cpu)  (*(((byte*)cpu->regs) + (CPU_PC * 2) + REG_LO))
#define REG_MPH(cpu)  (*(((byte*)cpu->regs) + (CPU_MP * 2) + REG_HI))
#define REG_MPL(cpu)  (*(((byte*)cpu->regs) + (CPU_MP * 2) + REG_LO))
#define REG_A_(cpu)   (*(((byte*)cpu->regs) + (CPU_AF_ * 2) + REG_HI))
#define REG_F_(cpu)   (*(((byte*)cpu->regs) + (CPU_AF_ * 2) + REG_LO))
#define REG_B_(cpu)   (*(((byte*)cpu->regs) + (CPU_BC_ * 2) + REG_HI))
#define REG_C_(cpu)   (*(((byte*)cpu->regs) + (CPU_BC_ * 2) + REG_LO))
#define REG_D_(cpu)   (*(((byte*)cpu->regs) + (CPU_DE_ * 2) + REG_HI))
#define REG_E_(cpu)   (*(((byte*)cpu->regs) + (CPU_DE_ * 2) + REG_LO))
#define REG_H_(cpu)   (*(((byte*)cpu->regs) + (CPU_HL_ * 2) + REG_HI))
#define REG_L_(cpu)   (*(((byte*)cpu->regs) + (CPU_HL_ * 2) + REG_LO))
#define REG_AF(cpu)   (*(cpu->regs + CPU_AF))
#define REG_BC(cpu)   (*(cpu->regs + CPU_BC))
#define REG_DE(cpu)   (*(cpu->regs + CPU_DE))
#define REG_HL(cpu)   (*(cpu->regs + CPU_HL))
#define REG_IX(cpu)   (*(cpu->regs + CPU_IX))
#define REG_IY(cpu)   (*(cpu->regs + CPU_IY))
#define REG_SP(cpu)   (*(cpu->regs + CPU_SP))
#define REG_PC(cpu)   (*(cpu->regs + CPU_PC))
#define REG_MP(cpu)   (*(cpu->regs + CPU_MP))
#define REG_AF_(cpu)  (*(cpu->regs + CPU_AF_))
#define REG_BC_(cpu)  (*(cpu->regs + CPU_BC_))
#define REG_DE_(cpu)  (*(cpu->regs + CPU_DE_))
#define REG_HL_(cpu)  (*(cpu->regs + CPU_HL_))
#define REG_I(cpu)    (*(((byte*)cpu->regs) + (CPU_I * 2) + REG_LO))
#define REG_R(cpu)    (*(((byte*)cpu->regs) + (CPU_R * 2) + REG_LO))
#define REG_IFF1(cpu) (*(((byte*)cpu->regs) + (CPU_IFF1 * 2) + REG_LO))
#define REG_IFF2(cpu) (*(((byte*)cpu->regs) + (CPU_IFF2 * 2) + REG_LO))
#define REG_IM(cpu)   (*(((byte*)cpu->regs) + (CPU_IM * 2) + REG_LO))

#define CPU_READ_BYTE(cpu) (cpu->is_read_int \
    ? cpu->ptr_read_int(cpu->data_read_int) \
    : cpu->ptr_read(REG_PC(cpu)++, false, cpu->data_read))

#define CPU_READ_OFFSET(cpu) ((int8_t)CPU_READ_BYTE(cpu))

#define CPU_TMPL(cpu) (*(((byte*) &(cpu->tmp_word)) + REG_LO))
#define CPU_TMPH(cpu) (*(((byte*) &(cpu->tmp_word)) + REG_HI))

#define CPU_TMPL_B(cpu) (*(((byte*) &(cpu->tmp_word_b)) + REG_LO))
#define CPU_TMPH_B(cpu) (*(((byte*) &(cpu->tmp_word_b)) + REG_HI))

#define CPU_DO_READ_WORD(cpu) \
    CPU_TMPL(cpu) = CPU_READ_BYTE(cpu); \
    CPU_TMPH(cpu) = CPU_READ_BYTE(cpu);

#define CPU_DO_READ_OFFSET(cpu, RP) \
    cpu->tmp_int8 = CPU_READ_OFFSET(cpu); \
    REG_MP(cpu) = RP(cpu) + cpu->tmp_int8;

#define CPU_INC_R(cpu) \
    REG_R(cpu) = (REG_R(cpu) & 0x80) | ((REG_R(cpu) + 1) & 0x7F);

struct s_Cpu;

#namespace Cpu
    typedef byte (* ::t_read)(word addr, bool m1, void* data);
    typedef void (* ::t_write)(word addr, byte val, void* data);
    typedef byte (* ::t_in)(word port, void* data);
    typedef void (* ::t_out)(word port, byte val, void* data);
    typedef byte (* ::t_read_int)(void* data);
    typedef void (* ::t_opcode)(struct s_Cpu* self);
#end

typedef struct s_Cpu {
    word regs[CPU_LAST];

    bool is_halted;
    bool is_opcode;
    bool is_noint;
    bool is_reset_pv;
    bool is_read_int;

    Cpu::t_read ptr_read;
    Cpu::t_write ptr_write;
    Cpu::t_in ptr_in;
    Cpu::t_out ptr_out;
    Cpu::t_read_int ptr_read_int;

    void* data_read;
    void* data_write;
    void* data_in;
    void* data_out;
    void* data_read_int;

    unsigned (* tick)(struct s_Cpu* self);
    Cpu::t_opcode* optable;
    byte prefix;
    unsigned long tstate;

    int8_t tmp_int8;
    int16_t tmp_int16;
    int32_t tmp_int32;
    byte tmp_byte;
    byte tmp_byte_b;
    word tmp_word;
    word tmp_word_b;
    dword tmp_dword;
} s_Cpu;

#namespace Cpu
    extern bool ::is_tbl_initialized;
    extern byte ::tbl_parity[0x100];

    s_Cpu* ::new(
        ::t_read ptr_read,
        void* data_read,
        ::t_write ptr_write,
        void* data_write,
        ::t_in ptr_in,
        void* data_in,
        ::t_out ptr_out,
        void* data_out,
        ::t_read_int ptr_read_int,
        void* data_read_int
    );

    void ::init_tables(void);
    void ::free(s_Cpu* self);
    void ::reset(s_Cpu* self);
    unsigned ::tick_def(s_Cpu* self);
    unsigned ::tick_int(s_Cpu* self);
    unsigned ::do_int(s_Cpu* self);
    unsigned ::do_nmi(s_Cpu* self);

    bool ::is_int_possible(s_Cpu* self);
    bool ::is_nmi_possible(s_Cpu* self);

    word ::get_reg(s_Cpu* self, int reg);
    void ::set_reg(s_Cpu* self, int reg, word val);

    #define ::tick(cpu) (cpu->tick(cpu))
#end
