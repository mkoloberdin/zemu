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
    OP_NOP          (::op_DD_00, DO_PREF_00)                    // *NOP
    OP_LD_RP_NN     (::op_DD_01, REG_BC, DO_PREF_00)            // *LD BC,NN
    OP_LD_MRP_A     (::op_DD_02, REG_BC, DO_PREF_00)            // *LD (BC),A
    OP_DO_RP        (::op_DD_03, DO_INC_16, REG_BC, DO_PREF_00) // *INC BC
    OP_DO_R         (::op_DD_04, DO_INC_8, REG_B, DO_PREF_00)   // *INC B
    OP_DO_R         (::op_DD_05, DO_DEC_8, REG_B, DO_PREF_00)   // *DEC B
    OP_LD_R_N       (::op_DD_06, REG_B, DO_PREF_00)             // *LD B,N
    OP_RLCA         (::op_DD_07, DO_PREF_00)                    // *RLCA
    OP_EX_AF_AF_    (::op_DD_08, DO_PREF_00)                    // *EX AF,AF'
    OP_ADD_RP_RP    (::op_DD_09, REG_IX, REG_BC, DO_PREF_00)    // ADD IX,BC
    OP_LD_A_MRP     (::op_DD_0A, REG_BC, DO_PREF_00)            // *LD A,(BC)
    OP_DO_RP        (::op_DD_0B, DO_DEC_16, REG_BC, DO_PREF_00) // *DEC BC
    OP_DO_R         (::op_DD_0C, DO_INC_8, REG_C, DO_PREF_00)   // *INC C
    OP_DO_R         (::op_DD_0D, DO_DEC_8, REG_C, DO_PREF_00)   // *DEC C
    OP_LD_R_N       (::op_DD_0E, REG_C, DO_PREF_00)             // *LD C,N
    OP_RRCA         (::op_DD_0F, DO_PREF_00)                    // *RRCA

    OP_DJNZ         (::op_DD_10, DO_PREF_00)                    // *DJNZ O
    OP_LD_RP_NN     (::op_DD_11, REG_DE, DO_PREF_00)            // *LD DE,NN
    OP_LD_MRP_A     (::op_DD_12, REG_DE, DO_PREF_00)            // *LD (DE),A
    OP_DO_RP        (::op_DD_13, DO_INC_16, REG_DE, DO_PREF_00) // *INC DE
    OP_DO_R         (::op_DD_14, DO_INC_8, REG_D, DO_PREF_00)   // *INC D
    OP_DO_R         (::op_DD_15, DO_DEC_8, REG_D, DO_PREF_00)   // *DEC D
    OP_LD_R_N       (::op_DD_16, REG_D, DO_PREF_00)             // *LD D,N
    OP_RLA          (::op_DD_17, DO_PREF_00)                    // *RLA
    OP_JR           (::op_DD_18, DO_PREF_00)                    // *JR O
    OP_ADD_RP_RP    (::op_DD_19, REG_IX, REG_DE, DO_PREF_00)    // ADD IX,DE
    OP_LD_A_MRP     (::op_DD_1A, REG_DE, DO_PREF_00)            // *LD A,(DE)
    OP_DO_RP        (::op_DD_1B, DO_DEC_16, REG_DE, DO_PREF_00) // *DEC DE
    OP_DO_R         (::op_DD_1C, DO_INC_8, REG_E, DO_PREF_00)   // *INC E
    OP_DO_R         (::op_DD_1D, DO_DEC_8, REG_E, DO_PREF_00)   // *DEC E
    OP_LD_R_N       (::op_DD_1E, REG_E, DO_PREF_00)             // *LD E,N
    OP_RRA          (::op_DD_1F, DO_PREF_00)                    // *RRA

    OP_JR_CC        (::op_DD_20, CC_NZ, DO_PREF_00)             // *JR NZ,O
    OP_LD_RP_NN     (::op_DD_21, REG_IX, DO_PREF_00)            // LD IX,NN
    OP_LD_MNN_RP    (::op_DD_22, REG_IXL, REG_IXH, DO_PREF_00)  // LD (NN),IX
    OP_DO_RP        (::op_DD_23, DO_INC_16, REG_IX, DO_PREF_00) // INC IX
    OP_DO_R         (::op_DD_24, DO_INC_8, REG_IXH, DO_PREF_00) // INC IXH
    OP_DO_R         (::op_DD_25, DO_DEC_8, REG_IXH, DO_PREF_00) // DEC IXH
    OP_LD_R_N       (::op_DD_26, REG_IXH, DO_PREF_00)           // LD IXH,N
    OP_DAA          (::op_DD_27, DO_PREF_00)                    // *DAA
    OP_JR_CC        (::op_DD_28, CC_Z, DO_PREF_00)              // *JR Z,O
    OP_ADD_RP_RP    (::op_DD_29, REG_IX, REG_IX, DO_PREF_00)    // ADD IX,IX
    OP_LD_RP_MNN    (::op_DD_2A, REG_IXL, REG_IXH, DO_PREF_00)  // LD IX,(NN)
    OP_DO_RP        (::op_DD_2B, DO_DEC_16, REG_IX, DO_PREF_00) // DEC IX
    OP_DO_R         (::op_DD_2C, DO_INC_8, REG_IXL, DO_PREF_00) // INC IXL
    OP_DO_R         (::op_DD_2D, DO_DEC_8, REG_IXL, DO_PREF_00) // DEC IXL
    OP_LD_R_N       (::op_DD_2E, REG_IXL, DO_PREF_00)           // LD IXL,N
    OP_CPL          (::op_DD_2F, DO_PREF_00)                    // *CPL

    OP_JR_CC        (::op_DD_30, CC_NC, DO_PREF_00)             // *JR NC,O
    OP_LD_RP_NN     (::op_DD_31, REG_SP, DO_PREF_00)            // *LD SP,NN
    OP_LD_MNN_A     (::op_DD_32, DO_PREF_00)                    // *LD (NN),A
    OP_DO_RP        (::op_DD_33, DO_INC_16, REG_SP, DO_PREF_00) // *INC SP
    OP_DO_ORP       (::op_DD_34, DO_INC_8, REG_IX)              // INC (IX+N)
    OP_DO_ORP       (::op_DD_35, DO_DEC_8, REG_IX)              // DEC (IX+N)
    OP_LD_ORP_N     (::op_DD_36, REG_IX)                        // LD (IX+N),N
    OP_SCF          (::op_DD_37, DO_PREF_00)                    // *SCF
    OP_JR_CC        (::op_DD_38, CC_C, DO_PREF_00)              // *JR C,O
    OP_ADD_RP_RP    (::op_DD_39, REG_IX, REG_SP, DO_PREF_00)    // ADD IX,SP
    OP_LD_A_MNN     (::op_DD_3A, DO_PREF_00)                    // *LD A,(NN)
    OP_DO_RP        (::op_DD_3B, DO_DEC_16, REG_SP, DO_PREF_00) // *DEC SP
    OP_DO_R         (::op_DD_3C, DO_INC_8, REG_A, DO_PREF_00)   // *INC A
    OP_DO_R         (::op_DD_3D, DO_DEC_8, REG_A, DO_PREF_00)   // *DEC A
    OP_LD_R_N       (::op_DD_3E, REG_A, DO_PREF_00)             // *LD A,N
    OP_CCF          (::op_DD_3F, DO_PREF_00)                    // *CCF

    OP_LD_R_R       (::op_DD_40, REG_B, REG_B, DO_PREF_00)      // *LD B,B
    OP_LD_R_R       (::op_DD_41, REG_B, REG_C, DO_PREF_00)      // *LD B,C
    OP_LD_R_R       (::op_DD_42, REG_B, REG_D, DO_PREF_00)      // *LD B,D
    OP_LD_R_R       (::op_DD_43, REG_B, REG_E, DO_PREF_00)      // *LD B,E
    OP_LD_R_R       (::op_DD_44, REG_B, REG_IXH, DO_PREF_00)    // LD B,IXH
    OP_LD_R_R       (::op_DD_45, REG_B, REG_IXL, DO_PREF_00)    // LD B,IXL
    OP_LD_R_ORP     (::op_DD_46, REG_B, REG_IX)                 // LD B,(IX+N)
    OP_LD_R_R       (::op_DD_47, REG_B, REG_A, DO_PREF_00)      // *LD B,A
    OP_LD_R_R       (::op_DD_48, REG_C, REG_B, DO_PREF_00)      // *LD C,B
    OP_LD_R_R       (::op_DD_49, REG_C, REG_C, DO_PREF_00)      // *LD C,C
    OP_LD_R_R       (::op_DD_4A, REG_C, REG_D, DO_PREF_00)      // *LD C,D
    OP_LD_R_R       (::op_DD_4B, REG_C, REG_E, DO_PREF_00)      // *LD C,E
    OP_LD_R_R       (::op_DD_4C, REG_C, REG_IXH, DO_PREF_00)    // LD C,IXH
    OP_LD_R_R       (::op_DD_4D, REG_C, REG_IXL, DO_PREF_00)    // LD C,IXL
    OP_LD_R_ORP     (::op_DD_4E, REG_C, REG_IX)                 // LD C,(IX+N)
    OP_LD_R_R       (::op_DD_4F, REG_C, REG_A, DO_PREF_00)      // *LD C,A

    OP_LD_R_R       (::op_DD_50, REG_D, REG_B, DO_PREF_00)      // *LD D,B
    OP_LD_R_R       (::op_DD_51, REG_D, REG_C, DO_PREF_00)      // *LD D,C
    OP_LD_R_R       (::op_DD_52, REG_D, REG_D, DO_PREF_00)      // *LD D,D
    OP_LD_R_R       (::op_DD_53, REG_D, REG_E, DO_PREF_00)      // *LD D,E
    OP_LD_R_R       (::op_DD_54, REG_D, REG_IXH, DO_PREF_00)    // LD D,IXH
    OP_LD_R_R       (::op_DD_55, REG_D, REG_IXL, DO_PREF_00)    // LD D,IXL
    OP_LD_R_ORP     (::op_DD_56, REG_D, REG_IX)                 // LD D,(IX+N)
    OP_LD_R_R       (::op_DD_57, REG_D, REG_A, DO_PREF_00)      // *LD D,A
    OP_LD_R_R       (::op_DD_58, REG_E, REG_B, DO_PREF_00)      // *LD E,B
    OP_LD_R_R       (::op_DD_59, REG_E, REG_C, DO_PREF_00)      // *LD E,C
    OP_LD_R_R       (::op_DD_5A, REG_E, REG_D, DO_PREF_00)      // *LD E,D
    OP_LD_R_R       (::op_DD_5B, REG_E, REG_E, DO_PREF_00)      // *LD E,E
    OP_LD_R_R       (::op_DD_5C, REG_E, REG_IXH, DO_PREF_00)    // LD E,IXH
    OP_LD_R_R       (::op_DD_5D, REG_E, REG_IXL, DO_PREF_00)    // LD E,IXL
    OP_LD_R_ORP     (::op_DD_5E, REG_E, REG_IX)                 // LD E,(IX+N)
    OP_LD_R_R       (::op_DD_5F, REG_E, REG_A, DO_PREF_00)      // *LD E,A

    OP_LD_R_R       (::op_DD_60, REG_IXH, REG_B, DO_PREF_00)    // LD IXH,B
    OP_LD_R_R       (::op_DD_61, REG_IXH, REG_C, DO_PREF_00)    // LD IXH,C
    OP_LD_R_R       (::op_DD_62, REG_IXH, REG_D, DO_PREF_00)    // LD IXH,D
    OP_LD_R_R       (::op_DD_63, REG_IXH, REG_E, DO_PREF_00)    // LD IXH,E
    OP_LD_R_R       (::op_DD_64, REG_IXH, REG_IXH, DO_PREF_00)  // LD IXH,H
    OP_LD_R_R       (::op_DD_65, REG_IXH, REG_IXL, DO_PREF_00)  // LD IXH,L
    OP_LD_R_ORP     (::op_DD_66, REG_H, REG_IX)                 // LD H,(IX+N)
    OP_LD_R_R       (::op_DD_67, REG_IXH, REG_A, DO_PREF_00)    // LD IXH,A
    OP_LD_R_R       (::op_DD_68, REG_IXL, REG_B, DO_PREF_00)    // LD IXL,B
    OP_LD_R_R       (::op_DD_69, REG_IXL, REG_C, DO_PREF_00)    // LD IXL,C
    OP_LD_R_R       (::op_DD_6A, REG_IXL, REG_D, DO_PREF_00)    // LD IXL,D
    OP_LD_R_R       (::op_DD_6B, REG_IXL, REG_E, DO_PREF_00)    // LD IXL,E
    OP_LD_R_R       (::op_DD_6C, REG_IXL, REG_IXH, DO_PREF_00)  // LD IXL,H
    OP_LD_R_R       (::op_DD_6D, REG_IXL, REG_IXL, DO_PREF_00)  // LD IXL,L
    OP_LD_R_ORP     (::op_DD_6E, REG_L, REG_IX)                 // LD L,(IX+N)
    OP_LD_R_R       (::op_DD_6F, REG_IXL, REG_A, DO_PREF_00)    // LD IXL,A

    OP_LD_ORP_R     (::op_DD_70, REG_IX, REG_B)                 // LD (IX+N),B
    OP_LD_ORP_R     (::op_DD_71, REG_IX, REG_C)                 // LD (IX+N),C
    OP_LD_ORP_R     (::op_DD_72, REG_IX, REG_D)                 // LD (IX+N),D
    OP_LD_ORP_R     (::op_DD_73, REG_IX, REG_E)                 // LD (IX+N),E
    OP_LD_ORP_R     (::op_DD_74, REG_IX, REG_H)                 // LD (IX+N),H
    OP_LD_ORP_R     (::op_DD_75, REG_IX, REG_L)                 // LD (IX+N),L
    OP_HALT         (::op_DD_76, DO_PREF_00)                    // *HALT
    OP_LD_ORP_R     (::op_DD_77, REG_IX, REG_A)                 // LD (IX+N),A
    OP_LD_R_R       (::op_DD_78, REG_A, REG_B, DO_PREF_00)      // *LD A,B
    OP_LD_R_R       (::op_DD_79, REG_A, REG_C, DO_PREF_00)      // *LD A,C
    OP_LD_R_R       (::op_DD_7A, REG_A, REG_D, DO_PREF_00)      // *LD A,D
    OP_LD_R_R       (::op_DD_7B, REG_A, REG_E, DO_PREF_00)      // *LD A,E
    OP_LD_R_R       (::op_DD_7C, REG_A, REG_IXH, DO_PREF_00)    // LD A,IXH
    OP_LD_R_R       (::op_DD_7D, REG_A, REG_IXL, DO_PREF_00)    // LD A,IXL
    OP_LD_R_ORP     (::op_DD_7E, REG_A, REG_IX)                 // LD A,(IX+N)
    OP_LD_R_R       (::op_DD_7F, REG_A, REG_A, DO_PREF_00)      // *LD A,A

    OP_DO_A_R       (::op_DD_80, DO_ADD_8, REG_B, DO_PREF_00)   // *ADD A,B
    OP_DO_A_R       (::op_DD_81, DO_ADD_8, REG_C, DO_PREF_00)   // *ADD A,C
    OP_DO_A_R       (::op_DD_82, DO_ADD_8, REG_D, DO_PREF_00)   // *ADD A,D
    OP_DO_A_R       (::op_DD_83, DO_ADD_8, REG_E, DO_PREF_00)   // *ADD A,E
    OP_DO_A_R       (::op_DD_84, DO_ADD_8, REG_IXH, DO_PREF_00) // ADD A,IXH
    OP_DO_A_R       (::op_DD_85, DO_ADD_8, REG_IXL, DO_PREF_00) // ADD A,IXL
    OP_DO_A_ORP     (::op_DD_86, DO_ADD_8, REG_IX)              // ADD A,(IX+N)
    OP_DO_A_R       (::op_DD_87, DO_ADD_8, REG_A, DO_PREF_00)   // *ADD A,A
    OP_DO_A_R       (::op_DD_88, DO_ADC_8, REG_B, DO_PREF_00)   // *ADC A,B
    OP_DO_A_R       (::op_DD_89, DO_ADC_8, REG_C, DO_PREF_00)   // *ADC A,C
    OP_DO_A_R       (::op_DD_8A, DO_ADC_8, REG_D, DO_PREF_00)   // *ADC A,D
    OP_DO_A_R       (::op_DD_8B, DO_ADC_8, REG_E, DO_PREF_00)   // *ADC A,E
    OP_DO_A_R       (::op_DD_8C, DO_ADC_8, REG_IXH, DO_PREF_00) // ADC A,IXH
    OP_DO_A_R       (::op_DD_8D, DO_ADC_8, REG_IXL, DO_PREF_00) // ADC A,IXL
    OP_DO_A_ORP     (::op_DD_8E, DO_ADC_8, REG_IX)              // ADC A,(IX+N)
    OP_DO_A_R       (::op_DD_8F, DO_ADC_8, REG_A, DO_PREF_00)   // *ADC A,A

    OP_DO_A_R       (::op_DD_90, DO_SUB_8, REG_B, DO_PREF_00)   // *SUB B
    OP_DO_A_R       (::op_DD_91, DO_SUB_8, REG_C, DO_PREF_00)   // *SUB C
    OP_DO_A_R       (::op_DD_92, DO_SUB_8, REG_D, DO_PREF_00)   // *SUB D
    OP_DO_A_R       (::op_DD_93, DO_SUB_8, REG_E, DO_PREF_00)   // *SUB E
    OP_DO_A_R       (::op_DD_94, DO_SUB_8, REG_IXH, DO_PREF_00) // SUB IXH
    OP_DO_A_R       (::op_DD_95, DO_SUB_8, REG_IXL, DO_PREF_00) // SUB IXL
    OP_DO_A_ORP     (::op_DD_96, DO_SUB_8, REG_IX)              // SUB (IX+N)
    OP_DO_A_R       (::op_DD_97, DO_SUB_8, REG_A, DO_PREF_00)   // *SUB A
    OP_DO_A_R       (::op_DD_98, DO_SBC_8, REG_B, DO_PREF_00)   // *SBC A,B
    OP_DO_A_R       (::op_DD_99, DO_SBC_8, REG_C, DO_PREF_00)   // *SBC A,C
    OP_DO_A_R       (::op_DD_9A, DO_SBC_8, REG_D, DO_PREF_00)   // *SBC A,D
    OP_DO_A_R       (::op_DD_9B, DO_SBC_8, REG_E, DO_PREF_00)   // *SBC A,E
    OP_DO_A_R       (::op_DD_9C, DO_SBC_8, REG_IXH, DO_PREF_00) // SBC A,IXH
    OP_DO_A_R       (::op_DD_9D, DO_SBC_8, REG_IXL, DO_PREF_00) // SBC A,IXL
    OP_DO_A_ORP     (::op_DD_9E, DO_SBC_8, REG_IX)              // SBC A,(IX+N)
    OP_DO_A_R       (::op_DD_9F, DO_SBC_8, REG_A, DO_PREF_00)   // *SBC A,A

    OP_DO_A_R       (::op_DD_A0, DO_AND_8, REG_B, DO_PREF_00)   // *AND B
    OP_DO_A_R       (::op_DD_A1, DO_AND_8, REG_C, DO_PREF_00)   // *AND C
    OP_DO_A_R       (::op_DD_A2, DO_AND_8, REG_D, DO_PREF_00)   // *AND D
    OP_DO_A_R       (::op_DD_A3, DO_AND_8, REG_E, DO_PREF_00)   // *AND E
    OP_DO_A_R       (::op_DD_A4, DO_AND_8, REG_IXH, DO_PREF_00) // AND IXH
    OP_DO_A_R       (::op_DD_A5, DO_AND_8, REG_IXL, DO_PREF_00) // AND IXL
    OP_DO_A_ORP     (::op_DD_A6, DO_AND_8, REG_IX)              // AND (IX+N)
    OP_DO_A_R       (::op_DD_A7, DO_AND_8, REG_A, DO_PREF_00)   // *AND A
    OP_DO_A_R       (::op_DD_A8, DO_XOR_8, REG_B, DO_PREF_00)   // *XOR B
    OP_DO_A_R       (::op_DD_A9, DO_XOR_8, REG_C, DO_PREF_00)   // *XOR C
    OP_DO_A_R       (::op_DD_AA, DO_XOR_8, REG_D, DO_PREF_00)   // *XOR D
    OP_DO_A_R       (::op_DD_AB, DO_XOR_8, REG_E, DO_PREF_00)   // *XOR E
    OP_DO_A_R       (::op_DD_AC, DO_XOR_8, REG_IXH, DO_PREF_00) // XOR IXH
    OP_DO_A_R       (::op_DD_AD, DO_XOR_8, REG_IXL, DO_PREF_00) // XOR IXL
    OP_DO_A_ORP     (::op_DD_AE, DO_XOR_8, REG_IX)              // XOR (IX+N)
    OP_DO_A_R       (::op_DD_AF, DO_XOR_8, REG_A, DO_PREF_00)   // *XOR A

    OP_DO_A_R       (::op_DD_B0, DO_OR_8, REG_B, DO_PREF_00)    // *OR B
    OP_DO_A_R       (::op_DD_B1, DO_OR_8, REG_C, DO_PREF_00)    // *OR C
    OP_DO_A_R       (::op_DD_B2, DO_OR_8, REG_D, DO_PREF_00)    // *OR D
    OP_DO_A_R       (::op_DD_B3, DO_OR_8, REG_E, DO_PREF_00)    // *OR E
    OP_DO_A_R       (::op_DD_B4, DO_OR_8, REG_IXH, DO_PREF_00)  // OR IXH
    OP_DO_A_R       (::op_DD_B5, DO_OR_8, REG_IXL, DO_PREF_00)  // OR IXL
    OP_DO_A_ORP     (::op_DD_B6, DO_OR_8, REG_IX)               // OR (IX+N)
    OP_DO_A_R       (::op_DD_B7, DO_OR_8, REG_A, DO_PREF_00)    // *OR A
    OP_DO_A_R       (::op_DD_B8, DO_CP_8, REG_B, DO_PREF_00)    // *CP B
    OP_DO_A_R       (::op_DD_B9, DO_CP_8, REG_C, DO_PREF_00)    // *CP C
    OP_DO_A_R       (::op_DD_BA, DO_CP_8, REG_D, DO_PREF_00)    // *CP D
    OP_DO_A_R       (::op_DD_BB, DO_CP_8, REG_E, DO_PREF_00)    // *CP E
    OP_DO_A_R       (::op_DD_BC, DO_CP_8, REG_IXH, DO_PREF_00)  // CP IXH
    OP_DO_A_R       (::op_DD_BD, DO_CP_8, REG_IXL, DO_PREF_00)  // CP IXL
    OP_DO_A_ORP     (::op_DD_BE, DO_CP_8, REG_IX)               // CP (IX+N)
    OP_DO_A_R       (::op_DD_BF, DO_CP_8, REG_A, DO_PREF_00)    // *CP A

    OP_RET_CC       (::op_DD_C0, CC_NZ, DO_PREF_00)             // *RET NZ
    OP_POP_RP       (::op_DD_C1, REG_BC, DO_PREF_00)            // *POP BC
    OP_JP_CC        (::op_DD_C2, CC_NZ, DO_PREF_00)             // *JP NZ,NN
    OP_JP           (::op_DD_C3, DO_PREF_00)                    // *JP NN
    OP_CALL_CC      (::op_DD_C4, CC_NZ, DO_PREF_00)             // *CALL NZ,NN
    OP_PUSH_RP      (::op_DD_C5, REG_BC, DO_PREF_00)            // *PUSH BC
    OP_DO_A_N       (::op_DD_C6, DO_ADD_8, DO_PREF_00)          // *ADD A,N
    OP_RST          (::op_DD_C7, 0x00, DO_PREF_00)              // *RST #00
    OP_RET_CC       (::op_DD_C8, CC_Z, DO_PREF_00)              // *RET Z
    OP_RET          (::op_DD_C9, DO_PREF_00)                    // *RET
    OP_JP_CC        (::op_DD_CA, CC_Z, DO_PREF_00)              // *JP Z,NN
    OP_PREF_XX_CB   (::op_DD_CB, optable_DD_CB)                 // prefix #DD_CB
    OP_CALL_CC      (::op_DD_CC, CC_Z, DO_PREF_00)              // *CALL Z,NN
    OP_CALL         (::op_DD_CD, DO_PREF_00)                    // *CALL NN
    OP_DO_A_N       (::op_DD_CE, DO_ADC_8, DO_PREF_00)          // *ADC A,N
    OP_RST          (::op_DD_CF, 0x08, DO_PREF_00)              // *RST #08

    OP_RET_CC       (::op_DD_D0, CC_NC, DO_PREF_00)             // *RET NC
    OP_POP_RP       (::op_DD_D1, REG_DE, DO_PREF_00)            // *POP DE
    OP_JP_CC        (::op_DD_D2, CC_NC, DO_PREF_00)             // *JP NC,NN
    OP_OUT_N_A      (::op_DD_D3, DO_PREF_00)                    // *OUT (N),A
    OP_CALL_CC      (::op_DD_D4, CC_NC, DO_PREF_00)             // *CALL NC,NN
    OP_PUSH_RP      (::op_DD_D5, REG_DE, DO_PREF_00)            // *PUSH DE
    OP_DO_A_N       (::op_DD_D6, DO_SUB_8, DO_PREF_00)          // *SUB N
    OP_RST          (::op_DD_D7, 0x10, DO_PREF_00)              // *RST #10
    OP_RET_CC       (::op_DD_D8, CC_C, DO_PREF_00)              // *RET C
    OP_EXX          (::op_DD_D9, DO_PREF_00)                    // *EXX
    OP_JP_CC        (::op_DD_DA, CC_C, DO_PREF_00)              // *JP C,NN
    OP_IN_A_N       (::op_DD_DB, DO_PREF_00)                    // *IN A,(N)
    OP_CALL_CC      (::op_DD_DC, CC_C, DO_PREF_00)              // *CALL C,NN
    OP_PREF_DD      (::op_DD_DD)                                // prefix #DD
    OP_DO_A_N       (::op_DD_DE, DO_SBC_8, DO_PREF_00)          // *SBC A,N
    OP_RST          (::op_DD_DF, 0x18, DO_PREF_00)              // *RST #18

    OP_RET_CC       (::op_DD_E0, CC_PO, DO_PREF_00)                     // *RET PO
    OP_POP_RP       (::op_DD_E1, REG_IX, DO_PREF_00)                    // POP IX
    OP_JP_CC        (::op_DD_E2, CC_PO, DO_PREF_00)                     // *JP PO,NN
    OP_EX_SP_RP     (::op_DD_E3, REG_IXL, REG_IXH, REG_IX, DO_PREF_00)  // EX (SP),IX
    OP_CALL_CC      (::op_DD_E4, CC_PO, DO_PREF_00)                     // *CALL PO,NN
    OP_PUSH_RP      (::op_DD_E5, REG_IX, DO_PREF_00)                    // PUSH IX
    OP_DO_A_N       (::op_DD_E6, DO_AND_8, DO_PREF_00)                  // *AND N
    OP_RST          (::op_DD_E7, 0x20, DO_PREF_00)                      // *RST #20
    OP_RET_CC       (::op_DD_E8, CC_PE, DO_PREF_00)                     // *RET PE
    OP_JP_RP        (::op_DD_E9, REG_IX, DO_PREF_00)                    // JP IX
    OP_JP_CC        (::op_DD_EA, CC_PE, DO_PREF_00)                     // *JP PE,NN
    OP_EX_DE_HL     (::op_DD_EB, DO_PREF_00)                            // *EX DE,HL
    OP_CALL_CC      (::op_DD_EC, CC_PE, DO_PREF_00)                     // *CALL PE,NN
    OP_PREF_ED      (::op_DD_ED)                                        // prefix #ED
    OP_DO_A_N       (::op_DD_EE, DO_XOR_8, DO_PREF_00)                  // *XOR N
    OP_RST          (::op_DD_EF, 0x28, DO_PREF_00)                      // *RST #28

    OP_RET_CC       (::op_DD_F0, CC_P, DO_PREF_00)              // *RET P
    OP_POP_RP       (::op_DD_F1, REG_AF, DO_PREF_00)            // *POP AF
    OP_JP_CC        (::op_DD_F2, CC_P, DO_PREF_00)              // *JP P,NN
    OP_DI           (::op_DD_F3, DO_PREF_00)                    // *DI
    OP_CALL_CC      (::op_DD_F4, CC_P, DO_PREF_00)              // *CALL P,NN
    OP_PUSH_RP      (::op_DD_F5, REG_AF, DO_PREF_00)            // *PUSH AF
    OP_DO_A_N       (::op_DD_F6, DO_OR_8, DO_PREF_00)           // *OR N
    OP_RST          (::op_DD_F7, 0x30, DO_PREF_00)              // *RST #30
    OP_RET_CC       (::op_DD_F8, CC_M, DO_PREF_00)              // *RET M
    OP_LD_SP_RP     (::op_DD_F9, REG_IX, DO_PREF_00)            // LD SP,IX
    OP_JP_CC        (::op_DD_FA, CC_M, DO_PREF_00)              // *JP M,NN
    OP_EI           (::op_DD_FB, DO_PREF_00)                    // *EI
    OP_CALL_CC      (::op_DD_FC, CC_M, DO_PREF_00)              // *CALL M,NN
    OP_PREF_FD      (::op_DD_FD)                                // prefix #FD
    OP_DO_A_N       (::op_DD_FE, DO_CP_8, DO_PREF_00)           // *CP N
    OP_RST          (::op_DD_FF, 0x38, DO_PREF_00)              // *RST #38

    ::t_opcode optable_DD[0x100] = {
        ::op_DD_00, ::op_DD_01, ::op_DD_02, ::op_DD_03, ::op_DD_04, ::op_DD_05, ::op_DD_06, ::op_DD_07,
        ::op_DD_08, ::op_DD_09, ::op_DD_0A, ::op_DD_0B, ::op_DD_0C, ::op_DD_0D, ::op_DD_0E, ::op_DD_0F,
        ::op_DD_10, ::op_DD_11, ::op_DD_12, ::op_DD_13, ::op_DD_14, ::op_DD_15, ::op_DD_16, ::op_DD_17,
        ::op_DD_18, ::op_DD_19, ::op_DD_1A, ::op_DD_1B, ::op_DD_1C, ::op_DD_1D, ::op_DD_1E, ::op_DD_1F,
        ::op_DD_20, ::op_DD_21, ::op_DD_22, ::op_DD_23, ::op_DD_24, ::op_DD_25, ::op_DD_26, ::op_DD_27,
        ::op_DD_28, ::op_DD_29, ::op_DD_2A, ::op_DD_2B, ::op_DD_2C, ::op_DD_2D, ::op_DD_2E, ::op_DD_2F,
        ::op_DD_30, ::op_DD_31, ::op_DD_32, ::op_DD_33, ::op_DD_34, ::op_DD_35, ::op_DD_36, ::op_DD_37,
        ::op_DD_38, ::op_DD_39, ::op_DD_3A, ::op_DD_3B, ::op_DD_3C, ::op_DD_3D, ::op_DD_3E, ::op_DD_3F,
        ::op_DD_40, ::op_DD_41, ::op_DD_42, ::op_DD_43, ::op_DD_44, ::op_DD_45, ::op_DD_46, ::op_DD_47,
        ::op_DD_48, ::op_DD_49, ::op_DD_4A, ::op_DD_4B, ::op_DD_4C, ::op_DD_4D, ::op_DD_4E, ::op_DD_4F,
        ::op_DD_50, ::op_DD_51, ::op_DD_52, ::op_DD_53, ::op_DD_54, ::op_DD_55, ::op_DD_56, ::op_DD_57,
        ::op_DD_58, ::op_DD_59, ::op_DD_5A, ::op_DD_5B, ::op_DD_5C, ::op_DD_5D, ::op_DD_5E, ::op_DD_5F,
        ::op_DD_60, ::op_DD_61, ::op_DD_62, ::op_DD_63, ::op_DD_64, ::op_DD_65, ::op_DD_66, ::op_DD_67,
        ::op_DD_68, ::op_DD_69, ::op_DD_6A, ::op_DD_6B, ::op_DD_6C, ::op_DD_6D, ::op_DD_6E, ::op_DD_6F,
        ::op_DD_70, ::op_DD_71, ::op_DD_72, ::op_DD_73, ::op_DD_74, ::op_DD_75, ::op_DD_76, ::op_DD_77,
        ::op_DD_78, ::op_DD_79, ::op_DD_7A, ::op_DD_7B, ::op_DD_7C, ::op_DD_7D, ::op_DD_7E, ::op_DD_7F,
        ::op_DD_80, ::op_DD_81, ::op_DD_82, ::op_DD_83, ::op_DD_84, ::op_DD_85, ::op_DD_86, ::op_DD_87,
        ::op_DD_88, ::op_DD_89, ::op_DD_8A, ::op_DD_8B, ::op_DD_8C, ::op_DD_8D, ::op_DD_8E, ::op_DD_8F,
        ::op_DD_90, ::op_DD_91, ::op_DD_92, ::op_DD_93, ::op_DD_94, ::op_DD_95, ::op_DD_96, ::op_DD_97,
        ::op_DD_98, ::op_DD_99, ::op_DD_9A, ::op_DD_9B, ::op_DD_9C, ::op_DD_9D, ::op_DD_9E, ::op_DD_9F,
        ::op_DD_A0, ::op_DD_A1, ::op_DD_A2, ::op_DD_A3, ::op_DD_A4, ::op_DD_A5, ::op_DD_A6, ::op_DD_A7,
        ::op_DD_A8, ::op_DD_A9, ::op_DD_AA, ::op_DD_AB, ::op_DD_AC, ::op_DD_AD, ::op_DD_AE, ::op_DD_AF,
        ::op_DD_B0, ::op_DD_B1, ::op_DD_B2, ::op_DD_B3, ::op_DD_B4, ::op_DD_B5, ::op_DD_B6, ::op_DD_B7,
        ::op_DD_B8, ::op_DD_B9, ::op_DD_BA, ::op_DD_BB, ::op_DD_BC, ::op_DD_BD, ::op_DD_BE, ::op_DD_BF,
        ::op_DD_C0, ::op_DD_C1, ::op_DD_C2, ::op_DD_C3, ::op_DD_C4, ::op_DD_C5, ::op_DD_C6, ::op_DD_C7,
        ::op_DD_C8, ::op_DD_C9, ::op_DD_CA, ::op_DD_CB, ::op_DD_CC, ::op_DD_CD, ::op_DD_CE, ::op_DD_CF,
        ::op_DD_D0, ::op_DD_D1, ::op_DD_D2, ::op_DD_D3, ::op_DD_D4, ::op_DD_D5, ::op_DD_D6, ::op_DD_D7,
        ::op_DD_D8, ::op_DD_D9, ::op_DD_DA, ::op_DD_DB, ::op_DD_DC, ::op_DD_DD, ::op_DD_DE, ::op_DD_DF,
        ::op_DD_E0, ::op_DD_E1, ::op_DD_E2, ::op_DD_E3, ::op_DD_E4, ::op_DD_E5, ::op_DD_E6, ::op_DD_E7,
        ::op_DD_E8, ::op_DD_E9, ::op_DD_EA, ::op_DD_EB, ::op_DD_EC, ::op_DD_ED, ::op_DD_EE, ::op_DD_EF,
        ::op_DD_F0, ::op_DD_F1, ::op_DD_F2, ::op_DD_F3, ::op_DD_F4, ::op_DD_F5, ::op_DD_F6, ::op_DD_F7,
        ::op_DD_F8, ::op_DD_F9, ::op_DD_FA, ::op_DD_FB, ::op_DD_FC, ::op_DD_FD, ::op_DD_FE, ::op_DD_FF
    };
#end
