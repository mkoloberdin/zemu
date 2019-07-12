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

#include <stdio.h>

extern Cpu::t_opcode optable_00[0x100];
extern Cpu::t_opcode optable_CB[0x100];
extern Cpu::t_opcode optable_DD[0x100];
extern Cpu::t_opcode optable_DD_CB[0x100];
extern Cpu::t_opcode optable_ED[0x100];
extern Cpu::t_opcode optable_FD[0x100];
extern Cpu::t_opcode optable_FD_CB[0x100];

//-V:OP_NOP:524
//-V:OP_LD_RP_NN:524
//-V:OP_LD_MRP_A:524
//-V:OP_DO_RP:524
//-V:OP_LD_R_N:524
//-V:OP_ADD_RP_RP:524
//-V:OP_LD_A_MRP:524
//-V:OP_DO_R:524
//-V:OP_DO_MHL:524
//-V:OP_DO_ORP:524
//-V:OP_LD_DO_R_PORP:524
//-V:OP_DO_PORP:524
//-V:OP_RLCA:524
//-V:OP_RRCA:524
//-V:OP_RLA:524
//-V:OP_RRA:524
//-V:OP_EX_AF_AF_:524
//-V:OP_DJNZ:524
//-V:OP_JR:524
//-V:OP_LD_R_R:524
//-V:OP_LD_R_MRP:524
//-V:OP_LD_R_ORP:524
//-V:OP_LD_MRP_R:524
//-V:OP_LD_ORP_R:524
//-V:OP_HALT:524
//-V:OP_JR_CC:524
//-V:OP_LD_MNN_RP:524
//-V:OP_DAA:524
//-V:OP_LD_RP_MNN:524
//-V:OP_CPL:524
//-V:OP_LD_MNN_A:524
//-V:OP_LD_MHL_N:524
//-V:OP_LD_ORP_N:524
//-V:OP_SCF:524
//-V:OP_LD_A_MNN:524
//-V:OP_CCF:524
//-V:OP_DO_A_R:524,547
//-V:OP_DO_A_MHL:524
//-V:OP_DO_A_ORP:524
//-V:OP_RET_CC:524
//-V:OP_POP_RP:524
//-V:OP_JP_CC:524
//-V:OP_JP:524
//-V:OP_CALL_CC:524
//-V:OP_PUSH_RP:524
//-V:OP_DO_A_N:524
//-V:OP_RST:524
//-V:OP_RET:524
//-V:OP_CALL:524
//-V:OP_OUT_N_A:524
//-V:OP_EXX:524
//-V:OP_IN_A_N:524
//-V:OP_EX_SP_RP:524
//-V:OP_JP_RP:524
//-V:OP_EX_DE_HL:524
//-V:OP_DI:524
//-V:OP_LD_SP_RP:524
//-V:OP_EI:524
//-V:OP_BIT_R:524
//-V:OP_BIT_MHL:524
//-V:OP_BIT_PORP:524
//-V:OP_RES_R:524
//-V:OP_RES_MHL:524
//-V:OP_LD_RES_PORP:524
//-V:OP_RES_PORP:524
//-V:OP_SET_R:524
//-V:OP_SET_MHL:524
//-V:OP_LD_SET_PORP:524
//-V:OP_SET_PORP:524
//-V:OP_IN_R_BC_P00:524
//-V:OP_OUT_BC_R_P00:524
//-V:OP_IN_F_BC_P00:524
//-V:OP_OUT_BC_0_P00:524
//-V:OP_SBC_HL_RP_P00:524
//-V:OP_NEG_P00:524
//-V:OP_RETN_P00:524
//-V:OP_RETI_P00:524
//-V:OP_IM_P00:524
//-V:OP_LD_I_A_P00:524
//-V:OP_ADC_HL_RP_P00:524
//-V:OP_LD_RR_A_P00:524
//-V:OP_LD_A_I_P00:524
//-V:OP_LD_A_RR_P00:524
//-V:OP_RRD_P00:524
//-V:OP_RLD_P00:524
//-V:OP_LDI_P00:524
//-V:OP_LDD_P00:524
//-V:OP_LDIR_P00:524
//-V:OP_LDDR_P00:524
//-V:OP_CPI_P00:524
//-V:OP_CPD_P00:524
//-V:OP_CPIR_P00:524
//-V:OP_CPDR_P00:524
//-V:OP_INI_P00:524
//-V:OP_IND_P00:524
//-V:OP_INIR_P00:524
//-V:OP_INDR_P00:524
//-V:OP_OUTI_P00:524
//-V:OP_OUTD_P00:524
//-V:OP_OTIR_P00:524
//-V:OP_OTDR_P00:524
//-V:OP_PREF_CB:524
//-V:OP_PREF_DD:524
//-V:OP_PREF_ED:524
//-V:OP_PREF_FD:524
//-V:OP_PREF_XX_CB:524

#define OP_NOP(FN, ...) \
    void FN(s_Cpu* self) { \
        __VA_ARGS__ \
    }

#define OP_LD_RP_NN(FN, RP, ...) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_WORD(self); \
        RP(self) = self->tmp_word; \
        self->tstate += (10 - 4); \
        __VA_ARGS__ \
    }

#define OP_LD_MRP_A(FN, RP, ...) \
    void FN(s_Cpu* self) { \
        REG_MPH(self) = REG_A(self); \
        REG_MPL(self) = (byte)(RP(self) + 1); \
        self->ptr_write(RP(self), REG_A(self), self->data_write); \
        self->tstate += (7 - 4); \
        __VA_ARGS__ \
    }

#define OP_DO_RP(FN, DO, RP, ...) \
    void FN(s_Cpu* self) { \
        DO(RP(self)); \
        self->tstate += (6 - 4); \
        __VA_ARGS__ \
    }

#define OP_LD_R_N(FN, R, ...) \
    void FN(s_Cpu* self) { \
        R(self) = CPU_READ_BYTE(self); \
        self->tstate += (7 - 4); \
        __VA_ARGS__ \
    }

#define OP_ADD_RP_RP(FN, RP1, RP2, ...) \
    void FN(s_Cpu* self) { \
        DO_ADD_16(RP1(self), RP2(self)); \
        self->tstate += (11 - 4); \
        __VA_ARGS__ \
    }

#define OP_LD_A_MRP(FN, RP, ...) \
    void FN(s_Cpu* self) { \
        REG_A(self) = self->ptr_read(RP(self), false, self->data_read); \
        REG_MP(self) = RP(self) + 1; \
        self->tstate += (7 - 4); \
        __VA_ARGS__ \
    }

#define OP_DO_R(FN, DO, R,...) \
    void FN(s_Cpu* self) { \
        DO(R(self)); \
        __VA_ARGS__ \
    }

#define OP_DO_MHL(FN, DO, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
        DO(self->tmp_byte); \
        self->ptr_write(REG_HL(self), self->tmp_byte, self->data_write); \
        self->tstate += (11 - 4); \
        __VA_ARGS__ \
    }

#define OP_DO_ORP(FN, DO, RP) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_OFFSET(self, RP); \
        self->tmp_byte = self->ptr_read(RP(self) + self->tmp_int8, false, self->data_read); \
        DO(self->tmp_byte); \
        self->ptr_write(RP(self) + self->tmp_int8, self->tmp_byte, self->data_write); \
        self->tstate += (19 - 4); \
        DO_PREF_00; \
    }

#define OP_LD_DO_R_PORP(FN, DO, R, RP) \
    void FN(s_Cpu* self) { \
        REG_MP(self) = RP(self) + self->tmp_int8; \
        self->tmp_byte = self->ptr_read(RP(self) + self->tmp_int8, false, self->data_read); \
        DO(self->tmp_byte); \
        R(self) = self->tmp_byte; \
        self->ptr_write(RP(self) + self->tmp_int8, self->tmp_byte, self->data_write); \
        self->tstate += (19 - 4); \
        DO_PREF_00; \
    }

#define OP_DO_PORP(FN, DO, RP) \
    void FN(s_Cpu* self) { \
        REG_MP(self) = RP(self) + self->tmp_int8; \
        self->tmp_byte = self->ptr_read(RP(self) + self->tmp_int8, false, self->data_read); \
        DO(self->tmp_byte); \
        self->ptr_write(RP(self) + self->tmp_int8, self->tmp_byte, self->data_write); \
        self->tstate += (19 - 4); \
        DO_PREF_00; \
    }

#define OP_RLCA(FN, ...) \
    void FN(s_Cpu* self) { \
        REG_A(self) = (REG_A(self) << 1) | (REG_A(self) >> 7); \
        REG_F(self) = (REG_F(self) & (FLAG_PV | FLAG_Z | FLAG_S)) | (REG_A(self) & (FLAG_C | FLAG_3 | FLAG_5)); \
        __VA_ARGS__ \
    }

#define OP_RRCA(FN, ...) \
    void FN(s_Cpu* self) { \
        REG_F(self) = (REG_F(self) & (FLAG_PV | FLAG_Z | FLAG_S)) | (REG_A(self) & FLAG_C); \
        REG_A(self) = (REG_A(self) >> 1) | (REG_A(self) << 7); \
        REG_F(self) |= (REG_A(self) & (FLAG_3 | FLAG_5)); \
        __VA_ARGS__ \
    }

#define OP_RLA(FN, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = REG_A(self); \
        REG_A(self) = (REG_A(self) << 1) | (REG_F(self) & FLAG_C); \
        REG_F(self) = (REG_F(self) & (FLAG_PV | FLAG_Z | FLAG_S)) | (REG_A(self) & (FLAG_3 | FLAG_5)) | (self->tmp_byte >> 7); \
        __VA_ARGS__ \
    }

#define OP_RRA(FN, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = REG_A(self); \
        REG_A(self) = (REG_A(self) >> 1) | (REG_F(self) << 7); \
        REG_F(self) = (REG_F(self) & (FLAG_PV | FLAG_Z | FLAG_S)) | (REG_A(self) & (FLAG_3 | FLAG_5)) | (self->tmp_byte & FLAG_C); \
        __VA_ARGS__ \
    }

#define OP_EX_AF_AF_(FN, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_word = REG_AF(self); \
        REG_AF(self) = REG_AF_(self); \
        REG_AF_(self) = self->tmp_word; \
        __VA_ARGS__ \
    }

#define OP_DJNZ(FN, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_int8 = (int8_t)CPU_READ_BYTE(self); \
        if (--REG_B(self)) { \
            REG_PC(self) += self->tmp_int8; \
            REG_MP(self) = REG_PC(self); \
            self->tstate += (13 - 4); \
        } else { \
            self->tstate += (8 - 4); \
        } \
        __VA_ARGS__ \
    }

#define OP_JR(FN, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_int8 = (int8_t)CPU_READ_BYTE(self); \
        REG_PC(self) += self->tmp_int8; \
        REG_MP(self) = REG_PC(self); \
        self->tstate += (12 - 4); \
        __VA_ARGS__ \
    }

#define OP_LD_R_R(FN, R1, R2, ...) \
    void FN(s_Cpu* self) { \
        R1(self) = R2(self); \
        __VA_ARGS__ \
    }

#define OP_LD_R_MRP(FN, R, RP) \
    void FN(s_Cpu* self) { \
        R(self) = self->ptr_read(RP(self), false, self->data_read); \
        self->tstate += (7 - 4); \
    }

#define OP_LD_R_ORP(FN, R, RP) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_OFFSET(self, RP); \
        R(self) = self->ptr_read(RP(self) + self->tmp_int8, false, self->data_read); \
        self->tstate += (15 - 4); \
        DO_PREF_00; \
    }

#define OP_LD_MRP_R(FN, RP, R, ...) \
    void FN(s_Cpu* self) { \
        self->ptr_write(RP(self), R(self), self->data_write); \
        self->tstate += (7 - 4); \
        __VA_ARGS__ \
    }

#define OP_LD_ORP_R(FN, RP, R) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_OFFSET(self, RP); \
        self->ptr_write(RP(self) + self->tmp_int8, R(self), self->data_write); \
        self->tstate += (15 - 4); \
        DO_PREF_00; \
    }

#define OP_HALT(FN, ...) \
    void FN(s_Cpu* self) { \
        self->is_halted = true; \
        REG_PC(self)--; \
        __VA_ARGS__ \
    }

#define OP_JR_CC(FN, CC, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_int8 = (int8_t)CPU_READ_BYTE(self); \
        if (CC) { \
            REG_PC(self) += self->tmp_int8; \
            REG_MP(self) = REG_PC(self); \
            self->tstate += (12 - 4); \
        } else {\
            self->tstate += (7 - 4); \
        } \
        __VA_ARGS__ \
    }

#define OP_LD_MNN_RP(FN, RL, RH, ...) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_WORD(self); \
        REG_MP(self) = self->tmp_word + 1; \
        self->ptr_write(self->tmp_word, RL(self), self->data_write); \
        self->ptr_write(self->tmp_word + 1, RH(self), self->data_write); \
        self->tstate += (16 - 4); \
        __VA_ARGS__ \
    }

#define OP_DAA(FN,...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = REG_A(self); \
        if (REG_F(self) & FLAG_N) { \
            if ((REG_F(self) & FLAG_H) || ((REG_A(self) & 0x0F) > 9)) { \
                self->tmp_byte -= 6; \
            } \
            if ((REG_F(self) & FLAG_C) || (REG_A(self) > 0x99)) { \
                self->tmp_byte -= 0x60; \
            } \
        } else { \
            if ((REG_F(self) & FLAG_H) || ((REG_A(self) & 0x0F) > 9)) { \
                self->tmp_byte += 6; \
            } \
            if ((REG_F(self) & FLAG_C) || (REG_A(self) > 0x99)) { \
                self->tmp_byte += 0x60; \
            } \
        } \
        REG_F(self) = (REG_F(self) & (FLAG_N | FLAG_C)) \
            | (self->tmp_byte & (FLAG_S | FLAG_5 | FLAG_3)) \
            | (self->tmp_byte ? 0 : FLAG_Z) \
            | Cpu::tbl_parity[self->tmp_byte] \
            | ((self->tmp_byte ^ REG_A(self)) & FLAG_H) \
            | (REG_A(self) > 0x99 ? FLAG_C : 0); \
        REG_A(self) = self->tmp_byte; \
        __VA_ARGS__ \
    }

#define OP_LD_RP_MNN(FN, RL, RH, ...) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_WORD(self); \
        RL(self) = self->ptr_read(self->tmp_word, false, self->data_read); \
        RH(self) = self->ptr_read(self->tmp_word + 1, false, self->data_read); \
        REG_MP(self) = self->tmp_word + 1; \
        self->tstate += (16 - 4); \
        __VA_ARGS__ \
    }

#define OP_CPL(FN, ...) \
    void FN(s_Cpu* self) { \
        REG_A(self) ^= 0xFF; \
        REG_F(self) = (REG_F(self) & (FLAG_C | FLAG_PV | FLAG_Z | FLAG_S)) | (REG_A(self) & (FLAG_3 | FLAG_5)) | (FLAG_N | FLAG_H); \
        __VA_ARGS__ \
    }

#define OP_LD_MNN_A(FN, ...) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_WORD(self); \
        REG_MPH(self) = REG_A(self); \
        REG_MPL(self) = (byte)(self->tmp_word + 1); \
        self->ptr_write(self->tmp_word, REG_A(self), self->data_write); \
        self->tstate += (13 - 4); \
        __VA_ARGS__ \
    }

#define OP_LD_MHL_N(FN, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = CPU_READ_BYTE(self); \
        self->ptr_write(REG_HL(self), self->tmp_byte, self->data_write); \
        self->tstate += (10 - 4); \
        __VA_ARGS__ \
    }

#define OP_LD_ORP_N(FN, RP) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_OFFSET(self, RP); \
        self->tmp_byte = CPU_READ_BYTE(self); \
        self->ptr_write(RP(self) + self->tmp_int8, self->tmp_byte, self->data_write); \
        self->tstate += (15 - 4); \
        DO_PREF_00; \
    }

#define OP_SCF(FN, ...) \
    void FN(s_Cpu* self) { \
        REG_F(self) = (REG_F(self) & (FLAG_PV | FLAG_Z | FLAG_S)) | (REG_A(self) & (FLAG_5 | FLAG_3)) | FLAG_C; \
        __VA_ARGS__ \
    }

#define OP_LD_A_MNN(FN, ...) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_WORD(self); \
        REG_A(self) = self->ptr_read(self->tmp_word, false, self->data_read); \
        REG_MP(self) = self->tmp_word + 1; \
        self->tstate += (13 - 4); \
        __VA_ARGS__ \
    }

#define OP_CCF(FN, ...) \
    void FN(s_Cpu* self) { \
        REG_F(self) = (REG_F(self) & (FLAG_PV | FLAG_Z | FLAG_S)) \
            | ((REG_F(self) & FLAG_C) << FLAG_C_TO_H) \
            | ((REG_F(self) & FLAG_C) ^ FLAG_C) \
            | (REG_A(self) & (FLAG_3 | FLAG_5)); \
        __VA_ARGS__ \
    }

#define OP_DO_A_R(FN, DO, R, ...) \
    void FN(s_Cpu* self) { \
        DO(REG_A(self), R(self)); \
        __VA_ARGS__ \
    }

#define OP_DO_A_MHL(FN, DO, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
        DO(REG_A(self), self->tmp_byte); \
        self->tstate += (7 - 4); \
        __VA_ARGS__ \
    }

#define OP_DO_A_ORP(FN, DO, RP) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_OFFSET(self, RP); \
        self->tmp_byte = self->ptr_read(RP(self) + self->tmp_int8, false, self->data_read); \
        DO(REG_A(self), self->tmp_byte); \
        self->tstate += (15 - 4); \
        DO_PREF_00; \
    }

#define OP_RET_CC(FN, CC, ...) \
    void FN(s_Cpu* self) { \
        if (CC) { \
            DO_RET; \
            self->tstate += (11 - 4); \
        } else { \
            self->tstate += (5 - 4); \
        } \
        __VA_ARGS__ \
    }

#define OP_POP_RP(FN, RP, ...) \
    void FN(s_Cpu* self) { \
        DO_POP_TMP; \
        RP(self) = self->tmp_word; \
        self->tstate += (10 - 4); \
        __VA_ARGS__ \
    }

#define OP_JP_CC(FN, CC, ...) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_WORD(self); \
        REG_MP(self) = self->tmp_word; \
        if (CC) { \
            REG_PC(self) = self->tmp_word; \
        } \
        self->tstate += (10 - 4); \
        __VA_ARGS__ \
    }

#define OP_JP(FN, ...) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_WORD(self); \
        REG_PC(self) = self->tmp_word; \
        REG_MP(self) = self->tmp_word; \
        self->tstate += (10 - 4); \
        __VA_ARGS__ \
    }

#define OP_CALL_CC(FN, CC, ...) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_WORD(self); \
        REG_MP(self) = self->tmp_word; \
        if (CC) { \
            DO_PUSH(REG_PC(self)); \
            REG_PC(self) = self->tmp_word; \
            self->tstate += (17 - 4); \
        } else { \
            self->tstate += (10 - 4); \
        } \
        __VA_ARGS__ \
    }

#define OP_PUSH_RP(FN, RP, ...) \
    void FN(s_Cpu* self) { \
        DO_PUSH(RP(self)); \
        self->tstate += (11 - 4); \
        __VA_ARGS__ \
    }

#define OP_DO_A_N(FN, DO, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = CPU_READ_BYTE(self); \
        DO(REG_A(self), self->tmp_byte); \
        self->tstate += (7 - 4); \
        __VA_ARGS__ \
    }

#define OP_RST(FN, ADDR, ...) \
    void FN(s_Cpu* self) { \
        DO_PUSH(REG_PC(self)); \
        REG_PC(self) = (ADDR); \
        REG_MP(self) = (ADDR); \
        self->tstate += (11 - 4); \
        __VA_ARGS__ \
    }

#define OP_RET(FN, ...) \
    void FN(s_Cpu* self) { \
        DO_RET; \
        self->tstate += (10 - 4); \
        __VA_ARGS__ \
    }

#define OP_CALL(FN, ...) \
    void FN(s_Cpu* self) { \
        CPU_DO_READ_WORD(self); \
        DO_PUSH(REG_PC(self)); \
        REG_PC(self) = self->tmp_word; \
        REG_MP(self) = self->tmp_word; \
        self->tstate += (17 - 4); \
        __VA_ARGS__ \
    }

#define OP_OUT_N_A(FN, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_word = (REG_A(self) << 8) | CPU_READ_BYTE(self); \
        self->tstate += (8 - 4); \
        DO_OUT(self->tmp_word, REG_A(self)); \
        self->tstate += (11 - 8); \
        __VA_ARGS__ \
    }

#define OP_EXX(FN,...) \
    void FN(s_Cpu* self) { \
        self->tmp_word = REG_BC(self); REG_BC(self) = REG_BC_(self); REG_BC_(self) = self->tmp_word; \
        self->tmp_word = REG_DE(self); REG_DE(self) = REG_DE_(self); REG_DE_(self) = self->tmp_word; \
        self->tmp_word = REG_HL(self); REG_HL(self) = REG_HL_(self); REG_HL_(self) = self->tmp_word; \
        __VA_ARGS__ \
    }

#define OP_IN_A_N(FN, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_word = (REG_A(self) << 8) | CPU_READ_BYTE(self); \
        DO_IN(REG_A(self), self->tmp_word); \
        self->tstate += (11 - 4); \
        __VA_ARGS__ \
    }

#define OP_EX_SP_RP(FN, RL, RH, RP, ...) \
    void FN(s_Cpu* self) { \
        CPU_TMPL(self) = self->ptr_read(REG_SP(self), false, self->data_read); \
        CPU_TMPH(self) = self->ptr_read(REG_SP(self) + 1, false, self->data_read); \
        REG_MP(self) = self->tmp_word; \
        self->ptr_write(REG_SP(self), RL(self), self->data_write); \
        self->ptr_write(REG_SP(self) + 1, RH(self), self->data_write); \
        RP(self) = self->tmp_word; \
        self->tstate += (19 - 4); \
        __VA_ARGS__ \
    }

#define OP_JP_RP(FN, RP, ...) \
    void FN(s_Cpu* self) { \
        REG_PC(self) = RP(self); \
        __VA_ARGS__ \
    }

#define OP_EX_DE_HL(FN, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_word = REG_DE(self); \
        REG_DE(self) = REG_HL(self); \
        REG_HL(self) = self->tmp_word; \
        __VA_ARGS__ \
    }

#define OP_DI(FN, ...) \
    void FN(s_Cpu* self) { \
        REG_IFF1(self) = 0; \
        REG_IFF2(self) = 0; \
        __VA_ARGS__ \
    }

#define OP_LD_SP_RP(FN, RP, ...) \
    void FN(s_Cpu* self) { \
        REG_SP(self) = RP(self); \
        self->tstate += (6 - 4); \
        __VA_ARGS__ \
    }

#define OP_EI(FN, ...) \
    void FN(s_Cpu* self) { \
        REG_IFF1(self) = 1; \
        REG_IFF2(self) = 1; \
        self->is_noint = true; \
        __VA_ARGS__ \
    }

#define OP_BIT_R(FN, BIT, R, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = R(self) & (0x01 << (BIT)); \
        REG_F(self) = (REG_F(self) & FLAG_C) \
            | FLAG_H \
            | (self->tmp_byte & (FLAG_S | FLAG_5 | FLAG_3)) \
            | (self->tmp_byte ? 0 : FLAG_Z) \
            | Cpu::tbl_parity[self->tmp_byte] \
            | (R(self) & (FLAG_5 | FLAG_3)); \
        __VA_ARGS__ \
    }

#define OP_BIT_MHL(FN, BIT, ...) \
    void FN(s_Cpu* self) { \
        DO_BIT_M(REG_HL(self), BIT); \
        self->tstate += (8 - 4); \
        __VA_ARGS__ \
    }

#define OP_BIT_PORP(FN, BIT, RP) \
    void FN(s_Cpu* self) { \
        REG_MP(self) = RP(self) + self->tmp_int8; \
        DO_BIT_M((RP(self) + self->tmp_int8), BIT); \
        self->tstate += (16 - 4); \
        DO_PREF_00; \
    }

#define OP_RES_R(FN, BIT, R, ...) \
    void FN(s_Cpu* self) { \
        R(self) &= ~(0x01 << (BIT)); \
        __VA_ARGS__ \
    }

#define OP_RES_MHL(FN, BIT, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
        self->tmp_byte &= ~(0x01 << (BIT)); \
        self->ptr_write(REG_HL(self), self->tmp_byte, self->data_write); \
        self->tstate += (11 - 4); \
        __VA_ARGS__ \
    }

#define OP_LD_RES_PORP(FN, BIT, R, RP) \
    void FN(s_Cpu* self) { \
        REG_MP(self) = RP(self) + self->tmp_int8; \
        self->tmp_byte = self->ptr_read(RP(self) + self->tmp_int8, false, self->data_read); \
        self->tmp_byte &= ~(0x01 << (BIT)); \
        R(self) = self->tmp_byte; \
        self->ptr_write(RP(self) + self->tmp_int8, self->tmp_byte, self->data_write); \
        self->tstate += (19 - 4); \
        DO_PREF_00; \
    }

#define OP_RES_PORP(FN, BIT, RP) \
    void FN(s_Cpu* self) { \
        REG_MP(self) = RP(self) + self->tmp_int8; \
        self->tmp_byte = self->ptr_read(RP(self) + self->tmp_int8, false, self->data_read); \
        self->tmp_byte &= ~(0x01 << (BIT)); \
        self->ptr_write(RP(self) + self->tmp_int8, self->tmp_byte, self->data_write); \
        self->tstate += (19 - 4); \
        DO_PREF_00; \
    }

#define OP_SET_R(FN, BIT, R, ...) \
    void FN(s_Cpu* self) { \
        R(self) |= (0x01 << (BIT)); \
        __VA_ARGS__ \
    }

#define OP_SET_MHL(FN, BIT, ...) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
        self->tmp_byte |= (0x01 << (BIT)); \
        self->ptr_write(REG_HL(self), self->tmp_byte, self->data_write); \
        self->tstate += (11 - 4); \
        __VA_ARGS__ \
    }

#define OP_LD_SET_PORP(FN, BIT, R, RP) \
    void FN(s_Cpu* self) { \
        REG_MP(self) = RP(self) + self->tmp_int8; \
        self->tmp_byte = self->ptr_read(RP(self) + self->tmp_int8, false, self->data_read); \
        self->tmp_byte |= (0x01 << (BIT)); \
        R(self) = self->tmp_byte; \
        self->ptr_write(RP(self) + self->tmp_int8, self->tmp_byte, self->data_write); \
        self->tstate += (19 - 4); \
        DO_PREF_00; \
    }

#define OP_SET_PORP(FN, BIT, RP) \
    void FN(s_Cpu* self) { \
        REG_MP(self) = RP(self) + self->tmp_int8; \
        self->tmp_byte = self->ptr_read(RP(self) + self->tmp_int8, false, self->data_read); \
        self->tmp_byte |= (0x01 << (BIT)); \
        self->ptr_write(RP(self) + self->tmp_int8, self->tmp_byte, self->data_write); \
        self->tstate += (19 - 4); \
        DO_PREF_00; \
    }

#define OP_IN_R_BC_P00(FN, R) \
    void FN(s_Cpu* self) { \
        R(self) = self->ptr_in(REG_BC(self), self->data_in); \
        REG_MP(self) = (word)(REG_BC(self) + 1); \
        REG_F(self) = (REG_F(self) & FLAG_C) \
            | (R(self) & (FLAG_S | FLAG_5 | FLAG_3)) \
            | (R(self) ? 0 : FLAG_Z) \
            | Cpu::tbl_parity[R(self)]; \
        self->tstate += (8 - 4); \
        DO_PREF_00; \
    }

#define OP_OUT_BC_R_P00(FN, R) \
    void FN(s_Cpu* self) { \
        self->tstate += (5 - 4); \
        self->ptr_out(REG_BC(self), R(self), self->data_out); \
        REG_MP(self) = (word)(REG_BC(self) + 1); \
        self->tstate += (8 - 5); \
        DO_PREF_00; \
    }

#define OP_IN_F_BC_P00(FN) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = self->ptr_in(REG_BC(self), self->data_in); \
        REG_MP(self) = (word)(REG_BC(self) + 1); \
        REG_F(self) = (REG_F(self) & FLAG_C) \
            | (self->tmp_byte & (FLAG_S | FLAG_5 | FLAG_3)) \
            | (self->tmp_byte ? 0 : FLAG_Z) \
            | Cpu::tbl_parity[self->tmp_byte]; \
        self->tstate += (8 - 4); \
        DO_PREF_00; \
    }

#define OP_OUT_BC_0_P00(FN) \
    void FN(s_Cpu* self) { \
        self->tstate += (5 - 4); \
        self->ptr_out(REG_BC(self), 0, self->data_out); \
        REG_MP(self) = (word)(REG_BC(self) + 1); \
        self->tstate += (8 - 5); \
        DO_PREF_00; \
    }

#define OP_SBC_HL_RP_P00(FN, RP) \
    void FN(s_Cpu* self) { \
        DO_SBC_16(REG_HL(self), RP(self)); \
        self->tstate += (11 - 4); \
        DO_PREF_00; \
    }

#define OP_NEG_P00(FN) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = REG_A(self); \
        REG_A(self) = 0; \
        DO_SUB_8(REG_A(self), self->tmp_byte); \
        DO_PREF_00; \
    }

#define OP_RETN_P00(FN) \
    void FN(s_Cpu* self) { \
        REG_IFF1(self) = REG_IFF2(self); \
        DO_RET; \
        self->tstate += (10 - 4); \
        DO_PREF_00; \
    }

#define OP_RETI_P00 OP_RETN_P00

#define OP_IM_P00(FN, MODE) \
    void FN(s_Cpu* self) { \
        REG_IM(self) = (MODE); \
        DO_PREF_00; \
    }

#define OP_LD_I_A_P00(FN) \
    void FN(s_Cpu* self) { \
        REG_I(self) = REG_A(self); \
        self->tstate += (5 - 4); \
        DO_PREF_00; \
    }

#define OP_ADC_HL_RP_P00(FN, RP) \
    void FN(s_Cpu* self) { \
        DO_ADC_16(REG_HL(self), RP(self)); \
        self->tstate += (11 - 4); \
        DO_PREF_00; \
    }

#define OP_LD_RR_A_P00(FN) \
    void FN(s_Cpu* self) { \
        REG_R(self) = REG_A(self); \
        self->tstate += (5 - 4); \
        DO_PREF_00; \
    }

#define OP_LD_A_I_P00(FN) \
    void FN(s_Cpu* self) { \
        REG_A(self) = REG_I(self); \
        REG_F(self) = (REG_F(self) & FLAG_C) \
            | (REG_A(self) & (FLAG_S | FLAG_5 | FLAG_3)) \
            | (REG_A(self) ? 0 : FLAG_Z) \
            | (REG_IFF2(self) << FLAG_PV_IFF_S8); \
        self->is_reset_pv = true; \
        self->tstate += (5 - 4); \
        DO_PREF_00; \
    }

#define OP_LD_A_RR_P00(FN) \
    void FN(s_Cpu* self) { \
        REG_A(self) = REG_R(self); \
        REG_F(self) = (REG_F(self) & FLAG_C) \
            | (REG_A(self) & (FLAG_S | FLAG_5 | FLAG_3)) \
            | (REG_A(self) ? 0 : FLAG_Z) \
            | (REG_IFF2(self) << FLAG_PV_IFF_S8); \
        self->is_reset_pv = true; \
        self->tstate += (5 - 4); \
        DO_PREF_00; \
    }

#define OP_RRD_P00(FN) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
        self->ptr_write(REG_HL(self), ((REG_A(self) << 4) | (self->tmp_byte >> 4)), self->data_write); \
        REG_A(self) = (REG_A(self) & 0xF0) | (self->tmp_byte & 0x0F); \
        REG_F(self) = (REG_F(self) & FLAG_C) \
            | (REG_A(self) & (FLAG_S | FLAG_5 | FLAG_3)) \
            | (REG_A(self) ? 0 : FLAG_Z) \
            | Cpu::tbl_parity[REG_A(self)]; \
        REG_MP(self) = (word)(REG_HL(self) + 1); \
        self->tstate += (14 - 4); \
        DO_PREF_00; \
    }

#define OP_RLD_P00(FN) \
    void FN(s_Cpu* self) { \
        self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
        self->ptr_write(REG_HL(self), ((self->tmp_byte << 4) | (REG_A(self) & 0x0F)), self->data_write); \
        REG_A(self) = (REG_A(self) & 0xF0) | (self->tmp_byte >> 4); \
        REG_F(self) = (REG_F(self) & FLAG_C) \
            | (REG_A(self) & (FLAG_S | FLAG_5 | FLAG_3)) \
            | (REG_A(self) ? 0 : FLAG_Z) \
            | Cpu::tbl_parity[REG_A(self)]; \
        REG_MP(self) = (word)(REG_HL(self) + 1); \
        self->tstate += (14 - 4); \
        DO_PREF_00; \
    }

#define OP_LDI_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_LD; \
        DO_REP_LD_INC; \
        DO_PREF_00; \
    }

#define OP_LDD_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_LD; \
        DO_REP_LD_DEC; \
        DO_PREF_00; \
    }

#define OP_LDIR_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_LD; \
        DO_REP_LD_INCR; \
        DO_PREF_00; \
    }

#define OP_LDDR_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_LD; \
        DO_REP_LD_DECR; \
        DO_PREF_00; \
    }

#define OP_CPI_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_CP; \
        DO_REP_CP_INC; \
        DO_PREF_00; \
    }

#define OP_CPD_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_CP; \
        DO_REP_CP_DEC; \
        DO_PREF_00; \
    }

#define OP_CPIR_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_CP; \
        DO_REP_CP_INCR; \
        DO_PREF_00; \
    }

#define OP_CPDR_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_CP; \
        DO_REP_CP_DECR; \
        DO_PREF_00; \
    }

#define OP_INI_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_INI; \
        self->tstate += (12 - 9); \
        DO_PREF_00; \
    }

#define OP_IND_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_IND; \
        self->tstate += (12 - 9); \
        DO_PREF_00; \
    }

#define OP_INIR_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_INI; \
        DO_REP_IN_OUT_REP; \
        DO_PREF_00; \
    }

#define OP_INDR_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_IND; \
        DO_REP_IN_OUT_REP; \
        DO_PREF_00; \
    }

#define OP_OUTI_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_OUTI; \
        self->tstate += (12 - 9); \
        DO_PREF_00; \
    }

#define OP_OUTD_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_OUTD; \
        self->tstate += (12 - 9); \
        DO_PREF_00; \
    }

#define OP_OTIR_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_OUTI; \
        DO_REP_IN_OUT_REP; \
        DO_PREF_00; \
    }

#define OP_OTDR_P00(FN) \
    void FN(s_Cpu* self) { \
        DO_REP_OUTD; \
        DO_REP_IN_OUT_REP; \
        DO_PREF_00; \
    }

#define OP_PREF_CB(FN) \
    void FN(s_Cpu* self) { \
        self->prefix = 0xCB; \
        self->optable = optable_CB; \
        self->is_noint = true; \
    }

#define OP_PREF_DD(FN) \
    void FN(s_Cpu* self) { \
        self->prefix = 0xDD; \
        self->optable = optable_DD; \
        self->is_noint = true; \
    }

#define OP_PREF_ED(FN) \
    void FN(s_Cpu* self) { \
        self->prefix = 0xED; \
        self->optable = optable_ED; \
        self->is_noint = true; \
    }

#define OP_PREF_FD(FN) \
    void FN(s_Cpu* self) { \
        self->prefix = 0xFD; \
        self->optable = optable_FD; \
        self->is_noint = true; \
    }

#define OP_PREF_XX_CB(FN, OPTBL) \
    void FN(s_Cpu* self) { \
        self->tmp_int8 = CPU_READ_OFFSET(self); \
        self->tmp_byte = CPU_READ_BYTE(self); \
        OPTBL[self->tmp_byte](self); \
    }

#define CC_NZ (!(REG_F(self) & FLAG_Z))
#define CC_Z  (REG_F(self) & FLAG_Z)
#define CC_NC (!(REG_F(self) & FLAG_C))
#define CC_C  (REG_F(self) & FLAG_C)
#define CC_PO (!(REG_F(self) & FLAG_PV)) // parity odd
#define CC_PE (REG_F(self) & FLAG_PV)    // parity even
#define CC_P  (!(REG_F(self) & FLAG_S))  // positive
#define CC_M  (REG_F(self) & FLAG_S)     // minus

#define DO_PREF_00 \
    self->prefix = 0x00; \
    self->optable = optable_00;

#define DO_INC_16(X) \
    (X)++;

#define DO_DEC_16(X) \
    (X)--;

#define DO_ADD_16(X, Y) \
    self->tmp_dword = (X) + (Y); \
    self->tmp_word_b = ((X) & 0x0FFF) + ((Y) & 0x0FFF); \
    REG_MP(self) = (word)((X) + 1); \
    (X) = (word)self->tmp_dword; \
    REG_F(self) = (REG_F(self) & (FLAG_PV | FLAG_Z | FLAG_S)) \
        | ((self->tmp_dword & FLAG_C_M16) >> FLAG_C_S16) \
        | ((self->tmp_dword >> 8) & (FLAG_5 | FLAG_3)) \
        | ((self->tmp_word_b & FLAG_H_M16) >> FLAG_H_S16);

#define DO_ADC_16(X, Y) \
    self->tmp_dword = (X) + (Y) + (REG_F(self) & FLAG_C); \
    self->tmp_word_b = ((X) & 0x0FFF) + ((Y) & 0x0FFF) + (REG_F(self) & FLAG_C); \
    self->tmp_int32 = (int32_t)((int16_t)(X)) + (int32_t)((int16_t)(Y)) + (int32_t)(REG_F(self) & FLAG_C); \
    REG_MP(self) = (word)((X) + 1); \
    (X) = (word)self->tmp_dword; \
    REG_F(self) = ((self->tmp_dword & FLAG_C_M16) >> FLAG_C_S16) \
        | ((self->tmp_word_b & FLAG_H_M16) >> FLAG_H_S16) \
        | (((self->tmp_int32 < -32768) || (self->tmp_int32 > 32767)) ? FLAG_PV : 0) \
        | ((self->tmp_dword >> 8) & (FLAG_S | FLAG_5 | FLAG_3)) \
        | ((word)self->tmp_dword ? 0 : FLAG_Z);

#define DO_SBC_16(X, Y) \
    self->tmp_dword = (X) - (Y) - (REG_F(self) & FLAG_C); \
    self->tmp_word_b = ((X) & 0x0FFF) - ((Y) & 0x0FFF) - (REG_F(self) & FLAG_C); \
    self->tmp_int32 = (int32_t)((int16_t)(X)) - (int32_t)((int16_t)(Y)) - (int32_t)(REG_F(self) & FLAG_C); \
    REG_F(self) = ((self->tmp_dword & FLAG_C_M16) >> FLAG_C_S16) \
        | FLAG_N \
        | ((self->tmp_word_b & FLAG_H_M16) >> FLAG_H_S16) \
        | (((self->tmp_int32 < -32768) || (self->tmp_int32 > 32767)) ? FLAG_PV : 0) \
        | ((self->tmp_dword >> 8) & (FLAG_S | FLAG_5 | FLAG_3)) \
        | ((word)self->tmp_dword ? 0 : FLAG_Z); \
    REG_MP(self) = (word)((X) + 1); \
    (X) = (word)self->tmp_dword;

#define DO_INC_8(X) \
    REG_F(self) = (REG_F(self) & FLAG_C) | ((((X) & 0x0F) + 1) & FLAG_H); \
    (X)++; \
    REG_F(self) |= ((X) == FLAG_PV_C8 ? FLAG_PV : 0) | ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z);

#define DO_DEC_8(X) \
    REG_F(self) = (REG_F(self) & FLAG_C) | FLAG_N | ((((X) & 0x0F) - 1) & FLAG_H) | ((X) == FLAG_PV_C8 ? FLAG_PV : 0); \
    (X)--; \
    REG_F(self) |= ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z);

#define DO_ADD_8(X, Y) \
    self->tmp_word = (X) + (Y); \
    self->tmp_byte_b = ((X) & 0x0F) + ((Y) & 0x0F); \
    self->tmp_int16 = (int16_t)((int8_t)(X)) + (int16_t)((int8_t)(Y)); \
    REG_F(self) = ((self->tmp_word & FLAG_C_M8) >> FLAG_C_S8) \
        | (self->tmp_byte_b & FLAG_H) \
        | (((self->tmp_int16 < -128) || (self->tmp_int16 > 127)) ? FLAG_PV : 0) \
        | (self->tmp_word & (FLAG_S | FLAG_5 | FLAG_3)) \
        | ((byte)self->tmp_word ? 0 : FLAG_Z); \
    (X) = (byte)self->tmp_word;

#define DO_ADC_8(X, Y) \
    self->tmp_word = (X) + (Y) + (REG_F(self) & FLAG_C); \
    self->tmp_byte_b = ((X) & 0x0F) + ((Y) & 0x0F) + (REG_F(self) & FLAG_C); \
    self->tmp_int16 = (int16_t)((int8_t)(X)) + (int16_t)((int8_t)(Y)) + (int16_t)(REG_F(self) & FLAG_C); \
    REG_F(self) = ((self->tmp_word & FLAG_C_M8) >> FLAG_C_S8) \
        | (self->tmp_byte_b & FLAG_H) \
        | (((self->tmp_int16 < -128) || (self->tmp_int16 > 127)) ? FLAG_PV : 0) \
        | (self->tmp_word & (FLAG_S | FLAG_5 | FLAG_3)) \
        | ((byte)self->tmp_word ? 0 : FLAG_Z); \
    (X) = (byte)self->tmp_word;

#define DO_SUB_8(X, Y) \
    self->tmp_word = (X) - (Y); \
    self->tmp_byte_b = ((X) & 0x0F) - ((Y) & 0x0F); \
    self->tmp_int16 = (int16_t)((int8_t)(X)) - (int16_t)((int8_t)(Y)); \
    REG_F(self) = ((self->tmp_word & FLAG_C_M8) >> FLAG_C_S8) \
        | FLAG_N \
        | (self->tmp_byte_b & FLAG_H) \
        | (((self->tmp_int16 < -128) || (self->tmp_int16 > 127)) ? FLAG_PV : 0) \
        | (self->tmp_word & (FLAG_S | FLAG_5 | FLAG_3)) \
        | ((byte)self->tmp_word ? 0 : FLAG_Z); \
    (X) = (byte)self->tmp_word;

#define DO_SBC_8(X, Y) \
    self->tmp_word = (X) - (Y) - (REG_F(self) & FLAG_C); \
    self->tmp_byte_b = ((X) & 0x0F) - ((Y) & 0x0F) - (REG_F(self) & FLAG_C); \
    self->tmp_int16 = (int16_t)((int8_t)(X)) - (int16_t)((int8_t)(Y)) - (int16_t)(REG_F(self) & FLAG_C); \
    REG_F(self) = ((self->tmp_word & FLAG_C_M8) >> FLAG_C_S8) \
        | FLAG_N \
        | (self->tmp_byte_b & FLAG_H) \
        | (((self->tmp_int16 < -128) || (self->tmp_int16 > 127)) ? FLAG_PV : 0) \
        | (self->tmp_word & (FLAG_S | FLAG_5 | FLAG_3)) \
        | ((byte)self->tmp_word ? 0 : FLAG_Z); \
    (X) = (byte)self->tmp_word;

#define DO_AND_8(X, Y) \
    (X) &= (Y); \
    REG_F(self) = FLAG_H | ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_XOR_8(X, Y) \
    (X) ^= (Y); \
    REG_F(self) = ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_OR_8(X, Y) \
    (X) |= (Y); \
    REG_F(self) = ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_CP_8(X, Y) \
    self->tmp_word = (X) - (Y); \
    self->tmp_byte_b = ((X) & 0x0F) - ((Y) & 0x0F); \
    self->tmp_int16 = (int16_t)((int8_t)(X)) - (int16_t)((int8_t)(Y)); \
    REG_F(self) = ((self->tmp_word & FLAG_C_M8) >> FLAG_C_S8) \
        | FLAG_N \
        | (self->tmp_byte_b & FLAG_H) \
        | (((self->tmp_int16 < -128) || (self->tmp_int16 > 127)) ? FLAG_PV : 0) \
        | ((Y) & (FLAG_5 | FLAG_3)) \
        | (self->tmp_word & FLAG_S) \
        | ((byte)self->tmp_word ? 0 : FLAG_Z);

#define DO_POP_TMP \
    CPU_TMPL(self) = self->ptr_read((REG_SP(self))++, false, self->data_read); \
    CPU_TMPH(self) = self->ptr_read((REG_SP(self))++, false, self->data_read);

#define DO_RET \
    DO_POP_TMP; \
    REG_PC(self) = self->tmp_word; \
    REG_MP(self) = self->tmp_word;

#define DO_PUSH(VAL) \
    self->tmp_word_b = VAL; \
    self->ptr_write(--(REG_SP(self)), CPU_TMPH_B(self), self->data_write); \
    self->ptr_write(--(REG_SP(self)), CPU_TMPL_B(self), self->data_write);

#define DO_OUT(PORT, VAL) \
    self->ptr_out((PORT), (VAL), self->data_out); \
    REG_MPL(self) = (byte)((PORT) + 1); \
    REG_MPH(self) = REG_A(self);

#define DO_IN(VAL, PORT) \
    (VAL) = self->ptr_in((PORT), self->data_in); \
    REG_MP(self) = (word)((PORT) + 1);

#define DO_RLC_8(X) \
    (X) = ((X) << 1) | ((X) >> 7); \
    REG_F(self) = ((X) & (FLAG_S | FLAG_5 | FLAG_3 | FLAG_C)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_RRC_8(X) \
    REG_F(self) = ((X) & FLAG_C); \
    (X) = ((X) >> 1) | ((X) << 7); \
    REG_F(self) |= ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_RL_8(X) \
    self->tmp_byte_b = (X); \
    (X) = ((X) << 1) | (REG_F(self) & FLAG_C); \
    REG_F(self) = (self->tmp_byte_b >> 7) | ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_RR_8(X) \
    self->tmp_byte_b = (X); \
    (X) = ((X) >> 1) | (REG_F(self) << 7); \
    REG_F(self) = (self->tmp_byte_b & FLAG_C) | ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_SLA_8(X) \
    REG_F(self) = (X) >> 7; \
    (X) <<= 1; \
    REG_F(self) |= ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_SRA_8(X) \
    REG_F(self) = (X) & FLAG_C; \
    (X) = ((X) & 0x80) | ((X) >> 1); \
    REG_F(self) |= ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_SLL_8(X) \
    REG_F(self) = (X) >> 7; \
    (X) = ((X) << 1) | 0x01; \
    REG_F(self) |= ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_SRL_8(X) \
    REG_F(self) = (X) & FLAG_C; \
    (X) >>= 1; \
    REG_F(self) |= ((X) & (FLAG_S | FLAG_5 | FLAG_3)) | ((X) ? 0 : FLAG_Z) | Cpu::tbl_parity[(X)];

#define DO_BIT_M(X, BIT) \
    self->tmp_byte = self->ptr_read((X), false, self->data_read); \
    self->tmp_byte_b = self->tmp_byte & (0x01 << (BIT)); \
    REG_F(self) = (REG_F(self) & FLAG_C) \
        | FLAG_H \
        | (self->tmp_byte_b & FLAG_S) \
        | (self->tmp_byte_b ? 0 : FLAG_Z) \
        | Cpu::tbl_parity[self->tmp_byte_b] \
        | (REG_MPH(self) & (FLAG_5 | FLAG_3));

#define DO_REP_LD \
    self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
    self->ptr_write(REG_DE(self), self->tmp_byte, self->data_write); \
    REG_BC(self)--; \
    self->tmp_byte += REG_A(self); \
    REG_F(self) = (REG_F(self) & (FLAG_S | FLAG_Z | FLAG_C)) \
        | (REG_BC(self) ? FLAG_PV : 0) \
        | (self->tmp_byte & FLAG_3) \
        | ((self->tmp_byte << FLAG_N_TO_5) & FLAG_5);

#define DO_REP_LD_INC \
    REG_HL(self)++; \
    REG_DE(self)++; \
    self->tstate += (12 - 4);

#define DO_REP_LD_DEC \
    REG_HL(self)--; \
    REG_DE(self)--; \
    self->tstate += (12 - 4);

#define DO_REP_LD_INCR \
    REG_HL(self)++; \
    REG_DE(self)++; \
    if (REG_BC(self)) { \
        REG_PC(self) -= 2; \
        REG_MP(self) = (word)(REG_PC(self) + 1); \
        self->tstate += (17 - 4); \
    } else { \
        self->tstate += (12 - 4); \
    }

#define DO_REP_LD_DECR \
    REG_HL(self)--; \
    REG_DE(self)--; \
    if (REG_BC(self)) { \
        REG_PC(self) -= 2; \
        REG_MP(self) = (word)(REG_PC(self) + 1); \
        self->tstate += (17 - 4); \
    } else { \
        self->tstate += (12 - 4); \
    }

#define DO_REP_CP \
    self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
    self->tmp_byte_b = (REG_A(self) & 0x0F) - (self->tmp_byte & 0x0F); \
    self->tmp_byte = REG_A(self) - self->tmp_byte; \
    REG_BC(self)--; \
    REG_F(self) = (REG_F(self) & FLAG_C) \
        | FLAG_N \
        | (REG_BC(self) ? FLAG_PV : 0) \
        | (self->tmp_byte_b & FLAG_H) \
        | (self->tmp_byte ? 0 : FLAG_Z) \
        | (self->tmp_byte & FLAG_S); \
    self->tmp_byte -= ((REG_F(self) & FLAG_H) >> FLAG_H_TO_C); \
    REG_F(self) |= (self->tmp_byte & FLAG_3) | ((self->tmp_byte << FLAG_N_TO_5) & FLAG_5);

#define DO_REP_CP_INC \
    REG_HL(self)++; \
    REG_MP(self)++; \
    self->tstate += (12 - 4);

#define DO_REP_CP_DEC \
    REG_HL(self)--; \
    REG_MP(self)--; \
    self->tstate += (12 - 4);

#define DO_REP_CP_INCR \
    REG_HL(self)++; \
    if ((REG_F(self) & (FLAG_Z | FLAG_PV)) == FLAG_PV) { \
        REG_PC(self) -= 2; \
        REG_MP(self) = (word)(REG_PC(self) + 1); \
        self->tstate += (17 - 4); \
    } else { \
        REG_MP(self)++; \
        self->tstate += (12 - 4); \
    }

#define DO_REP_CP_DECR \
    REG_HL(self)--; \
    if ((REG_F(self) & (FLAG_Z | FLAG_PV)) == FLAG_PV) { \
        REG_PC(self) -= 2; \
        REG_MP(self) = (word)(REG_PC(self) + 1); \
        self->tstate += (17 - 4); \
    } else { \
        REG_MP(self)--; \
        self->tstate += (12 - 4); \
    }

#define DO_REP_INI \
    self->tstate += (6 - 4); \
    self->tmp_byte = self->ptr_in(REG_BC(self), self->data_in); \
    self->tstate += (9 - 6); \
    self->ptr_write(REG_HL(self), self->tmp_byte, self->data_write); \
    REG_MP(self) = (word)(REG_BC(self) + 1); \
    REG_B(self)--; \
    REG_HL(self)++; \
    self->tmp_word = (word)self->tmp_byte + (word)((byte)(REG_C(self) + 1)); \
    REG_F(self) = ((self->tmp_byte >> FLAG_S_TO_N) & FLAG_N) \
        | (REG_B(self) & (FLAG_S | FLAG_5 | FLAG_3)) \
        | (REG_B(self) ? 0 : FLAG_Z) \
        | Cpu::tbl_parity[(byte)(self->tmp_word & 0x07) ^ REG_B(self)] \
        | ((self->tmp_word > 255) ? (FLAG_C | FLAG_H) : 0);

#define DO_REP_IND \
    self->tstate += (6 - 4); \
    self->tmp_byte = self->ptr_in(REG_BC(self), self->data_in); \
    self->tstate += (9 - 6); \
    self->ptr_write(REG_HL(self), self->tmp_byte, self->data_write); \
    REG_MP(self) = (word)(REG_BC(self) - 1); \
    REG_B(self)--; \
    REG_HL(self)--; \
    self->tmp_word = (word)self->tmp_byte + (word)((byte)(REG_C(self) - 1)); \
    REG_F(self) = ((self->tmp_byte >> FLAG_S_TO_N) & FLAG_N) \
        | (REG_B(self) & (FLAG_S | FLAG_5 | FLAG_3)) \
        | (REG_B(self) ? 0 : FLAG_Z) \
        | Cpu::tbl_parity[(byte)(self->tmp_word & 0x07) ^ REG_B(self)] \
        | ((self->tmp_word > 255) ? (FLAG_C | FLAG_H) : 0);

#define DO_REP_IN_OUT_REP \
    if (REG_B(self)) { \
        REG_PC(self) -= 2; \
        self->tstate += (17 - 9); \
    } else { \
        self->tstate += (12 - 9); \
    }

#define DO_REP_OUTI \
    self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
    REG_B(self)--; \
    REG_MP(self) = (word)(REG_BC(self) + 1); \
    self->tstate += (9 - 4); \
    self->ptr_out(REG_BC(self), self->tmp_byte, self->data_out); \
    REG_HL(self)++; \
    self->tmp_word = (word)self->tmp_byte + (word)REG_L(self); \
    REG_F(self) = ((self->tmp_byte >> FLAG_S_TO_N) & FLAG_N) \
        | (REG_B(self) & (FLAG_S | FLAG_5 | FLAG_3)) \
        | (REG_B(self) ? 0 : FLAG_Z) \
        | Cpu::tbl_parity[(byte)(self->tmp_word & 0x07) ^ REG_B(self)] \
        | ((self->tmp_word > 255) ? (FLAG_C | FLAG_H) : 0);

#define DO_REP_OUTD \
    self->tmp_byte = self->ptr_read(REG_HL(self), false, self->data_read); \
    REG_B(self)--; \
    REG_MP(self) = (word)(REG_BC(self) - 1); \
    self->tstate += (9 - 4); \
    self->ptr_out(REG_BC(self), self->tmp_byte, self->data_out); \
    REG_HL(self)--; \
    self->tmp_word = (word)self->tmp_byte + (word)REG_L(self); \
    REG_F(self) = ((self->tmp_byte >> FLAG_S_TO_N) & FLAG_N) \
        | (REG_B(self) & (FLAG_S | FLAG_5 | FLAG_3)) \
        | (REG_B(self) ? 0 : FLAG_Z) \
        | Cpu::tbl_parity[(byte)(self->tmp_word & 0x07) ^ REG_B(self)] \
        | ((self->tmp_word > 255) ? (FLAG_C | FLAG_H) : 0);
