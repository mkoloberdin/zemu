// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

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

#include "cpu.h"
#include "op_common.h"
#include "op_pref_00.h"
#include "op_pref_CB.h"
#include "op_pref_DD.h"
#include "op_pref_DD_CB.h"
#include "op_pref_ED.h"
#include "op_pref_FD.h"
#include "op_pref_FD_CB.h"
#include <string.h>

#namespace Cpu
    bool ::is_tbl_initialized = false;
    byte ::tbl_parity[0x100];

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
    ) {
        ::init_tables();
        s_Cpu* cpu = ALLOC_MEM(s_Cpu);

        cpu->ptr_read = ptr_read;
        cpu->ptr_write = ptr_write;
        cpu->ptr_in = ptr_in;
        cpu->ptr_out = ptr_out;
        cpu->ptr_read_int = ptr_read_int;

        cpu->data_read = data_read;
        cpu->data_write = data_write;
        cpu->data_in = data_in;
        cpu->data_out = data_out;
        cpu->data_read_int = data_read_int;

        memset(cpu->regs, 0, CPU_LAST * sizeof(word));
        ::reset(cpu);

        return cpu;
    }

    void ::init_tables(void) {
        if (::is_tbl_initialized) {
            return;
        }

        for (int i = 0; i < 0x100; i++) {
            int p = ((i & 0x80) >> 7)
                ^ ((i & 0x40) >> 6)
                ^ ((i & 0x20) >> 5)
                ^ ((i & 0x10) >> 4)
                ^ ((i & 0x08) >> 3)
                ^ ((i & 0x04) >> 2)
                ^ ((i & 0x02) >> 1)
                ^ (i & 0x01);

            ::tbl_parity[i] = (p ? 0 : FLAG_PV);
        }

        ::is_tbl_initialized = true;
    }

    void ::free(s_Cpu* self) {
        if (self) {
            free(self);
        }
    }

    void ::reset(s_Cpu* self) {
        memset(self->regs, 0, sizeof(word) * CPU_LAST);

        REG_AF(self) = 0xFFFF;
        REG_SP(self) = 0xFFFF;
        // REG_PC(self) = 0x0000;
        // REG_MP(self) = 0x0000;
        // REG_IFF1(self) = 0;
        // REG_IFF2(self) = 0;
        // REG_I(self) = 0x00;
        // REG_R(self) = 0x00;
        // REG_IM(self) = 0;

        // actually all others registers may has any value
        REG_BC(self) = 0xFFFF;
        REG_DE(self) = 0xFFFF;
        REG_HL(self) = 0xFFFF;
        REG_IX(self) = 0xFFFF;
        REG_IY(self) = 0xFFFF;
        REG_AF_(self) = 0xFFFF;
        REG_BC_(self) = 0xFFFF;
        REG_DE_(self) = 0xFFFF;
        REG_HL_(self) = 0xFFFF;

        self->is_halted = false;
        self->is_opcode = false;
        self->is_noint = false;
        self->is_reset_pv = false;
        self->is_read_int = false;

        self->tick = ::tick_def;
        self->optable = optable_00;
        self->prefix = 0;
        self->tstate = 0;
    }

    unsigned ::tick_def(s_Cpu* self) {
        self->is_opcode = true;
        self->is_noint = false;
        self->is_reset_pv = false;
        self->tstate = 4;

        byte op = self->ptr_read(REG_PC(self)++, true, self->data_read);
        CPU_INC_R(self);

        self->optable[op](self);
        self->is_opcode = false;

        return self->tstate;
    }

    unsigned ::tick_int(s_Cpu* self) {
        self->is_opcode = true;
        self->is_noint = false;
        self->is_reset_pv = false;
        self->tstate = (4 + 2);

        byte op = self->ptr_read_int(self->data_read_int);
        CPU_INC_R(self);

        self->optable[op](self);
        self->is_opcode = false;

        if (self->prefix) {
            self->is_noint = true;
        } else {
            self->tick = ::tick_def;
            self->is_read_int = false;
        }

        return self->tstate;
    }

    unsigned ::do_int(s_Cpu* self) {
        if (!REG_IFF1(self) || self->is_noint || self->is_opcode || self->prefix) {
            return 0;
        }

        if (self->is_halted) {
            (REG_PC(self))++;
            self->is_halted = false;
        }

        REG_IFF1(self) = 0;
        REG_IFF2(self) = 0;

        if (self->is_reset_pv) {
            REG_F(self) &= ~FLAG_PV;
            self->is_reset_pv = false;
        }

        self->is_opcode = true;
        self->tstate = 0;

        switch (REG_IM(self)) {
            case 0: {
                self->is_read_int = true;
                ::tick_int(self); // tstate changed in this function

                if (self->prefix) {
                    self->tick = ::tick_int;
                }

                break;
            }

            case 1: {
                CPU_INC_R(self);
                self->tstate += (4 + 2);
                optable_00[0xFF](self); // RST #38
                break;
            }

            case 2: {
                CPU_INC_R(self);
                byte vec = self->ptr_read_int(self->data_read_int);
                word addr = ((word)REG_I(self) << 8) | vec;

                self->ptr_write(--(REG_SP(self)), REG_PCH(self), self->data_write);
                self->ptr_write(--(REG_SP(self)), REG_PCL(self), self->data_write);

                REG_PCL(self) = self->ptr_read(addr, false, self->data_read);
                REG_PCH(self) = self->ptr_read((word)(addr + 1), false, self->data_read);

                REG_MP(self) = REG_PC(self);
                self->tstate += 19;

                break;
            }
        }

        self->is_opcode = false;
        return self->tstate;
    }

    unsigned ::do_nmi(s_Cpu* self) {
        if (self->is_noint || self->is_opcode || self->prefix) {
            return 0;
        }

        if (self->is_halted) {
            (REG_PC(self))++;
            self->is_halted = false;
        }

        self->is_opcode = true;
        CPU_INC_R(self);

        // NOTICE: if you thinking about submitting bug-report about IFF1 not copied to IFF2, you are not right :)
        // actually IFF1 *not* copied to IFF2. this is tested on real hardware.
        REG_IFF1(self) = 0;

        self->ptr_write(--(REG_SP(self)), REG_PCH(self), self->data_write);
        self->ptr_write(--(REG_SP(self)), REG_PCL(self), self->data_write);

        REG_PC(self) = 0x0066;
        REG_MP(self) = REG_PC(self);
        self->is_opcode = false;

        return 11;
    }

    bool ::is_int_possible(s_Cpu* self) {
        return (REG_IFF1(self) && !self->is_noint && !self->is_opcode && !self->prefix);
    }

    bool ::is_nmi_possible(s_Cpu* self) {
        return (!self->is_noint && !self->is_opcode && !self->prefix);
    }

    word ::get_reg(s_Cpu* self, int reg) {
        return self->regs[reg];
    }

    void ::set_reg(s_Cpu* self, int reg, word val) {
        self->regs[reg] = val;
    }
#end
