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
    OP_NOP          (::op_FD_00, DO_PREF_00)                    // *NOP
    OP_LD_RP_NN     (::op_FD_01, REG_BC, DO_PREF_00)            // *LD BC,NN
    OP_LD_MRP_A     (::op_FD_02, REG_BC, DO_PREF_00)            // *LD (BC),A
    OP_DO_RP        (::op_FD_03, DO_INC_16, REG_BC, DO_PREF_00) // *INC BC
    OP_DO_R         (::op_FD_04, DO_INC_8, REG_B, DO_PREF_00)   // *INC B
    OP_DO_R         (::op_FD_05, DO_DEC_8, REG_B, DO_PREF_00)   // *DEC B
    OP_LD_R_N       (::op_FD_06, REG_B, DO_PREF_00)             // *LD B,N
    OP_RLCA         (::op_FD_07, DO_PREF_00)                    // *RLCA
    OP_EX_AF_AF_    (::op_FD_08, DO_PREF_00)                    // *EX AF,AF'
    OP_ADD_RP_RP    (::op_FD_09, REG_IY, REG_BC, DO_PREF_00)    // ADD IY,BC
    OP_LD_A_MRP     (::op_FD_0A, REG_BC, DO_PREF_00)            // *LD A,(BC)
    OP_DO_RP        (::op_FD_0B, DO_DEC_16, REG_BC, DO_PREF_00) // *DEC BC
    OP_DO_R         (::op_FD_0C, DO_INC_8, REG_C, DO_PREF_00)   // *INC C
    OP_DO_R         (::op_FD_0D, DO_DEC_8, REG_C, DO_PREF_00)   // *DEC C
    OP_LD_R_N       (::op_FD_0E, REG_C, DO_PREF_00)             // *LD C,N
    OP_RRCA         (::op_FD_0F, DO_PREF_00)                    // *RRCA

    OP_DJNZ         (::op_FD_10, DO_PREF_00)                    // *DJNZ O
    OP_LD_RP_NN     (::op_FD_11, REG_DE, DO_PREF_00)            // *LD DE,NN
    OP_LD_MRP_A     (::op_FD_12, REG_DE, DO_PREF_00)            // *LD (DE),A
    OP_DO_RP        (::op_FD_13, DO_INC_16, REG_DE, DO_PREF_00) // *INC DE
    OP_DO_R         (::op_FD_14, DO_INC_8, REG_D, DO_PREF_00)   // *INC D
    OP_DO_R         (::op_FD_15, DO_DEC_8, REG_D, DO_PREF_00)   // *DEC D
    OP_LD_R_N       (::op_FD_16, REG_D, DO_PREF_00)             // *LD D,N
    OP_RLA          (::op_FD_17, DO_PREF_00)                    // *RLA
    OP_JR           (::op_FD_18, DO_PREF_00)                    // *JR O
    OP_ADD_RP_RP    (::op_FD_19, REG_IY, REG_DE, DO_PREF_00)    // ADD IY,DE
    OP_LD_A_MRP     (::op_FD_1A, REG_DE, DO_PREF_00)            // *LD A,(DE)
    OP_DO_RP        (::op_FD_1B, DO_DEC_16, REG_DE, DO_PREF_00) // *DEC DE
    OP_DO_R         (::op_FD_1C, DO_INC_8, REG_E, DO_PREF_00)   // *INC E
    OP_DO_R         (::op_FD_1D, DO_DEC_8, REG_E, DO_PREF_00)   // *DEC E
    OP_LD_R_N       (::op_FD_1E, REG_E, DO_PREF_00)             // *LD E,N
    OP_RRA          (::op_FD_1F, DO_PREF_00)                    // *RRA

    OP_JR_CC        (::op_FD_20, CC_NZ, DO_PREF_00)             // *JR NZ,O
    OP_LD_RP_NN     (::op_FD_21, REG_IY, DO_PREF_00)            // LD IY,NN
    OP_LD_MNN_RP    (::op_FD_22, REG_IYL, REG_IYH, DO_PREF_00)  // LD (NN),IY
    OP_DO_RP        (::op_FD_23, DO_INC_16, REG_IY, DO_PREF_00) // INC IY
    OP_DO_R         (::op_FD_24, DO_INC_8, REG_IYH, DO_PREF_00) // INC IYH
    OP_DO_R         (::op_FD_25, DO_DEC_8, REG_IYH, DO_PREF_00) // DEC IYH
    OP_LD_R_N       (::op_FD_26, REG_IYH, DO_PREF_00)           // LD IYH,N
    OP_DAA          (::op_FD_27, DO_PREF_00)                    // *DAA
    OP_JR_CC        (::op_FD_28, CC_Z, DO_PREF_00)              // *JR Z,O
    OP_ADD_RP_RP    (::op_FD_29, REG_IY, REG_IY, DO_PREF_00)    // ADD IY,IY
    OP_LD_RP_MNN    (::op_FD_2A, REG_IYL, REG_IYH, DO_PREF_00)  // LD IY,(NN)
    OP_DO_RP        (::op_FD_2B, DO_DEC_16, REG_IY, DO_PREF_00) // DEC IY
    OP_DO_R         (::op_FD_2C, DO_INC_8, REG_IYL, DO_PREF_00) // INC IYL
    OP_DO_R         (::op_FD_2D, DO_DEC_8, REG_IYL, DO_PREF_00) // DEC IYL
    OP_LD_R_N       (::op_FD_2E, REG_IYL, DO_PREF_00)           // LD IYL,N
    OP_CPL          (::op_FD_2F, DO_PREF_00)                    // *CPL

    OP_JR_CC        (::op_FD_30, CC_NC, DO_PREF_00)             // *JR NC,O
    OP_LD_RP_NN     (::op_FD_31, REG_SP, DO_PREF_00)            // *LD SP,NN
    OP_LD_MNN_A     (::op_FD_32, DO_PREF_00)                    // *LD (NN),A
    OP_DO_RP        (::op_FD_33, DO_INC_16, REG_SP, DO_PREF_00) // *INC SP
    OP_DO_ORP       (::op_FD_34, DO_INC_8, REG_IY)              // INC (IY+N)
    OP_DO_ORP       (::op_FD_35, DO_DEC_8, REG_IY)              // DEC (IY+N)
    OP_LD_ORP_N     (::op_FD_36, REG_IY)                        // LD (IY+N),N
    OP_SCF          (::op_FD_37, DO_PREF_00)                    // *SCF
    OP_JR_CC        (::op_FD_38, CC_C, DO_PREF_00)              // *JR C,O
    OP_ADD_RP_RP    (::op_FD_39, REG_IY, REG_SP, DO_PREF_00)    // ADD IY,SP
    OP_LD_A_MNN     (::op_FD_3A, DO_PREF_00)                    // *LD A,(NN)
    OP_DO_RP        (::op_FD_3B, DO_DEC_16, REG_SP, DO_PREF_00) // *DEC SP
    OP_DO_R         (::op_FD_3C, DO_INC_8, REG_A, DO_PREF_00)   // *INC A
    OP_DO_R         (::op_FD_3D, DO_DEC_8, REG_A, DO_PREF_00)   // *DEC A
    OP_LD_R_N       (::op_FD_3E, REG_A, DO_PREF_00)             // *LD A,N
    OP_CCF          (::op_FD_3F, DO_PREF_00)                    // *CCF

    OP_LD_R_R       (::op_FD_40, REG_B, REG_B, DO_PREF_00)      // *LD B,B
    OP_LD_R_R       (::op_FD_41, REG_B, REG_C, DO_PREF_00)      // *LD B,C
    OP_LD_R_R       (::op_FD_42, REG_B, REG_D, DO_PREF_00)      // *LD B,D
    OP_LD_R_R       (::op_FD_43, REG_B, REG_E, DO_PREF_00)      // *LD B,E
    OP_LD_R_R       (::op_FD_44, REG_B, REG_IYH, DO_PREF_00)    // LD B,IYH
    OP_LD_R_R       (::op_FD_45, REG_B, REG_IYL, DO_PREF_00)    // LD B,IYL
    OP_LD_R_ORP     (::op_FD_46, REG_B, REG_IY)                 // LD B,(IY+N)
    OP_LD_R_R       (::op_FD_47, REG_B, REG_A, DO_PREF_00)      // *LD B,A
    OP_LD_R_R       (::op_FD_48, REG_C, REG_B, DO_PREF_00)      // *LD C,B
    OP_LD_R_R       (::op_FD_49, REG_C, REG_C, DO_PREF_00)      // *LD C,C
    OP_LD_R_R       (::op_FD_4A, REG_C, REG_D, DO_PREF_00)      // *LD C,D
    OP_LD_R_R       (::op_FD_4B, REG_C, REG_E, DO_PREF_00)      // *LD C,E
    OP_LD_R_R       (::op_FD_4C, REG_C, REG_IYH, DO_PREF_00)    // LD C,IYH
    OP_LD_R_R       (::op_FD_4D, REG_C, REG_IYL, DO_PREF_00)    // LD C,IYL
    OP_LD_R_ORP     (::op_FD_4E, REG_C, REG_IY)                 // LD C,(IY+N)
    OP_LD_R_R       (::op_FD_4F, REG_C, REG_A, DO_PREF_00)      // *LD C,A

    OP_LD_R_R       (::op_FD_50, REG_D, REG_B, DO_PREF_00)      // *LD D,B
    OP_LD_R_R       (::op_FD_51, REG_D, REG_C, DO_PREF_00)      // *LD D,C
    OP_LD_R_R       (::op_FD_52, REG_D, REG_D, DO_PREF_00)      // *LD D,D
    OP_LD_R_R       (::op_FD_53, REG_D, REG_E, DO_PREF_00)      // *LD D,E
    OP_LD_R_R       (::op_FD_54, REG_D, REG_IYH, DO_PREF_00)    // LD D,IYH
    OP_LD_R_R       (::op_FD_55, REG_D, REG_IYL, DO_PREF_00)    // LD D,IYL
    OP_LD_R_ORP     (::op_FD_56, REG_D, REG_IY)                 // LD D,(IY+N)
    OP_LD_R_R       (::op_FD_57, REG_D, REG_A, DO_PREF_00)      // *LD D,A
    OP_LD_R_R       (::op_FD_58, REG_E, REG_B, DO_PREF_00)      // *LD E,B
    OP_LD_R_R       (::op_FD_59, REG_E, REG_C, DO_PREF_00)      // *LD E,C
    OP_LD_R_R       (::op_FD_5A, REG_E, REG_D, DO_PREF_00)      // *LD E,D
    OP_LD_R_R       (::op_FD_5B, REG_E, REG_E, DO_PREF_00)      // *LD E,E
    OP_LD_R_R       (::op_FD_5C, REG_E, REG_IYH, DO_PREF_00)    // LD E,IYH
    OP_LD_R_R       (::op_FD_5D, REG_E, REG_IYL, DO_PREF_00)    // LD E,IYL
    OP_LD_R_ORP     (::op_FD_5E, REG_E, REG_IY)                 // LD E,(IY+N)
    OP_LD_R_R       (::op_FD_5F, REG_E, REG_A, DO_PREF_00)      // *LD E,A

    OP_LD_R_R       (::op_FD_60, REG_IYH, REG_B, DO_PREF_00)    // LD IYH,B
    OP_LD_R_R       (::op_FD_61, REG_IYH, REG_C, DO_PREF_00)    // LD IYH,C
    OP_LD_R_R       (::op_FD_62, REG_IYH, REG_D, DO_PREF_00)    // LD IYH,D
    OP_LD_R_R       (::op_FD_63, REG_IYH, REG_E, DO_PREF_00)    // LD IYH,E
    OP_LD_R_R       (::op_FD_64, REG_IYH, REG_IYH, DO_PREF_00)  // LD IYH,H
    OP_LD_R_R       (::op_FD_65, REG_IYH, REG_IYL, DO_PREF_00)  // LD IYH,L
    OP_LD_R_ORP     (::op_FD_66, REG_H, REG_IY)                 // LD H,(IY+N)
    OP_LD_R_R       (::op_FD_67, REG_IYH, REG_A, DO_PREF_00)    // LD IYH,A
    OP_LD_R_R       (::op_FD_68, REG_IYL, REG_B, DO_PREF_00)    // LD IYL,B
    OP_LD_R_R       (::op_FD_69, REG_IYL, REG_C, DO_PREF_00)    // LD IYL,C
    OP_LD_R_R       (::op_FD_6A, REG_IYL, REG_D, DO_PREF_00)    // LD IYL,D
    OP_LD_R_R       (::op_FD_6B, REG_IYL, REG_E, DO_PREF_00)    // LD IYL,E
    OP_LD_R_R       (::op_FD_6C, REG_IYL, REG_IYH, DO_PREF_00)  // LD IYL,H
    OP_LD_R_R       (::op_FD_6D, REG_IYL, REG_IYL, DO_PREF_00)  // LD IYL,L
    OP_LD_R_ORP     (::op_FD_6E, REG_L, REG_IY)                 // LD L,(IY+N)
    OP_LD_R_R       (::op_FD_6F, REG_IYL, REG_A, DO_PREF_00)    // LD IYL,A

    OP_LD_ORP_R     (::op_FD_70, REG_IY, REG_B)                 // LD (IY+N),B
    OP_LD_ORP_R     (::op_FD_71, REG_IY, REG_C)                 // LD (IY+N),C
    OP_LD_ORP_R     (::op_FD_72, REG_IY, REG_D)                 // LD (IY+N),D
    OP_LD_ORP_R     (::op_FD_73, REG_IY, REG_E)                 // LD (IY+N),E
    OP_LD_ORP_R     (::op_FD_74, REG_IY, REG_H)                 // LD (IY+N),H
    OP_LD_ORP_R     (::op_FD_75, REG_IY, REG_L)                 // LD (IY+N),L
    OP_HALT         (::op_FD_76, DO_PREF_00)                    // *HALT
    OP_LD_ORP_R     (::op_FD_77, REG_IY, REG_A)                 // LD (IY+N),A
    OP_LD_R_R       (::op_FD_78, REG_A, REG_B, DO_PREF_00)      // *LD A,B
    OP_LD_R_R       (::op_FD_79, REG_A, REG_C, DO_PREF_00)      // *LD A,C
    OP_LD_R_R       (::op_FD_7A, REG_A, REG_D, DO_PREF_00)      // *LD A,D
    OP_LD_R_R       (::op_FD_7B, REG_A, REG_E, DO_PREF_00)      // *LD A,E
    OP_LD_R_R       (::op_FD_7C, REG_A, REG_IYH, DO_PREF_00)    // LD A,IYH
    OP_LD_R_R       (::op_FD_7D, REG_A, REG_IYL, DO_PREF_00)    // LD A,IYL
    OP_LD_R_ORP     (::op_FD_7E, REG_A, REG_IY)                 // LD A,(IY+N)
    OP_LD_R_R       (::op_FD_7F, REG_A, REG_A, DO_PREF_00)      // *LD A,A

    OP_DO_A_R       (::op_FD_80, DO_ADD_8, REG_B, DO_PREF_00)   // *ADD A,B
    OP_DO_A_R       (::op_FD_81, DO_ADD_8, REG_C, DO_PREF_00)   // *ADD A,C
    OP_DO_A_R       (::op_FD_82, DO_ADD_8, REG_D, DO_PREF_00)   // *ADD A,D
    OP_DO_A_R       (::op_FD_83, DO_ADD_8, REG_E, DO_PREF_00)   // *ADD A,E
    OP_DO_A_R       (::op_FD_84, DO_ADD_8, REG_IYH, DO_PREF_00) // ADD A,IYH
    OP_DO_A_R       (::op_FD_85, DO_ADD_8, REG_IYL, DO_PREF_00) // ADD A,IYL
    OP_DO_A_ORP     (::op_FD_86, DO_ADD_8, REG_IY)              // ADD A,(IY+N)
    OP_DO_A_R       (::op_FD_87, DO_ADD_8, REG_A, DO_PREF_00)   // *ADD A,A
    OP_DO_A_R       (::op_FD_88, DO_ADC_8, REG_B, DO_PREF_00)   // *ADC A,B
    OP_DO_A_R       (::op_FD_89, DO_ADC_8, REG_C, DO_PREF_00)   // *ADC A,C
    OP_DO_A_R       (::op_FD_8A, DO_ADC_8, REG_D, DO_PREF_00)   // *ADC A,D
    OP_DO_A_R       (::op_FD_8B, DO_ADC_8, REG_E, DO_PREF_00)   // *ADC A,E
    OP_DO_A_R       (::op_FD_8C, DO_ADC_8, REG_IYH, DO_PREF_00) // ADC A,IYH
    OP_DO_A_R       (::op_FD_8D, DO_ADC_8, REG_IYL, DO_PREF_00) // ADC A,IYL
    OP_DO_A_ORP     (::op_FD_8E, DO_ADC_8, REG_IY)              // ADC A,(IY+N)
    OP_DO_A_R       (::op_FD_8F, DO_ADC_8, REG_A, DO_PREF_00)   // *ADC A,A

    OP_DO_A_R       (::op_FD_90, DO_SUB_8, REG_B, DO_PREF_00)   // *SUB B
    OP_DO_A_R       (::op_FD_91, DO_SUB_8, REG_C, DO_PREF_00)   // *SUB C
    OP_DO_A_R       (::op_FD_92, DO_SUB_8, REG_D, DO_PREF_00)   // *SUB D
    OP_DO_A_R       (::op_FD_93, DO_SUB_8, REG_E, DO_PREF_00)   // *SUB E
    OP_DO_A_R       (::op_FD_94, DO_SUB_8, REG_IYH, DO_PREF_00) // SUB IYH
    OP_DO_A_R       (::op_FD_95, DO_SUB_8, REG_IYL, DO_PREF_00) // SUB IYL
    OP_DO_A_ORP     (::op_FD_96, DO_SUB_8, REG_IY)              // SUB (IY+N)
    OP_DO_A_R       (::op_FD_97, DO_SUB_8, REG_A, DO_PREF_00)   // *SUB A
    OP_DO_A_R       (::op_FD_98, DO_SBC_8, REG_B, DO_PREF_00)   // *SBC A,B
    OP_DO_A_R       (::op_FD_99, DO_SBC_8, REG_C, DO_PREF_00)   // *SBC A,C
    OP_DO_A_R       (::op_FD_9A, DO_SBC_8, REG_D, DO_PREF_00)   // *SBC A,D
    OP_DO_A_R       (::op_FD_9B, DO_SBC_8, REG_E, DO_PREF_00)   // *SBC A,E
    OP_DO_A_R       (::op_FD_9C, DO_SBC_8, REG_IYH, DO_PREF_00) // SBC A,IYH
    OP_DO_A_R       (::op_FD_9D, DO_SBC_8, REG_IYL, DO_PREF_00) // SBC A,IYL
    OP_DO_A_ORP     (::op_FD_9E, DO_SBC_8, REG_IY)              // SBC A,(IY+N)
    OP_DO_A_R       (::op_FD_9F, DO_SBC_8, REG_A, DO_PREF_00)   // *SBC A,A

    OP_DO_A_R       (::op_FD_A0, DO_AND_8, REG_B, DO_PREF_00)   // *AND B
    OP_DO_A_R       (::op_FD_A1, DO_AND_8, REG_C, DO_PREF_00)   // *AND C
    OP_DO_A_R       (::op_FD_A2, DO_AND_8, REG_D, DO_PREF_00)   // *AND D
    OP_DO_A_R       (::op_FD_A3, DO_AND_8, REG_E, DO_PREF_00)   // *AND E
    OP_DO_A_R       (::op_FD_A4, DO_AND_8, REG_IYH, DO_PREF_00) // AND IYH
    OP_DO_A_R       (::op_FD_A5, DO_AND_8, REG_IYL, DO_PREF_00) // AND IYL
    OP_DO_A_ORP     (::op_FD_A6, DO_AND_8, REG_IY)              // AND (IY+N)
    OP_DO_A_R       (::op_FD_A7, DO_AND_8, REG_A, DO_PREF_00)   // *AND A
    OP_DO_A_R       (::op_FD_A8, DO_XOR_8, REG_B, DO_PREF_00)   // *XOR B
    OP_DO_A_R       (::op_FD_A9, DO_XOR_8, REG_C, DO_PREF_00)   // *XOR C
    OP_DO_A_R       (::op_FD_AA, DO_XOR_8, REG_D, DO_PREF_00)   // *XOR D
    OP_DO_A_R       (::op_FD_AB, DO_XOR_8, REG_E, DO_PREF_00)   // *XOR E
    OP_DO_A_R       (::op_FD_AC, DO_XOR_8, REG_IYH, DO_PREF_00) // XOR IYH
    OP_DO_A_R       (::op_FD_AD, DO_XOR_8, REG_IYL, DO_PREF_00) // XOR IYL
    OP_DO_A_ORP     (::op_FD_AE, DO_XOR_8, REG_IY)              // XOR (IY+N)
    OP_DO_A_R       (::op_FD_AF, DO_XOR_8, REG_A, DO_PREF_00)   // *XOR A

    OP_DO_A_R       (::op_FD_B0, DO_OR_8, REG_B, DO_PREF_00)    // *OR B
    OP_DO_A_R       (::op_FD_B1, DO_OR_8, REG_C, DO_PREF_00)    // *OR C
    OP_DO_A_R       (::op_FD_B2, DO_OR_8, REG_D, DO_PREF_00)    // *OR D
    OP_DO_A_R       (::op_FD_B3, DO_OR_8, REG_E, DO_PREF_00)    // *OR E
    OP_DO_A_R       (::op_FD_B4, DO_OR_8, REG_IYH, DO_PREF_00)  // OR IYH
    OP_DO_A_R       (::op_FD_B5, DO_OR_8, REG_IYL, DO_PREF_00)  // OR IYL
    OP_DO_A_ORP     (::op_FD_B6, DO_OR_8, REG_IY)               // OR (IY+N)
    OP_DO_A_R       (::op_FD_B7, DO_OR_8, REG_A, DO_PREF_00)    // *OR A
    OP_DO_A_R       (::op_FD_B8, DO_CP_8, REG_B, DO_PREF_00)    // *CP B
    OP_DO_A_R       (::op_FD_B9, DO_CP_8, REG_C, DO_PREF_00)    // *CP C
    OP_DO_A_R       (::op_FD_BA, DO_CP_8, REG_D, DO_PREF_00)    // *CP D
    OP_DO_A_R       (::op_FD_BB, DO_CP_8, REG_E, DO_PREF_00)    // *CP E
    OP_DO_A_R       (::op_FD_BC, DO_CP_8, REG_IYH, DO_PREF_00)  // CP IYH
    OP_DO_A_R       (::op_FD_BD, DO_CP_8, REG_IYL, DO_PREF_00)  // CP IYL
    OP_DO_A_ORP     (::op_FD_BE, DO_CP_8, REG_IY)               // CP (IY+N)
    OP_DO_A_R       (::op_FD_BF, DO_CP_8, REG_A, DO_PREF_00)    // *CP A

    OP_RET_CC       (::op_FD_C0, CC_NZ, DO_PREF_00)             // *RET NZ
    OP_POP_RP       (::op_FD_C1, REG_BC, DO_PREF_00)            // *POP BC
    OP_JP_CC        (::op_FD_C2, CC_NZ, DO_PREF_00)             // *JP NZ,NN
    OP_JP           (::op_FD_C3, DO_PREF_00)                    // *JP NN
    OP_CALL_CC      (::op_FD_C4, CC_NZ, DO_PREF_00)             // *CALL NZ,NN
    OP_PUSH_RP      (::op_FD_C5, REG_BC, DO_PREF_00)            // *PUSH BC
    OP_DO_A_N       (::op_FD_C6, DO_ADD_8, DO_PREF_00)          // *ADD A,N
    OP_RST          (::op_FD_C7, 0x00, DO_PREF_00)              // *RST #00
    OP_RET_CC       (::op_FD_C8, CC_Z, DO_PREF_00)              // *RET Z
    OP_RET          (::op_FD_C9, DO_PREF_00)                    // *RET
    OP_JP_CC        (::op_FD_CA, CC_Z, DO_PREF_00)              // *JP Z,NN
    OP_PREF_XX_CB   (::op_FD_CB, optable_FD_CB)                 // prefix #FD_CB
    OP_CALL_CC      (::op_FD_CC, CC_Z, DO_PREF_00)              // *CALL Z,NN
    OP_CALL         (::op_FD_CD, DO_PREF_00)                    // *CALL NN
    OP_DO_A_N       (::op_FD_CE, DO_ADC_8, DO_PREF_00)          // *ADC A,N
    OP_RST          (::op_FD_CF, 0x08, DO_PREF_00)              // *RST #08

    OP_RET_CC       (::op_FD_D0, CC_NC, DO_PREF_00)             // *RET NC
    OP_POP_RP       (::op_FD_D1, REG_DE, DO_PREF_00)            // *POP DE
    OP_JP_CC        (::op_FD_D2, CC_NC, DO_PREF_00)             // *JP NC,NN
    OP_OUT_N_A      (::op_FD_D3, DO_PREF_00)                    // *OUT (N),A
    OP_CALL_CC      (::op_FD_D4, CC_NC, DO_PREF_00)             // *CALL NC,NN
    OP_PUSH_RP      (::op_FD_D5, REG_DE, DO_PREF_00)            // *PUSH DE
    OP_DO_A_N       (::op_FD_D6, DO_SUB_8, DO_PREF_00)          // *SUB N
    OP_RST          (::op_FD_D7, 0x10, DO_PREF_00)              // *RST #10
    OP_RET_CC       (::op_FD_D8, CC_C, DO_PREF_00)              // *RET C
    OP_EXX          (::op_FD_D9, DO_PREF_00)                    // *EXX
    OP_JP_CC        (::op_FD_DA, CC_C, DO_PREF_00)              // *JP C,NN
    OP_IN_A_N       (::op_FD_DB, DO_PREF_00)                    // *IN A,(N)
    OP_CALL_CC      (::op_FD_DC, CC_C, DO_PREF_00)              // *CALL C,NN
    OP_PREF_DD      (::op_FD_DD)                                // prefix #DD
    OP_DO_A_N       (::op_FD_DE, DO_SBC_8, DO_PREF_00)          // *SBC A,N
    OP_RST          (::op_FD_DF, 0x18, DO_PREF_00)              // *RST #18

    OP_RET_CC       (::op_FD_E0, CC_PO, DO_PREF_00)                     // *RET PO
    OP_POP_RP       (::op_FD_E1, REG_IY, DO_PREF_00)                    // POP IY
    OP_JP_CC        (::op_FD_E2, CC_PO, DO_PREF_00)                     // *JP PO,NN
    OP_EX_SP_RP     (::op_FD_E3, REG_IYL, REG_IYH, REG_IY, DO_PREF_00)  // EX (SP),IY
    OP_CALL_CC      (::op_FD_E4, CC_PO, DO_PREF_00)                     // *CALL PO,NN
    OP_PUSH_RP      (::op_FD_E5, REG_IY, DO_PREF_00)                    // PUSH IY
    OP_DO_A_N       (::op_FD_E6, DO_AND_8, DO_PREF_00)                  // *AND N
    OP_RST          (::op_FD_E7, 0x20, DO_PREF_00)                      // *RST #20
    OP_RET_CC       (::op_FD_E8, CC_PE, DO_PREF_00)                     // *RET PE
    OP_JP_RP        (::op_FD_E9, REG_IY, DO_PREF_00)                    // JP IY
    OP_JP_CC        (::op_FD_EA, CC_PE, DO_PREF_00)                     // *JP PE,NN
    OP_EX_DE_HL     (::op_FD_EB, DO_PREF_00)                            // *EX DE,HL
    OP_CALL_CC      (::op_FD_EC, CC_PE, DO_PREF_00)                     // *CALL PE,NN
    OP_PREF_ED      (::op_FD_ED)                                        // prefix #ED
    OP_DO_A_N       (::op_FD_EE, DO_XOR_8, DO_PREF_00)                  // *XOR N
    OP_RST          (::op_FD_EF, 0x28, DO_PREF_00)                      // *RST #28

    OP_RET_CC       (::op_FD_F0, CC_P, DO_PREF_00)              // *RET P
    OP_POP_RP       (::op_FD_F1, REG_AF, DO_PREF_00)            // *POP AF
    OP_JP_CC        (::op_FD_F2, CC_P, DO_PREF_00)              // *JP P,NN
    OP_DI           (::op_FD_F3, DO_PREF_00)                    // *DI
    OP_CALL_CC      (::op_FD_F4, CC_P, DO_PREF_00)              // *CALL P,NN
    OP_PUSH_RP      (::op_FD_F5, REG_AF, DO_PREF_00)            // *PUSH AF
    OP_DO_A_N       (::op_FD_F6, DO_OR_8, DO_PREF_00)           // *OR N
    OP_RST          (::op_FD_F7, 0x30, DO_PREF_00)              // *RST #30
    OP_RET_CC       (::op_FD_F8, CC_M, DO_PREF_00)              // *RET M
    OP_LD_SP_RP     (::op_FD_F9, REG_IY, DO_PREF_00)            // LD SP,IY
    OP_JP_CC        (::op_FD_FA, CC_M, DO_PREF_00)              // *JP M,NN
    OP_EI           (::op_FD_FB, DO_PREF_00)                    // *EI
    OP_CALL_CC      (::op_FD_FC, CC_M, DO_PREF_00)              // *CALL M,NN
    OP_PREF_FD      (::op_FD_FD)                                // prefix #FD
    OP_DO_A_N       (::op_FD_FE, DO_CP_8, DO_PREF_00)           // *CP N
    OP_RST          (::op_FD_FF, 0x38, DO_PREF_00)              // *RST #38

    ::t_opcode optable_FD[0x100] = {
        ::op_FD_00, ::op_FD_01, ::op_FD_02, ::op_FD_03, ::op_FD_04, ::op_FD_05, ::op_FD_06, ::op_FD_07,
        ::op_FD_08, ::op_FD_09, ::op_FD_0A, ::op_FD_0B, ::op_FD_0C, ::op_FD_0D, ::op_FD_0E, ::op_FD_0F,
        ::op_FD_10, ::op_FD_11, ::op_FD_12, ::op_FD_13, ::op_FD_14, ::op_FD_15, ::op_FD_16, ::op_FD_17,
        ::op_FD_18, ::op_FD_19, ::op_FD_1A, ::op_FD_1B, ::op_FD_1C, ::op_FD_1D, ::op_FD_1E, ::op_FD_1F,
        ::op_FD_20, ::op_FD_21, ::op_FD_22, ::op_FD_23, ::op_FD_24, ::op_FD_25, ::op_FD_26, ::op_FD_27,
        ::op_FD_28, ::op_FD_29, ::op_FD_2A, ::op_FD_2B, ::op_FD_2C, ::op_FD_2D, ::op_FD_2E, ::op_FD_2F,
        ::op_FD_30, ::op_FD_31, ::op_FD_32, ::op_FD_33, ::op_FD_34, ::op_FD_35, ::op_FD_36, ::op_FD_37,
        ::op_FD_38, ::op_FD_39, ::op_FD_3A, ::op_FD_3B, ::op_FD_3C, ::op_FD_3D, ::op_FD_3E, ::op_FD_3F,
        ::op_FD_40, ::op_FD_41, ::op_FD_42, ::op_FD_43, ::op_FD_44, ::op_FD_45, ::op_FD_46, ::op_FD_47,
        ::op_FD_48, ::op_FD_49, ::op_FD_4A, ::op_FD_4B, ::op_FD_4C, ::op_FD_4D, ::op_FD_4E, ::op_FD_4F,
        ::op_FD_50, ::op_FD_51, ::op_FD_52, ::op_FD_53, ::op_FD_54, ::op_FD_55, ::op_FD_56, ::op_FD_57,
        ::op_FD_58, ::op_FD_59, ::op_FD_5A, ::op_FD_5B, ::op_FD_5C, ::op_FD_5D, ::op_FD_5E, ::op_FD_5F,
        ::op_FD_60, ::op_FD_61, ::op_FD_62, ::op_FD_63, ::op_FD_64, ::op_FD_65, ::op_FD_66, ::op_FD_67,
        ::op_FD_68, ::op_FD_69, ::op_FD_6A, ::op_FD_6B, ::op_FD_6C, ::op_FD_6D, ::op_FD_6E, ::op_FD_6F,
        ::op_FD_70, ::op_FD_71, ::op_FD_72, ::op_FD_73, ::op_FD_74, ::op_FD_75, ::op_FD_76, ::op_FD_77,
        ::op_FD_78, ::op_FD_79, ::op_FD_7A, ::op_FD_7B, ::op_FD_7C, ::op_FD_7D, ::op_FD_7E, ::op_FD_7F,
        ::op_FD_80, ::op_FD_81, ::op_FD_82, ::op_FD_83, ::op_FD_84, ::op_FD_85, ::op_FD_86, ::op_FD_87,
        ::op_FD_88, ::op_FD_89, ::op_FD_8A, ::op_FD_8B, ::op_FD_8C, ::op_FD_8D, ::op_FD_8E, ::op_FD_8F,
        ::op_FD_90, ::op_FD_91, ::op_FD_92, ::op_FD_93, ::op_FD_94, ::op_FD_95, ::op_FD_96, ::op_FD_97,
        ::op_FD_98, ::op_FD_99, ::op_FD_9A, ::op_FD_9B, ::op_FD_9C, ::op_FD_9D, ::op_FD_9E, ::op_FD_9F,
        ::op_FD_A0, ::op_FD_A1, ::op_FD_A2, ::op_FD_A3, ::op_FD_A4, ::op_FD_A5, ::op_FD_A6, ::op_FD_A7,
        ::op_FD_A8, ::op_FD_A9, ::op_FD_AA, ::op_FD_AB, ::op_FD_AC, ::op_FD_AD, ::op_FD_AE, ::op_FD_AF,
        ::op_FD_B0, ::op_FD_B1, ::op_FD_B2, ::op_FD_B3, ::op_FD_B4, ::op_FD_B5, ::op_FD_B6, ::op_FD_B7,
        ::op_FD_B8, ::op_FD_B9, ::op_FD_BA, ::op_FD_BB, ::op_FD_BC, ::op_FD_BD, ::op_FD_BE, ::op_FD_BF,
        ::op_FD_C0, ::op_FD_C1, ::op_FD_C2, ::op_FD_C3, ::op_FD_C4, ::op_FD_C5, ::op_FD_C6, ::op_FD_C7,
        ::op_FD_C8, ::op_FD_C9, ::op_FD_CA, ::op_FD_CB, ::op_FD_CC, ::op_FD_CD, ::op_FD_CE, ::op_FD_CF,
        ::op_FD_D0, ::op_FD_D1, ::op_FD_D2, ::op_FD_D3, ::op_FD_D4, ::op_FD_D5, ::op_FD_D6, ::op_FD_D7,
        ::op_FD_D8, ::op_FD_D9, ::op_FD_DA, ::op_FD_DB, ::op_FD_DC, ::op_FD_DD, ::op_FD_DE, ::op_FD_DF,
        ::op_FD_E0, ::op_FD_E1, ::op_FD_E2, ::op_FD_E3, ::op_FD_E4, ::op_FD_E5, ::op_FD_E6, ::op_FD_E7,
        ::op_FD_E8, ::op_FD_E9, ::op_FD_EA, ::op_FD_EB, ::op_FD_EC, ::op_FD_ED, ::op_FD_EE, ::op_FD_EF,
        ::op_FD_F0, ::op_FD_F1, ::op_FD_F2, ::op_FD_F3, ::op_FD_F4, ::op_FD_F5, ::op_FD_F6, ::op_FD_F7,
        ::op_FD_F8, ::op_FD_F9, ::op_FD_FA, ::op_FD_FB, ::op_FD_FC, ::op_FD_FD, ::op_FD_FE, ::op_FD_FF
    };
#end
