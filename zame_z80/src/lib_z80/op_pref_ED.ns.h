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
    OP_NOP          (::op_ED_00, DO_PREF_00)                    // *NOP
    #define          ::op_ED_01 ::op_ED_00
    #define          ::op_ED_02 ::op_ED_00
    #define          ::op_ED_03 ::op_ED_00
    #define          ::op_ED_04 ::op_ED_00
    #define          ::op_ED_05 ::op_ED_00
    #define          ::op_ED_06 ::op_ED_00
    #define          ::op_ED_07 ::op_ED_00
    #define          ::op_ED_08 ::op_ED_00
    #define          ::op_ED_09 ::op_ED_00
    #define          ::op_ED_0A ::op_ED_00
    #define          ::op_ED_0B ::op_ED_00
    #define          ::op_ED_0C ::op_ED_00
    #define          ::op_ED_0D ::op_ED_00
    #define          ::op_ED_0E ::op_ED_00
    #define          ::op_ED_0F ::op_ED_00

    #define          ::op_ED_10 ::op_ED_00
    #define          ::op_ED_11 ::op_ED_00
    #define          ::op_ED_12 ::op_ED_00
    #define          ::op_ED_13 ::op_ED_00
    #define          ::op_ED_14 ::op_ED_00
    #define          ::op_ED_15 ::op_ED_00
    #define          ::op_ED_16 ::op_ED_00
    #define          ::op_ED_17 ::op_ED_00
    #define          ::op_ED_18 ::op_ED_00
    #define          ::op_ED_19 ::op_ED_00
    #define          ::op_ED_1A ::op_ED_00
    #define          ::op_ED_1B ::op_ED_00
    #define          ::op_ED_1C ::op_ED_00
    #define          ::op_ED_1D ::op_ED_00
    #define          ::op_ED_1E ::op_ED_00
    #define          ::op_ED_1F ::op_ED_00

    #define          ::op_ED_20 ::op_ED_00
    #define          ::op_ED_21 ::op_ED_00
    #define          ::op_ED_22 ::op_ED_00
    #define          ::op_ED_23 ::op_ED_00
    #define          ::op_ED_24 ::op_ED_00
    #define          ::op_ED_25 ::op_ED_00
    #define          ::op_ED_26 ::op_ED_00
    #define          ::op_ED_27 ::op_ED_00
    #define          ::op_ED_28 ::op_ED_00
    #define          ::op_ED_29 ::op_ED_00
    #define          ::op_ED_2A ::op_ED_00
    #define          ::op_ED_2B ::op_ED_00
    #define          ::op_ED_2C ::op_ED_00
    #define          ::op_ED_2D ::op_ED_00
    #define          ::op_ED_2E ::op_ED_00
    #define          ::op_ED_2F ::op_ED_00

    #define          ::op_ED_30 ::op_ED_00
    #define          ::op_ED_31 ::op_ED_00
    #define          ::op_ED_32 ::op_ED_00
    #define          ::op_ED_33 ::op_ED_00
    #define          ::op_ED_34 ::op_ED_00
    #define          ::op_ED_35 ::op_ED_00
    #define          ::op_ED_36 ::op_ED_00
    #define          ::op_ED_37 ::op_ED_00
    #define          ::op_ED_38 ::op_ED_00
    #define          ::op_ED_39 ::op_ED_00
    #define          ::op_ED_3A ::op_ED_00
    #define          ::op_ED_3B ::op_ED_00
    #define          ::op_ED_3C ::op_ED_00
    #define          ::op_ED_3D ::op_ED_00
    #define          ::op_ED_3E ::op_ED_00
    #define          ::op_ED_3F ::op_ED_00

    OP_IN_R_BC_P00      (::op_ED_40, REG_B)                     // IN B,(C)
    OP_OUT_BC_R_P00     (::op_ED_41, REG_B)                     // OUT (C),B
    OP_SBC_HL_RP_P00    (::op_ED_42, REG_BC)                    // SBC HL,BC
    OP_LD_MNN_RP        (::op_ED_43, REG_C, REG_B, DO_PREF_00)  // LD (NN),BC
    OP_NEG_P00          (::op_ED_44)                            // NEG
    OP_RETN_P00         (::op_ED_45)                            // RETN
    OP_IM_P00           (::op_ED_46, 0)                         // IM 0
    OP_LD_I_A_P00       (::op_ED_47)                            // LD I,A
    OP_IN_R_BC_P00      (::op_ED_48, REG_C)                     // IN C,(C)
    OP_OUT_BC_R_P00     (::op_ED_49, REG_C)                     // OUT (C),C
    OP_ADC_HL_RP_P00    (::op_ED_4A, REG_BC)                    // ADC HL,BC
    OP_LD_RP_MNN        (::op_ED_4B, REG_C, REG_B, DO_PREF_00)  // LD BC,(NN)
    OP_NEG_P00          (::op_ED_4C)                            // NEG
    OP_RETI_P00         (::op_ED_4D)                            // RETI
    OP_IM_P00           (::op_ED_4E, 0)                         // IM 0
    OP_LD_RR_A_P00      (::op_ED_4F)                            // LD R,A

    OP_IN_R_BC_P00      (::op_ED_50, REG_D)                     // IN D,(C)
    OP_OUT_BC_R_P00     (::op_ED_51, REG_D)                     // OUT (C),D
    OP_SBC_HL_RP_P00    (::op_ED_52, REG_DE)                    // SBC HL,DE
    OP_LD_MNN_RP        (::op_ED_53, REG_E, REG_D, DO_PREF_00)  // LD (NN),DE
    OP_NEG_P00          (::op_ED_54)                            // NEG
    OP_RETN_P00         (::op_ED_55)                            // RETN
    OP_IM_P00           (::op_ED_56, 1)                         // IM 1
    OP_LD_A_I_P00       (::op_ED_57)                            // LD A,I
    OP_IN_R_BC_P00      (::op_ED_58, REG_E)                     // IN E,(C)
    OP_OUT_BC_R_P00     (::op_ED_59, REG_E)                     // OUT (C),E
    OP_ADC_HL_RP_P00    (::op_ED_5A, REG_DE)                    // ADC HL,DE
    OP_LD_RP_MNN        (::op_ED_5B, REG_E, REG_D, DO_PREF_00)  // LD DE,(NN)
    OP_NEG_P00          (::op_ED_5C)                            // NEG
    OP_RETN_P00         (::op_ED_5D)                            // RETN
    OP_IM_P00           (::op_ED_5E, 2)                         // IM 2
    OP_LD_A_RR_P00      (::op_ED_5F)                            // LD A,R

    OP_IN_R_BC_P00      (::op_ED_60, REG_H)                     // IN H,(C)
    OP_OUT_BC_R_P00     (::op_ED_61, REG_H)                     // OUT (H),D
    OP_SBC_HL_RP_P00    (::op_ED_62, REG_HL)                    // SBC HL,HL
    OP_LD_MNN_RP        (::op_ED_63, REG_L, REG_H, DO_PREF_00)  // LD (NN),HL
    OP_NEG_P00          (::op_ED_64)                            // NEG
    OP_RETN_P00         (::op_ED_65)                            // RETN
    OP_IM_P00           (::op_ED_66, 0)                         // IM 0
    OP_RRD_P00          (::op_ED_67)                            // RRD
    OP_IN_R_BC_P00      (::op_ED_68, REG_L)                     // IN L,(C)
    OP_OUT_BC_R_P00     (::op_ED_69, REG_L)                     // OUT (C),L
    OP_ADC_HL_RP_P00    (::op_ED_6A, REG_HL)                    // ADC HL,HL
    OP_LD_RP_MNN        (::op_ED_6B, REG_L, REG_H, DO_PREF_00)  // LD HL,(NN)
    OP_NEG_P00          (::op_ED_6C)                            // NEG
    OP_RETN_P00         (::op_ED_6D)                            // RETN
    OP_IM_P00           (::op_ED_6E, 0)                         // IM 0
    OP_RLD_P00          (::op_ED_6F)                            // RLD

    OP_IN_F_BC_P00      (::op_ED_70)                                // IN F,(C)
    OP_OUT_BC_0_P00     (::op_ED_71)                                // OUT (C),0
    OP_SBC_HL_RP_P00    (::op_ED_72, REG_SP)                        // SBC HL,SP
    OP_LD_MNN_RP        (::op_ED_73, REG_SPL, REG_SPH, DO_PREF_00)  // LD (NN),SP
    OP_NEG_P00          (::op_ED_74)                                // NEG
    OP_RETN_P00         (::op_ED_75)                                // RETN
    OP_IM_P00           (::op_ED_76, 1)                             // IM 1
    OP_LD_MRP_R         (::op_ED_77, REG_HL, REG_A, DO_PREF_00)     // LD (HL),A
    OP_IN_R_BC_P00      (::op_ED_78, REG_A)                         // IN A,(C)
    OP_OUT_BC_R_P00     (::op_ED_79, REG_A)                         // OUT (C),A
    OP_ADC_HL_RP_P00    (::op_ED_7A, REG_SP)                        // ADC HL,SP
    OP_LD_RP_MNN        (::op_ED_7B, REG_SPL, REG_SPH, DO_PREF_00)  // LD SP,(NN)
    OP_NEG_P00          (::op_ED_7C)                                // NEG
    OP_RETN_P00         (::op_ED_7D)                                // RETN
    OP_IM_P00           (::op_ED_7E, 2)                             // IM 2
    #define              ::op_ED_7F ::op_ED_00

    #define          ::op_ED_80 ::op_ED_00
    #define          ::op_ED_81 ::op_ED_00
    #define          ::op_ED_82 ::op_ED_00
    #define          ::op_ED_83 ::op_ED_00
    #define          ::op_ED_84 ::op_ED_00
    #define          ::op_ED_85 ::op_ED_00
    #define          ::op_ED_86 ::op_ED_00
    #define          ::op_ED_87 ::op_ED_00
    #define          ::op_ED_88 ::op_ED_00
    #define          ::op_ED_89 ::op_ED_00
    #define          ::op_ED_8A ::op_ED_00
    #define          ::op_ED_8B ::op_ED_00
    #define          ::op_ED_8C ::op_ED_00
    #define          ::op_ED_8D ::op_ED_00
    #define          ::op_ED_8E ::op_ED_00
    #define          ::op_ED_8F ::op_ED_00

    #define          ::op_ED_90 ::op_ED_00
    #define          ::op_ED_91 ::op_ED_00
    #define          ::op_ED_92 ::op_ED_00
    #define          ::op_ED_93 ::op_ED_00
    #define          ::op_ED_94 ::op_ED_00
    #define          ::op_ED_95 ::op_ED_00
    #define          ::op_ED_96 ::op_ED_00
    #define          ::op_ED_97 ::op_ED_00
    #define          ::op_ED_98 ::op_ED_00
    #define          ::op_ED_99 ::op_ED_00
    #define          ::op_ED_9A ::op_ED_00
    #define          ::op_ED_9B ::op_ED_00
    #define          ::op_ED_9C ::op_ED_00
    #define          ::op_ED_9D ::op_ED_00
    #define          ::op_ED_9E ::op_ED_00
    #define          ::op_ED_9F ::op_ED_00

    OP_LDI_P00      (::op_ED_A0)                                // LDI
    OP_CPI_P00      (::op_ED_A1)                                // CPI
    OP_INI_P00      (::op_ED_A2)                                // INI
    OP_OUTI_P00     (::op_ED_A3)                                // OUTI
    #define          ::op_ED_A4 ::op_ED_00
    #define          ::op_ED_A5 ::op_ED_00
    #define          ::op_ED_A6 ::op_ED_00
    #define          ::op_ED_A7 ::op_ED_00
    OP_LDD_P00      (::op_ED_A8)                                // LDD
    OP_CPD_P00      (::op_ED_A9)                                // CPD
    OP_IND_P00      (::op_ED_AA)                                // IND
    OP_OUTD_P00     (::op_ED_AB)                                // OUTD
    #define          ::op_ED_AC ::op_ED_00
    #define          ::op_ED_AD ::op_ED_00
    #define          ::op_ED_AE ::op_ED_00
    #define          ::op_ED_AF ::op_ED_00

    OP_LDIR_P00     (::op_ED_B0)                                // LDIR
    OP_CPIR_P00     (::op_ED_B1)                                // CPIR
    OP_INIR_P00     (::op_ED_B2)                                // INIR
    OP_OTIR_P00     (::op_ED_B3)                                // OTIR
    #define          ::op_ED_B4 ::op_ED_00
    #define          ::op_ED_B5 ::op_ED_00
    #define          ::op_ED_B6 ::op_ED_00
    #define          ::op_ED_B7 ::op_ED_00
    OP_LDDR_P00     (::op_ED_B8)                                // LDDR
    OP_CPDR_P00     (::op_ED_B9)                                // CPDR
    OP_INDR_P00     (::op_ED_BA)                                // INDR
    OP_OTDR_P00     (::op_ED_BB)                                // OTDR
    #define          ::op_ED_BC ::op_ED_00
    #define          ::op_ED_BD ::op_ED_00
    #define          ::op_ED_BE ::op_ED_00
    #define          ::op_ED_BF ::op_ED_00

    #define          ::op_ED_C0 ::op_ED_00
    #define          ::op_ED_C1 ::op_ED_00
    #define          ::op_ED_C2 ::op_ED_00
    #define          ::op_ED_C3 ::op_ED_00
    #define          ::op_ED_C4 ::op_ED_00
    #define          ::op_ED_C5 ::op_ED_00
    #define          ::op_ED_C6 ::op_ED_00
    #define          ::op_ED_C7 ::op_ED_00
    #define          ::op_ED_C8 ::op_ED_00
    #define          ::op_ED_C9 ::op_ED_00
    #define          ::op_ED_CA ::op_ED_00
    #define          ::op_ED_CB ::op_ED_00
    #define          ::op_ED_CC ::op_ED_00
    #define          ::op_ED_CD ::op_ED_00
    #define          ::op_ED_CE ::op_ED_00
    #define          ::op_ED_CF ::op_ED_00

    #define          ::op_ED_D0 ::op_ED_00
    #define          ::op_ED_D1 ::op_ED_00
    #define          ::op_ED_D2 ::op_ED_00
    #define          ::op_ED_D3 ::op_ED_00
    #define          ::op_ED_D4 ::op_ED_00
    #define          ::op_ED_D5 ::op_ED_00
    #define          ::op_ED_D6 ::op_ED_00
    #define          ::op_ED_D7 ::op_ED_00
    #define          ::op_ED_D8 ::op_ED_00
    #define          ::op_ED_D9 ::op_ED_00
    #define          ::op_ED_DA ::op_ED_00
    #define          ::op_ED_DB ::op_ED_00
    #define          ::op_ED_DC ::op_ED_00
    #define          ::op_ED_DD ::op_ED_00
    #define          ::op_ED_DE ::op_ED_00
    #define          ::op_ED_DF ::op_ED_00

    #define          ::op_ED_E0 ::op_ED_00
    #define          ::op_ED_E1 ::op_ED_00
    #define          ::op_ED_E2 ::op_ED_00
    #define          ::op_ED_E3 ::op_ED_00
    #define          ::op_ED_E4 ::op_ED_00
    #define          ::op_ED_E5 ::op_ED_00
    #define          ::op_ED_E6 ::op_ED_00
    #define          ::op_ED_E7 ::op_ED_00
    #define          ::op_ED_E8 ::op_ED_00
    #define          ::op_ED_E9 ::op_ED_00
    #define          ::op_ED_EA ::op_ED_00
    #define          ::op_ED_EB ::op_ED_00
    #define          ::op_ED_EC ::op_ED_00
    #define          ::op_ED_ED ::op_ED_00
    #define          ::op_ED_EE ::op_ED_00
    #define          ::op_ED_EF ::op_ED_00

    #define          ::op_ED_F0 ::op_ED_00
    #define          ::op_ED_F1 ::op_ED_00
    #define          ::op_ED_F2 ::op_ED_00
    #define          ::op_ED_F3 ::op_ED_00
    #define          ::op_ED_F4 ::op_ED_00
    #define          ::op_ED_F5 ::op_ED_00
    #define          ::op_ED_F6 ::op_ED_00
    #define          ::op_ED_F7 ::op_ED_00
    #define          ::op_ED_F8 ::op_ED_00
    #define          ::op_ED_F9 ::op_ED_00
    #define          ::op_ED_FA ::op_ED_00
    #define          ::op_ED_FB ::op_ED_00
    #define          ::op_ED_FC ::op_ED_00
    #define          ::op_ED_FD ::op_ED_00
    #define          ::op_ED_FE ::op_ED_00
    #define          ::op_ED_FF ::op_ED_00

    ::t_opcode optable_ED[0x100] = {
        ::op_ED_00, ::op_ED_01, ::op_ED_02, ::op_ED_03, ::op_ED_04, ::op_ED_05, ::op_ED_06, ::op_ED_07,
        ::op_ED_08, ::op_ED_09, ::op_ED_0A, ::op_ED_0B, ::op_ED_0C, ::op_ED_0D, ::op_ED_0E, ::op_ED_0F,
        ::op_ED_10, ::op_ED_11, ::op_ED_12, ::op_ED_13, ::op_ED_14, ::op_ED_15, ::op_ED_16, ::op_ED_17,
        ::op_ED_18, ::op_ED_19, ::op_ED_1A, ::op_ED_1B, ::op_ED_1C, ::op_ED_1D, ::op_ED_1E, ::op_ED_1F,
        ::op_ED_20, ::op_ED_21, ::op_ED_22, ::op_ED_23, ::op_ED_24, ::op_ED_25, ::op_ED_26, ::op_ED_27,
        ::op_ED_28, ::op_ED_29, ::op_ED_2A, ::op_ED_2B, ::op_ED_2C, ::op_ED_2D, ::op_ED_2E, ::op_ED_2F,
        ::op_ED_30, ::op_ED_31, ::op_ED_32, ::op_ED_33, ::op_ED_34, ::op_ED_35, ::op_ED_36, ::op_ED_37,
        ::op_ED_38, ::op_ED_39, ::op_ED_3A, ::op_ED_3B, ::op_ED_3C, ::op_ED_3D, ::op_ED_3E, ::op_ED_3F,
        ::op_ED_40, ::op_ED_41, ::op_ED_42, ::op_ED_43, ::op_ED_44, ::op_ED_45, ::op_ED_46, ::op_ED_47,
        ::op_ED_48, ::op_ED_49, ::op_ED_4A, ::op_ED_4B, ::op_ED_4C, ::op_ED_4D, ::op_ED_4E, ::op_ED_4F,
        ::op_ED_50, ::op_ED_51, ::op_ED_52, ::op_ED_53, ::op_ED_54, ::op_ED_55, ::op_ED_56, ::op_ED_57,
        ::op_ED_58, ::op_ED_59, ::op_ED_5A, ::op_ED_5B, ::op_ED_5C, ::op_ED_5D, ::op_ED_5E, ::op_ED_5F,
        ::op_ED_60, ::op_ED_61, ::op_ED_62, ::op_ED_63, ::op_ED_64, ::op_ED_65, ::op_ED_66, ::op_ED_67,
        ::op_ED_68, ::op_ED_69, ::op_ED_6A, ::op_ED_6B, ::op_ED_6C, ::op_ED_6D, ::op_ED_6E, ::op_ED_6F,
        ::op_ED_70, ::op_ED_71, ::op_ED_72, ::op_ED_73, ::op_ED_74, ::op_ED_75, ::op_ED_76, ::op_ED_77,
        ::op_ED_78, ::op_ED_79, ::op_ED_7A, ::op_ED_7B, ::op_ED_7C, ::op_ED_7D, ::op_ED_7E, ::op_ED_7F,
        ::op_ED_80, ::op_ED_81, ::op_ED_82, ::op_ED_83, ::op_ED_84, ::op_ED_85, ::op_ED_86, ::op_ED_87,
        ::op_ED_88, ::op_ED_89, ::op_ED_8A, ::op_ED_8B, ::op_ED_8C, ::op_ED_8D, ::op_ED_8E, ::op_ED_8F,
        ::op_ED_90, ::op_ED_91, ::op_ED_92, ::op_ED_93, ::op_ED_94, ::op_ED_95, ::op_ED_96, ::op_ED_97,
        ::op_ED_98, ::op_ED_99, ::op_ED_9A, ::op_ED_9B, ::op_ED_9C, ::op_ED_9D, ::op_ED_9E, ::op_ED_9F,
        ::op_ED_A0, ::op_ED_A1, ::op_ED_A2, ::op_ED_A3, ::op_ED_A4, ::op_ED_A5, ::op_ED_A6, ::op_ED_A7,
        ::op_ED_A8, ::op_ED_A9, ::op_ED_AA, ::op_ED_AB, ::op_ED_AC, ::op_ED_AD, ::op_ED_AE, ::op_ED_AF,
        ::op_ED_B0, ::op_ED_B1, ::op_ED_B2, ::op_ED_B3, ::op_ED_B4, ::op_ED_B5, ::op_ED_B6, ::op_ED_B7,
        ::op_ED_B8, ::op_ED_B9, ::op_ED_BA, ::op_ED_BB, ::op_ED_BC, ::op_ED_BD, ::op_ED_BE, ::op_ED_BF,
        ::op_ED_C0, ::op_ED_C1, ::op_ED_C2, ::op_ED_C3, ::op_ED_C4, ::op_ED_C5, ::op_ED_C6, ::op_ED_C7,
        ::op_ED_C8, ::op_ED_C9, ::op_ED_CA, ::op_ED_CB, ::op_ED_CC, ::op_ED_CD, ::op_ED_CE, ::op_ED_CF,
        ::op_ED_D0, ::op_ED_D1, ::op_ED_D2, ::op_ED_D3, ::op_ED_D4, ::op_ED_D5, ::op_ED_D6, ::op_ED_D7,
        ::op_ED_D8, ::op_ED_D9, ::op_ED_DA, ::op_ED_DB, ::op_ED_DC, ::op_ED_DD, ::op_ED_DE, ::op_ED_DF,
        ::op_ED_E0, ::op_ED_E1, ::op_ED_E2, ::op_ED_E3, ::op_ED_E4, ::op_ED_E5, ::op_ED_E6, ::op_ED_E7,
        ::op_ED_E8, ::op_ED_E9, ::op_ED_EA, ::op_ED_EB, ::op_ED_EC, ::op_ED_ED, ::op_ED_EE, ::op_ED_EF,
        ::op_ED_F0, ::op_ED_F1, ::op_ED_F2, ::op_ED_F3, ::op_ED_F4, ::op_ED_F5, ::op_ED_F6, ::op_ED_F7,
        ::op_ED_F8, ::op_ED_F9, ::op_ED_FA, ::op_ED_FB, ::op_ED_FC, ::op_ED_FD, ::op_ED_FE, ::op_ED_FF
    };
#end
