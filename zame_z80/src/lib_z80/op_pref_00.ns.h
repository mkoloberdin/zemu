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

#namespace Cpu
    OP_NOP          (::op_00_00,)                       // NOP
    OP_LD_RP_NN     (::op_00_01, REG_BC,)               // LD BC,NN
    OP_LD_MRP_A     (::op_00_02, REG_BC,)               // LD (BC),A
    OP_DO_RP        (::op_00_03, DO_INC_16, REG_BC,)    // INC BC
    OP_DO_R         (::op_00_04, DO_INC_8, REG_B,)      // INC B
    OP_DO_R         (::op_00_05, DO_DEC_8, REG_B,)      // DEC B
    OP_LD_R_N       (::op_00_06, REG_B,)                // LD B,N
    OP_RLCA         (::op_00_07,)                       // RLCA
    OP_EX_AF_AF_    (::op_00_08,)                       // EX AF,AF'
    OP_ADD_RP_RP    (::op_00_09, REG_HL, REG_BC,)       // ADD HL,BC
    OP_LD_A_MRP     (::op_00_0A, REG_BC,)               // LD A,(BC)
    OP_DO_RP        (::op_00_0B, DO_DEC_16, REG_BC,)    // DEC BC
    OP_DO_R         (::op_00_0C, DO_INC_8, REG_C,)      // INC C
    OP_DO_R         (::op_00_0D, DO_DEC_8, REG_C,)      // DEC C
    OP_LD_R_N       (::op_00_0E, REG_C,)                // LD C,N
    OP_RRCA         (::op_00_0F,)                       // RRCA

    OP_DJNZ         (::op_00_10,)                       // DJNZ O
    OP_LD_RP_NN     (::op_00_11, REG_DE,)               // LD DE,NN
    OP_LD_MRP_A     (::op_00_12, REG_DE,)               // LD (DE),A
    OP_DO_RP        (::op_00_13, DO_INC_16, REG_DE,)    // INC DE
    OP_DO_R         (::op_00_14, DO_INC_8, REG_D,)      // INC D
    OP_DO_R         (::op_00_15, DO_DEC_8, REG_D,)      // DEC D
    OP_LD_R_N       (::op_00_16, REG_D,)                // LD D,N
    OP_RLA          (::op_00_17,)                       // RLA
    OP_JR           (::op_00_18,)                       // JR O
    OP_ADD_RP_RP    (::op_00_19, REG_HL, REG_DE,)       // ADD HL,DE
    OP_LD_A_MRP     (::op_00_1A, REG_DE,)               // LD A,(DE)
    OP_DO_RP        (::op_00_1B, DO_DEC_16, REG_DE,)    // DEC DE
    OP_DO_R         (::op_00_1C, DO_INC_8, REG_E,)      // INC E
    OP_DO_R         (::op_00_1D, DO_DEC_8, REG_E,)      // DEC E
    OP_LD_R_N       (::op_00_1E, REG_E,)                // LD E,N
    OP_RRA          (::op_00_1F,)                       // RRA

    OP_JR_CC        (::op_00_20, CC_NZ,)                // JR NZ,O
    OP_LD_RP_NN     (::op_00_21, REG_HL,)               // LD HL,NN
    OP_LD_MNN_RP    (::op_00_22, REG_L, REG_H,)         // LD (NN),HL
    OP_DO_RP        (::op_00_23, DO_INC_16, REG_HL,)    // INC HL
    OP_DO_R         (::op_00_24, DO_INC_8, REG_H,)      // INC H
    OP_DO_R         (::op_00_25, DO_DEC_8, REG_H,)      // DEC H
    OP_LD_R_N       (::op_00_26, REG_H,)                // LD H,N
    OP_DAA          (::op_00_27,)                       // DAA
    OP_JR_CC        (::op_00_28, CC_Z,)                 // JR Z,O
    OP_ADD_RP_RP    (::op_00_29, REG_HL, REG_HL,)       // ADD HL,HL
    OP_LD_RP_MNN    (::op_00_2A, REG_L, REG_H,)         // LD HL,(NN)
    OP_DO_RP        (::op_00_2B, DO_DEC_16, REG_HL,)    // DEC HL
    OP_DO_R         (::op_00_2C, DO_INC_8, REG_L,)      // INC L
    OP_DO_R         (::op_00_2D, DO_DEC_8, REG_L,)      // DEC L
    OP_LD_R_N       (::op_00_2E, REG_L,)                // LD L,N
    OP_CPL          (::op_00_2F,)                       // CPL

    OP_JR_CC        (::op_00_30, CC_NC,)                // JR NC,O
    OP_LD_RP_NN     (::op_00_31, REG_SP,)               // LD SP,NN
    OP_LD_MNN_A     (::op_00_32,)                       // LD (NN),A
    OP_DO_RP        (::op_00_33, DO_INC_16, REG_SP,)    // INC SP
    OP_DO_MHL       (::op_00_34, DO_INC_8,)             // INC (HL)
    OP_DO_MHL       (::op_00_35, DO_DEC_8,)             // DEC (HL)
    OP_LD_MHL_N     (::op_00_36,)                       // LD (HL),N
    OP_SCF          (::op_00_37,)                       // SCF
    OP_JR_CC        (::op_00_38, CC_C,)                 // JR C,O
    OP_ADD_RP_RP    (::op_00_39, REG_HL, REG_SP,)       // ADD HL,SP
    OP_LD_A_MNN     (::op_00_3A,)                       // LD A,(NN)
    OP_DO_RP        (::op_00_3B, DO_DEC_16, REG_SP,)    // DEC SP
    OP_DO_R         (::op_00_3C, DO_INC_8, REG_A,)      // INC A
    OP_DO_R         (::op_00_3D, DO_DEC_8, REG_A,)      // DEC A
    OP_LD_R_N       (::op_00_3E, REG_A,)                // LD A,N
    OP_CCF          (::op_00_3F,)                       // CCF

    OP_LD_R_R       (::op_00_40, REG_B, REG_B,)         // LD B,B
    OP_LD_R_R       (::op_00_41, REG_B, REG_C,)         // LD B,C
    OP_LD_R_R       (::op_00_42, REG_B, REG_D,)         // LD B,D
    OP_LD_R_R       (::op_00_43, REG_B, REG_E,)         // LD B,E
    OP_LD_R_R       (::op_00_44, REG_B, REG_H,)         // LD B,H
    OP_LD_R_R       (::op_00_45, REG_B, REG_L,)         // LD B,L
    OP_LD_R_MRP     (::op_00_46, REG_B, REG_HL)         // LD B,(HL)
    OP_LD_R_R       (::op_00_47, REG_B, REG_A,)         // LD B,A
    OP_LD_R_R       (::op_00_48, REG_C, REG_B,)         // LD C,B
    OP_LD_R_R       (::op_00_49, REG_C, REG_C,)         // LD C,C
    OP_LD_R_R       (::op_00_4A, REG_C, REG_D,)         // LD C,D
    OP_LD_R_R       (::op_00_4B, REG_C, REG_E,)         // LD C,E
    OP_LD_R_R       (::op_00_4C, REG_C, REG_H,)         // LD C,H
    OP_LD_R_R       (::op_00_4D, REG_C, REG_L,)         // LD C,L
    OP_LD_R_MRP     (::op_00_4E, REG_C, REG_HL)         // LD C,(HL)
    OP_LD_R_R       (::op_00_4F, REG_C, REG_A,)         // LD C,A

    OP_LD_R_R       (::op_00_50, REG_D, REG_B,)         // LD D,B
    OP_LD_R_R       (::op_00_51, REG_D, REG_C,)         // LD D,C
    OP_LD_R_R       (::op_00_52, REG_D, REG_D,)         // LD D,D
    OP_LD_R_R       (::op_00_53, REG_D, REG_E,)         // LD D,E
    OP_LD_R_R       (::op_00_54, REG_D, REG_H,)         // LD D,H
    OP_LD_R_R       (::op_00_55, REG_D, REG_L,)         // LD D,L
    OP_LD_R_MRP     (::op_00_56, REG_D, REG_HL)         // LD D,(HL)
    OP_LD_R_R       (::op_00_57, REG_D, REG_A,)         // LD D,A
    OP_LD_R_R       (::op_00_58, REG_E, REG_B,)         // LD E,B
    OP_LD_R_R       (::op_00_59, REG_E, REG_C,)         // LD E,C
    OP_LD_R_R       (::op_00_5A, REG_E, REG_D,)         // LD E,D
    OP_LD_R_R       (::op_00_5B, REG_E, REG_E,)         // LD E,E
    OP_LD_R_R       (::op_00_5C, REG_E, REG_H,)         // LD E,H
    OP_LD_R_R       (::op_00_5D, REG_E, REG_L,)         // LD E,L
    OP_LD_R_MRP     (::op_00_5E, REG_E, REG_HL)         // LD E,(HL)
    OP_LD_R_R       (::op_00_5F, REG_E, REG_A,)         // LD E,A

    OP_LD_R_R       (::op_00_60, REG_H, REG_B,)         // LD H,B
    OP_LD_R_R       (::op_00_61, REG_H, REG_C,)         // LD H,C
    OP_LD_R_R       (::op_00_62, REG_H, REG_D,)         // LD H,D
    OP_LD_R_R       (::op_00_63, REG_H, REG_E,)         // LD H,E
    OP_LD_R_R       (::op_00_64, REG_H, REG_H,)         // LD H,H
    OP_LD_R_R       (::op_00_65, REG_H, REG_L,)         // LD H,L
    OP_LD_R_MRP     (::op_00_66, REG_H, REG_HL)         // LD H,(HL)
    OP_LD_R_R       (::op_00_67, REG_H, REG_A,)         // LD H,A
    OP_LD_R_R       (::op_00_68, REG_L, REG_B,)         // LD L,B
    OP_LD_R_R       (::op_00_69, REG_L, REG_C,)         // LD L,C
    OP_LD_R_R       (::op_00_6A, REG_L, REG_D,)         // LD L,D
    OP_LD_R_R       (::op_00_6B, REG_L, REG_E,)         // LD L,E
    OP_LD_R_R       (::op_00_6C, REG_L, REG_H,)         // LD L,H
    OP_LD_R_R       (::op_00_6D, REG_L, REG_L,)         // LD L,L
    OP_LD_R_MRP     (::op_00_6E, REG_L, REG_HL)         // LD L,(HL)
    OP_LD_R_R       (::op_00_6F, REG_L, REG_A,)         // LD L,A

    OP_LD_MRP_R     (::op_00_70, REG_HL, REG_B,)        // LD (HL),B
    OP_LD_MRP_R     (::op_00_71, REG_HL, REG_C,)        // LD (HL),C
    OP_LD_MRP_R     (::op_00_72, REG_HL, REG_D,)        // LD (HL),D
    OP_LD_MRP_R     (::op_00_73, REG_HL, REG_E,)        // LD (HL),E
    OP_LD_MRP_R     (::op_00_74, REG_HL, REG_H,)        // LD (HL),H
    OP_LD_MRP_R     (::op_00_75, REG_HL, REG_L,)        // LD (HL),L
    OP_HALT         (::op_00_76,)                       // HALT
    OP_LD_MRP_R     (::op_00_77, REG_HL, REG_A,)        // LD (HL),A
    OP_LD_R_R       (::op_00_78, REG_A, REG_B,)         // LD A,B
    OP_LD_R_R       (::op_00_79, REG_A, REG_C,)         // LD A,C
    OP_LD_R_R       (::op_00_7A, REG_A, REG_D,)         // LD A,D
    OP_LD_R_R       (::op_00_7B, REG_A, REG_E,)         // LD A,E
    OP_LD_R_R       (::op_00_7C, REG_A, REG_H,)         // LD A,H
    OP_LD_R_R       (::op_00_7D, REG_A, REG_L,)         // LD A,L
    OP_LD_R_MRP     (::op_00_7E, REG_A, REG_HL)         // LD A,(HL)
    OP_LD_R_R       (::op_00_7F, REG_A, REG_A,)         // LD A,A

    OP_DO_A_R       (::op_00_80, DO_ADD_8, REG_B,)      // ADD A,B
    OP_DO_A_R       (::op_00_81, DO_ADD_8, REG_C,)      // ADD A,C
    OP_DO_A_R       (::op_00_82, DO_ADD_8, REG_D,)      // ADD A,D
    OP_DO_A_R       (::op_00_83, DO_ADD_8, REG_E,)      // ADD A,E
    OP_DO_A_R       (::op_00_84, DO_ADD_8, REG_H,)      // ADD A,H
    OP_DO_A_R       (::op_00_85, DO_ADD_8, REG_L,)      // ADD A,L
    OP_DO_A_MHL     (::op_00_86, DO_ADD_8,)             // ADD A,(HL)
    OP_DO_A_R       (::op_00_87, DO_ADD_8, REG_A,)      // ADD A,A
    OP_DO_A_R       (::op_00_88, DO_ADC_8, REG_B,)      // ADC A,B
    OP_DO_A_R       (::op_00_89, DO_ADC_8, REG_C,)      // ADC A,C
    OP_DO_A_R       (::op_00_8A, DO_ADC_8, REG_D,)      // ADC A,D
    OP_DO_A_R       (::op_00_8B, DO_ADC_8, REG_E,)      // ADC A,E
    OP_DO_A_R       (::op_00_8C, DO_ADC_8, REG_H,)      // ADC A,H
    OP_DO_A_R       (::op_00_8D, DO_ADC_8, REG_L,)      // ADC A,L
    OP_DO_A_MHL     (::op_00_8E, DO_ADC_8,)             // ADC A,(HL)
    OP_DO_A_R       (::op_00_8F, DO_ADC_8, REG_A,)      // ADC A,A

    OP_DO_A_R       (::op_00_90, DO_SUB_8, REG_B,)      // SUB B
    OP_DO_A_R       (::op_00_91, DO_SUB_8, REG_C,)      // SUB C
    OP_DO_A_R       (::op_00_92, DO_SUB_8, REG_D,)      // SUB D
    OP_DO_A_R       (::op_00_93, DO_SUB_8, REG_E,)      // SUB E
    OP_DO_A_R       (::op_00_94, DO_SUB_8, REG_H,)      // SUB H
    OP_DO_A_R       (::op_00_95, DO_SUB_8, REG_L,)      // SUB L
    OP_DO_A_MHL     (::op_00_96, DO_SUB_8,)             // SUB (HL)
    OP_DO_A_R       (::op_00_97, DO_SUB_8, REG_A,)      // SUB A
    OP_DO_A_R       (::op_00_98, DO_SBC_8, REG_B,)      // SBC A,B
    OP_DO_A_R       (::op_00_99, DO_SBC_8, REG_C,)      // SBC A,C
    OP_DO_A_R       (::op_00_9A, DO_SBC_8, REG_D,)      // SBC A,D
    OP_DO_A_R       (::op_00_9B, DO_SBC_8, REG_E,)      // SBC A,E
    OP_DO_A_R       (::op_00_9C, DO_SBC_8, REG_H,)      // SBC A,H
    OP_DO_A_R       (::op_00_9D, DO_SBC_8, REG_L,)      // SBC A,L
    OP_DO_A_MHL     (::op_00_9E, DO_SBC_8,)             // SBC A,(HL)
    OP_DO_A_R       (::op_00_9F, DO_SBC_8, REG_A,)      // SBC A,A

    OP_DO_A_R       (::op_00_A0, DO_AND_8, REG_B,)      // AND B
    OP_DO_A_R       (::op_00_A1, DO_AND_8, REG_C,)      // AND C
    OP_DO_A_R       (::op_00_A2, DO_AND_8, REG_D,)      // AND D
    OP_DO_A_R       (::op_00_A3, DO_AND_8, REG_E,)      // AND E
    OP_DO_A_R       (::op_00_A4, DO_AND_8, REG_H,)      // AND H
    OP_DO_A_R       (::op_00_A5, DO_AND_8, REG_L,)      // AND L
    OP_DO_A_MHL     (::op_00_A6, DO_AND_8,)             // AND (HL)
    OP_DO_A_R       (::op_00_A7, DO_AND_8, REG_A,)      // AND A
    OP_DO_A_R       (::op_00_A8, DO_XOR_8, REG_B,)      // XOR B
    OP_DO_A_R       (::op_00_A9, DO_XOR_8, REG_C,)      // XOR C
    OP_DO_A_R       (::op_00_AA, DO_XOR_8, REG_D,)      // XOR D
    OP_DO_A_R       (::op_00_AB, DO_XOR_8, REG_E,)      // XOR E
    OP_DO_A_R       (::op_00_AC, DO_XOR_8, REG_H,)      // XOR H
    OP_DO_A_R       (::op_00_AD, DO_XOR_8, REG_L,)      // XOR L
    OP_DO_A_MHL     (::op_00_AE, DO_XOR_8,)             // XOR (HL)
    OP_DO_A_R       (::op_00_AF, DO_XOR_8, REG_A,)      // XOR A

    OP_DO_A_R       (::op_00_B0, DO_OR_8, REG_B,)       // OR B
    OP_DO_A_R       (::op_00_B1, DO_OR_8, REG_C,)       // OR C
    OP_DO_A_R       (::op_00_B2, DO_OR_8, REG_D,)       // OR D
    OP_DO_A_R       (::op_00_B3, DO_OR_8, REG_E,)       // OR E
    OP_DO_A_R       (::op_00_B4, DO_OR_8, REG_H,)       // OR H
    OP_DO_A_R       (::op_00_B5, DO_OR_8, REG_L,)       // OR L
    OP_DO_A_MHL     (::op_00_B6, DO_OR_8,)              // OR (HL)
    OP_DO_A_R       (::op_00_B7, DO_OR_8, REG_A,)       // OR A
    OP_DO_A_R       (::op_00_B8, DO_CP_8, REG_B,)       // CP B
    OP_DO_A_R       (::op_00_B9, DO_CP_8, REG_C,)       // CP C
    OP_DO_A_R       (::op_00_BA, DO_CP_8, REG_D,)       // CP D
    OP_DO_A_R       (::op_00_BB, DO_CP_8, REG_E,)       // CP E
    OP_DO_A_R       (::op_00_BC, DO_CP_8, REG_H,)       // CP H
    OP_DO_A_R       (::op_00_BD, DO_CP_8, REG_L,)       // CP L
    OP_DO_A_MHL     (::op_00_BE, DO_CP_8,)              // CP (HL)
    OP_DO_A_R       (::op_00_BF, DO_CP_8, REG_A,)       // CP A

    OP_RET_CC       (::op_00_C0, CC_NZ,)                // RET NZ
    OP_POP_RP       (::op_00_C1, REG_BC,)               // POP BC
    OP_JP_CC        (::op_00_C2, CC_NZ,)                // JP NZ,NN
    OP_JP           (::op_00_C3,)                       // JP NN
    OP_CALL_CC      (::op_00_C4, CC_NZ,)                // CALL NZ,NN
    OP_PUSH_RP      (::op_00_C5, REG_BC,)               // PUSH BC
    OP_DO_A_N       (::op_00_C6, DO_ADD_8,)             // ADD A,N
    OP_RST          (::op_00_C7, 0x00,)                 // RST #00
    OP_RET_CC       (::op_00_C8, CC_Z,)                 // RET Z
    OP_RET          (::op_00_C9,)                       // RET
    OP_JP_CC        (::op_00_CA, CC_Z,)                 // JP Z,NN
    OP_PREF_CB      (::op_00_CB)                        // prefix #CB
    OP_CALL_CC      (::op_00_CC, CC_Z,)                 // CALL Z,NN
    OP_CALL         (::op_00_CD,)                       // CALL NN
    OP_DO_A_N       (::op_00_CE, DO_ADC_8,)             // ADC A,N
    OP_RST          (::op_00_CF, 0x08,)                 // RST #08

    OP_RET_CC       (::op_00_D0, CC_NC,)                // RET NC
    OP_POP_RP       (::op_00_D1, REG_DE,)               // POP DE
    OP_JP_CC        (::op_00_D2, CC_NC,)                // JP NC,NN
    OP_OUT_N_A      (::op_00_D3,)                       // OUT (N),A
    OP_CALL_CC      (::op_00_D4, CC_NC,)                // CALL NC,NN
    OP_PUSH_RP      (::op_00_D5, REG_DE,)               // PUSH DE
    OP_DO_A_N       (::op_00_D6, DO_SUB_8,)             // SUB N
    OP_RST          (::op_00_D7, 0x10,)                 // RST #10
    OP_RET_CC       (::op_00_D8, CC_C,)                 // RET C
    OP_EXX          (::op_00_D9,)                       // EXX
    OP_JP_CC        (::op_00_DA, CC_C,)                 // JP C,NN
    OP_IN_A_N       (::op_00_DB,)                       // IN A,(N)
    OP_CALL_CC      (::op_00_DC, CC_C,)                 // CALL C,NN
    OP_PREF_DD      (::op_00_DD)                        // prefix #DD
    OP_DO_A_N       (::op_00_DE, DO_SBC_8,)             // SBC A,N
    OP_RST          (::op_00_DF, 0x18,)                 // RST #18

    OP_RET_CC       (::op_00_E0, CC_PO,)                // RET PO
    OP_POP_RP       (::op_00_E1, REG_HL,)               // POP HL
    OP_JP_CC        (::op_00_E2, CC_PO,)                // JP PO,NN
    OP_EX_SP_RP     (::op_00_E3, REG_L, REG_H, REG_HL,) // EX (SP),HL
    OP_CALL_CC      (::op_00_E4, CC_PO,)                // CALL PO,NN
    OP_PUSH_RP      (::op_00_E5, REG_HL,)               // PUSH HL
    OP_DO_A_N       (::op_00_E6, DO_AND_8,)             // AND N
    OP_RST          (::op_00_E7, 0x20,)                 // RST #20
    OP_RET_CC       (::op_00_E8, CC_PE,)                // RET PE
    OP_JP_RP        (::op_00_E9, REG_HL,)               // JP HL
    OP_JP_CC        (::op_00_EA, CC_PE,)                // JP PE,NN
    OP_EX_DE_HL     (::op_00_EB,)                       // EX DE,HL
    OP_CALL_CC      (::op_00_EC, CC_PE,)                // CALL PE,NN
    OP_PREF_ED      (::op_00_ED)                        // prefix #ED
    OP_DO_A_N       (::op_00_EE, DO_XOR_8,)             // XOR N
    OP_RST          (::op_00_EF, 0x28,)                 // RST #28

    OP_RET_CC       (::op_00_F0, CC_P,)             // RET P
    OP_POP_RP       (::op_00_F1, REG_AF,)           // POP AF
    OP_JP_CC        (::op_00_F2, CC_P,)             // JP P,NN
    OP_DI           (::op_00_F3,)                   // DI
    OP_CALL_CC      (::op_00_F4, CC_P,)             // CALL P,NN
    OP_PUSH_RP      (::op_00_F5, REG_AF,)           // PUSH AF
    OP_DO_A_N       (::op_00_F6, DO_OR_8,)          // OR N
    OP_RST          (::op_00_F7, 0x30,)             // RST #30
    OP_RET_CC       (::op_00_F8, CC_M,)             // RET M
    OP_LD_SP_RP     (::op_00_F9, REG_HL,)           // LD SP,HL
    OP_JP_CC        (::op_00_FA, CC_M,)             // JP M,NN
    OP_EI           (::op_00_FB,)                   // EI
    OP_CALL_CC      (::op_00_FC, CC_M,)             // CALL M,NN
    OP_PREF_FD      (::op_00_FD)                    // prefix #FD
    OP_DO_A_N       (::op_00_FE, DO_CP_8,)          // CP N
    OP_RST          (::op_00_FF, 0x38,)             // RST #38

    ::t_opcode optable_00[0x100] = {
        ::op_00_00, ::op_00_01, ::op_00_02, ::op_00_03, ::op_00_04, ::op_00_05, ::op_00_06, ::op_00_07,
        ::op_00_08, ::op_00_09, ::op_00_0A, ::op_00_0B, ::op_00_0C, ::op_00_0D, ::op_00_0E, ::op_00_0F,
        ::op_00_10, ::op_00_11, ::op_00_12, ::op_00_13, ::op_00_14, ::op_00_15, ::op_00_16, ::op_00_17,
        ::op_00_18, ::op_00_19, ::op_00_1A, ::op_00_1B, ::op_00_1C, ::op_00_1D, ::op_00_1E, ::op_00_1F,
        ::op_00_20, ::op_00_21, ::op_00_22, ::op_00_23, ::op_00_24, ::op_00_25, ::op_00_26, ::op_00_27,
        ::op_00_28, ::op_00_29, ::op_00_2A, ::op_00_2B, ::op_00_2C, ::op_00_2D, ::op_00_2E, ::op_00_2F,
        ::op_00_30, ::op_00_31, ::op_00_32, ::op_00_33, ::op_00_34, ::op_00_35, ::op_00_36, ::op_00_37,
        ::op_00_38, ::op_00_39, ::op_00_3A, ::op_00_3B, ::op_00_3C, ::op_00_3D, ::op_00_3E, ::op_00_3F,
        ::op_00_40, ::op_00_41, ::op_00_42, ::op_00_43, ::op_00_44, ::op_00_45, ::op_00_46, ::op_00_47,
        ::op_00_48, ::op_00_49, ::op_00_4A, ::op_00_4B, ::op_00_4C, ::op_00_4D, ::op_00_4E, ::op_00_4F,
        ::op_00_50, ::op_00_51, ::op_00_52, ::op_00_53, ::op_00_54, ::op_00_55, ::op_00_56, ::op_00_57,
        ::op_00_58, ::op_00_59, ::op_00_5A, ::op_00_5B, ::op_00_5C, ::op_00_5D, ::op_00_5E, ::op_00_5F,
        ::op_00_60, ::op_00_61, ::op_00_62, ::op_00_63, ::op_00_64, ::op_00_65, ::op_00_66, ::op_00_67,
        ::op_00_68, ::op_00_69, ::op_00_6A, ::op_00_6B, ::op_00_6C, ::op_00_6D, ::op_00_6E, ::op_00_6F,
        ::op_00_70, ::op_00_71, ::op_00_72, ::op_00_73, ::op_00_74, ::op_00_75, ::op_00_76, ::op_00_77,
        ::op_00_78, ::op_00_79, ::op_00_7A, ::op_00_7B, ::op_00_7C, ::op_00_7D, ::op_00_7E, ::op_00_7F,
        ::op_00_80, ::op_00_81, ::op_00_82, ::op_00_83, ::op_00_84, ::op_00_85, ::op_00_86, ::op_00_87,
        ::op_00_88, ::op_00_89, ::op_00_8A, ::op_00_8B, ::op_00_8C, ::op_00_8D, ::op_00_8E, ::op_00_8F,
        ::op_00_90, ::op_00_91, ::op_00_92, ::op_00_93, ::op_00_94, ::op_00_95, ::op_00_96, ::op_00_97,
        ::op_00_98, ::op_00_99, ::op_00_9A, ::op_00_9B, ::op_00_9C, ::op_00_9D, ::op_00_9E, ::op_00_9F,
        ::op_00_A0, ::op_00_A1, ::op_00_A2, ::op_00_A3, ::op_00_A4, ::op_00_A5, ::op_00_A6, ::op_00_A7,
        ::op_00_A8, ::op_00_A9, ::op_00_AA, ::op_00_AB, ::op_00_AC, ::op_00_AD, ::op_00_AE, ::op_00_AF,
        ::op_00_B0, ::op_00_B1, ::op_00_B2, ::op_00_B3, ::op_00_B4, ::op_00_B5, ::op_00_B6, ::op_00_B7,
        ::op_00_B8, ::op_00_B9, ::op_00_BA, ::op_00_BB, ::op_00_BC, ::op_00_BD, ::op_00_BE, ::op_00_BF,
        ::op_00_C0, ::op_00_C1, ::op_00_C2, ::op_00_C3, ::op_00_C4, ::op_00_C5, ::op_00_C6, ::op_00_C7,
        ::op_00_C8, ::op_00_C9, ::op_00_CA, ::op_00_CB, ::op_00_CC, ::op_00_CD, ::op_00_CE, ::op_00_CF,
        ::op_00_D0, ::op_00_D1, ::op_00_D2, ::op_00_D3, ::op_00_D4, ::op_00_D5, ::op_00_D6, ::op_00_D7,
        ::op_00_D8, ::op_00_D9, ::op_00_DA, ::op_00_DB, ::op_00_DC, ::op_00_DD, ::op_00_DE, ::op_00_DF,
        ::op_00_E0, ::op_00_E1, ::op_00_E2, ::op_00_E3, ::op_00_E4, ::op_00_E5, ::op_00_E6, ::op_00_E7,
        ::op_00_E8, ::op_00_E9, ::op_00_EA, ::op_00_EB, ::op_00_EC, ::op_00_ED, ::op_00_EE, ::op_00_EF,
        ::op_00_F0, ::op_00_F1, ::op_00_F2, ::op_00_F3, ::op_00_F4, ::op_00_F5, ::op_00_F6, ::op_00_F7,
        ::op_00_F8, ::op_00_F9, ::op_00_FA, ::op_00_FB, ::op_00_FC, ::op_00_FD, ::op_00_FE, ::op_00_FF
    };
#end
