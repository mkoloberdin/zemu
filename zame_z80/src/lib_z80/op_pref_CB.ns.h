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
    OP_DO_R     (::op_CB_00, DO_RLC_8, REG_B, DO_PREF_00)   // RLC B
    OP_DO_R     (::op_CB_01, DO_RLC_8, REG_C, DO_PREF_00)   // RLC C
    OP_DO_R     (::op_CB_02, DO_RLC_8, REG_D, DO_PREF_00)   // RLC D
    OP_DO_R     (::op_CB_03, DO_RLC_8, REG_E, DO_PREF_00)   // RLC E
    OP_DO_R     (::op_CB_04, DO_RLC_8, REG_H, DO_PREF_00)   // RLC H
    OP_DO_R     (::op_CB_05, DO_RLC_8, REG_L, DO_PREF_00)   // RLC L
    OP_DO_MHL   (::op_CB_06, DO_RLC_8, DO_PREF_00)          // RLC (HL)
    OP_DO_R     (::op_CB_07, DO_RLC_8, REG_A, DO_PREF_00)   // RLC A
    OP_DO_R     (::op_CB_08, DO_RRC_8, REG_B, DO_PREF_00)   // RRC B
    OP_DO_R     (::op_CB_09, DO_RRC_8, REG_C, DO_PREF_00)   // RRC C
    OP_DO_R     (::op_CB_0A, DO_RRC_8, REG_D, DO_PREF_00)   // RRC D
    OP_DO_R     (::op_CB_0B, DO_RRC_8, REG_E, DO_PREF_00)   // RRC E
    OP_DO_R     (::op_CB_0C, DO_RRC_8, REG_H, DO_PREF_00)   // RRC H
    OP_DO_R     (::op_CB_0D, DO_RRC_8, REG_L, DO_PREF_00)   // RRC L
    OP_DO_MHL   (::op_CB_0E, DO_RRC_8, DO_PREF_00)          // RRC (HL)
    OP_DO_R     (::op_CB_0F, DO_RRC_8, REG_A, DO_PREF_00)   // RRC A

    OP_DO_R     (::op_CB_10, DO_RL_8, REG_B, DO_PREF_00)    // RL B
    OP_DO_R     (::op_CB_11, DO_RL_8, REG_C, DO_PREF_00)    // RL C
    OP_DO_R     (::op_CB_12, DO_RL_8, REG_D, DO_PREF_00)    // RL D
    OP_DO_R     (::op_CB_13, DO_RL_8, REG_E, DO_PREF_00)    // RL E
    OP_DO_R     (::op_CB_14, DO_RL_8, REG_H, DO_PREF_00)    // RL H
    OP_DO_R     (::op_CB_15, DO_RL_8, REG_L, DO_PREF_00)    // RL L
    OP_DO_MHL   (::op_CB_16, DO_RL_8, DO_PREF_00)           // RL (HL)
    OP_DO_R     (::op_CB_17, DO_RL_8, REG_A, DO_PREF_00)    // RL A
    OP_DO_R     (::op_CB_18, DO_RR_8, REG_B, DO_PREF_00)    // RR B
    OP_DO_R     (::op_CB_19, DO_RR_8, REG_C, DO_PREF_00)    // RR C
    OP_DO_R     (::op_CB_1A, DO_RR_8, REG_D, DO_PREF_00)    // RR D
    OP_DO_R     (::op_CB_1B, DO_RR_8, REG_E, DO_PREF_00)    // RR E
    OP_DO_R     (::op_CB_1C, DO_RR_8, REG_H, DO_PREF_00)    // RR H
    OP_DO_R     (::op_CB_1D, DO_RR_8, REG_L, DO_PREF_00)    // RR L
    OP_DO_MHL   (::op_CB_1E, DO_RR_8, DO_PREF_00)           // RR (HL)
    OP_DO_R     (::op_CB_1F, DO_RR_8, REG_A, DO_PREF_00)    // RR A

    OP_DO_R     (::op_CB_20, DO_SLA_8, REG_B, DO_PREF_00)   // SLA B
    OP_DO_R     (::op_CB_21, DO_SLA_8, REG_C, DO_PREF_00)   // SLA C
    OP_DO_R     (::op_CB_22, DO_SLA_8, REG_D, DO_PREF_00)   // SLA D
    OP_DO_R     (::op_CB_23, DO_SLA_8, REG_E, DO_PREF_00)   // SLA E
    OP_DO_R     (::op_CB_24, DO_SLA_8, REG_H, DO_PREF_00)   // SLA H
    OP_DO_R     (::op_CB_25, DO_SLA_8, REG_L, DO_PREF_00)   // SLA L
    OP_DO_MHL   (::op_CB_26, DO_SLA_8, DO_PREF_00)          // SLA (HL)
    OP_DO_R     (::op_CB_27, DO_SLA_8, REG_A, DO_PREF_00)   // SLA A
    OP_DO_R     (::op_CB_28, DO_SRA_8, REG_B, DO_PREF_00)   // SRA B
    OP_DO_R     (::op_CB_29, DO_SRA_8, REG_C, DO_PREF_00)   // SRA C
    OP_DO_R     (::op_CB_2A, DO_SRA_8, REG_D, DO_PREF_00)   // SRA D
    OP_DO_R     (::op_CB_2B, DO_SRA_8, REG_E, DO_PREF_00)   // SRA E
    OP_DO_R     (::op_CB_2C, DO_SRA_8, REG_H, DO_PREF_00)   // SRA H
    OP_DO_R     (::op_CB_2D, DO_SRA_8, REG_L, DO_PREF_00)   // SRA L
    OP_DO_MHL   (::op_CB_2E, DO_SRA_8, DO_PREF_00)          // SRA (HL)
    OP_DO_R     (::op_CB_2F, DO_SRA_8, REG_A, DO_PREF_00)   // SRA A

    OP_DO_R     (::op_CB_30, DO_SLL_8, REG_B, DO_PREF_00)   // SLL B
    OP_DO_R     (::op_CB_31, DO_SLL_8, REG_C, DO_PREF_00)   // SLL C
    OP_DO_R     (::op_CB_32, DO_SLL_8, REG_D, DO_PREF_00)   // SLL D
    OP_DO_R     (::op_CB_33, DO_SLL_8, REG_E, DO_PREF_00)   // SLL E
    OP_DO_R     (::op_CB_34, DO_SLL_8, REG_H, DO_PREF_00)   // SLL H
    OP_DO_R     (::op_CB_35, DO_SLL_8, REG_L, DO_PREF_00)   // SLL L
    OP_DO_MHL   (::op_CB_36, DO_SLL_8, DO_PREF_00)          // SLL (HL)
    OP_DO_R     (::op_CB_37, DO_SLL_8, REG_A, DO_PREF_00)   // SLL A
    OP_DO_R     (::op_CB_38, DO_SRL_8, REG_B, DO_PREF_00)   // SRL B
    OP_DO_R     (::op_CB_39, DO_SRL_8, REG_C, DO_PREF_00)   // SRL C
    OP_DO_R     (::op_CB_3A, DO_SRL_8, REG_D, DO_PREF_00)   // SRL D
    OP_DO_R     (::op_CB_3B, DO_SRL_8, REG_E, DO_PREF_00)   // SRL E
    OP_DO_R     (::op_CB_3C, DO_SRL_8, REG_H, DO_PREF_00)   // SRL H
    OP_DO_R     (::op_CB_3D, DO_SRL_8, REG_L, DO_PREF_00)   // SRL L
    OP_DO_MHL   (::op_CB_3E, DO_SRL_8, DO_PREF_00)          // SRL (HL)
    OP_DO_R     (::op_CB_3F, DO_SRL_8, REG_A, DO_PREF_00)   // SRL A

    OP_BIT_R    (::op_CB_40, 0, REG_B, DO_PREF_00)          // BIT 0,B
    OP_BIT_R    (::op_CB_41, 0, REG_C, DO_PREF_00)          // BIT 0,C
    OP_BIT_R    (::op_CB_42, 0, REG_D, DO_PREF_00)          // BIT 0,D
    OP_BIT_R    (::op_CB_43, 0, REG_E, DO_PREF_00)          // BIT 0,E
    OP_BIT_R    (::op_CB_44, 0, REG_H, DO_PREF_00)          // BIT 0,H
    OP_BIT_R    (::op_CB_45, 0, REG_L, DO_PREF_00)          // BIT 0,L
    OP_BIT_MHL  (::op_CB_46, 0, DO_PREF_00)                 // BIT 0,(HL)
    OP_BIT_R    (::op_CB_47, 0, REG_A, DO_PREF_00)          // BIT 0,A
    OP_BIT_R    (::op_CB_48, 1, REG_B, DO_PREF_00)          // BIT 1,B
    OP_BIT_R    (::op_CB_49, 1, REG_C, DO_PREF_00)          // BIT 1,C
    OP_BIT_R    (::op_CB_4A, 1, REG_D, DO_PREF_00)          // BIT 1,D
    OP_BIT_R    (::op_CB_4B, 1, REG_E, DO_PREF_00)          // BIT 1,E
    OP_BIT_R    (::op_CB_4C, 1, REG_H, DO_PREF_00)          // BIT 1,H
    OP_BIT_R    (::op_CB_4D, 1, REG_L, DO_PREF_00)          // BIT 1,L
    OP_BIT_MHL  (::op_CB_4E, 1, DO_PREF_00)                 // BIT 1,(HL)
    OP_BIT_R    (::op_CB_4F, 1, REG_A, DO_PREF_00)          // BIT 1,A

    OP_BIT_R    (::op_CB_50, 2, REG_B, DO_PREF_00)          // BIT 2,B
    OP_BIT_R    (::op_CB_51, 2, REG_C, DO_PREF_00)          // BIT 2,C
    OP_BIT_R    (::op_CB_52, 2, REG_D, DO_PREF_00)          // BIT 2,D
    OP_BIT_R    (::op_CB_53, 2, REG_E, DO_PREF_00)          // BIT 2,E
    OP_BIT_R    (::op_CB_54, 2, REG_H, DO_PREF_00)          // BIT 2,H
    OP_BIT_R    (::op_CB_55, 2, REG_L, DO_PREF_00)          // BIT 2,L
    OP_BIT_MHL  (::op_CB_56, 2, DO_PREF_00)                 // BIT 2,(HL)
    OP_BIT_R    (::op_CB_57, 2, REG_A, DO_PREF_00)          // BIT 2,A
    OP_BIT_R    (::op_CB_58, 3, REG_B, DO_PREF_00)          // BIT 3,B
    OP_BIT_R    (::op_CB_59, 3, REG_C, DO_PREF_00)          // BIT 3,C
    OP_BIT_R    (::op_CB_5A, 3, REG_D, DO_PREF_00)          // BIT 3,D
    OP_BIT_R    (::op_CB_5B, 3, REG_E, DO_PREF_00)          // BIT 3,E
    OP_BIT_R    (::op_CB_5C, 3, REG_H, DO_PREF_00)          // BIT 3,H
    OP_BIT_R    (::op_CB_5D, 3, REG_L, DO_PREF_00)          // BIT 3,L
    OP_BIT_MHL  (::op_CB_5E, 3, DO_PREF_00)                 // BIT 3,(HL)
    OP_BIT_R    (::op_CB_5F, 3, REG_A, DO_PREF_00)          // BIT 3,A

    OP_BIT_R    (::op_CB_60, 4, REG_B, DO_PREF_00)          // BIT 4,B
    OP_BIT_R    (::op_CB_61, 4, REG_C, DO_PREF_00)          // BIT 4,C
    OP_BIT_R    (::op_CB_62, 4, REG_D, DO_PREF_00)          // BIT 4,D
    OP_BIT_R    (::op_CB_63, 4, REG_E, DO_PREF_00)          // BIT 4,E
    OP_BIT_R    (::op_CB_64, 4, REG_H, DO_PREF_00)          // BIT 4,H
    OP_BIT_R    (::op_CB_65, 4, REG_L, DO_PREF_00)          // BIT 4,L
    OP_BIT_MHL  (::op_CB_66, 4, DO_PREF_00)                 // BIT 4,(HL)
    OP_BIT_R    (::op_CB_67, 4, REG_A, DO_PREF_00)          // BIT 4,A
    OP_BIT_R    (::op_CB_68, 5, REG_B, DO_PREF_00)          // BIT 5,B
    OP_BIT_R    (::op_CB_69, 5, REG_C, DO_PREF_00)          // BIT 5,C
    OP_BIT_R    (::op_CB_6A, 5, REG_D, DO_PREF_00)          // BIT 5,D
    OP_BIT_R    (::op_CB_6B, 5, REG_E, DO_PREF_00)          // BIT 5,E
    OP_BIT_R    (::op_CB_6C, 5, REG_H, DO_PREF_00)          // BIT 5,H
    OP_BIT_R    (::op_CB_6D, 5, REG_L, DO_PREF_00)          // BIT 5,L
    OP_BIT_MHL  (::op_CB_6E, 5, DO_PREF_00)                 // BIT 5,(HL)
    OP_BIT_R    (::op_CB_6F, 5, REG_A, DO_PREF_00)          // BIT 5,A

    OP_BIT_R    (::op_CB_70, 6, REG_B, DO_PREF_00)          // BIT 6,B
    OP_BIT_R    (::op_CB_71, 6, REG_C, DO_PREF_00)          // BIT 6,C
    OP_BIT_R    (::op_CB_72, 6, REG_D, DO_PREF_00)          // BIT 6,D
    OP_BIT_R    (::op_CB_73, 6, REG_E, DO_PREF_00)          // BIT 6,E
    OP_BIT_R    (::op_CB_74, 6, REG_H, DO_PREF_00)          // BIT 6,H
    OP_BIT_R    (::op_CB_75, 6, REG_L, DO_PREF_00)          // BIT 6,L
    OP_BIT_MHL  (::op_CB_76, 6, DO_PREF_00)                 // BIT 6,(HL)
    OP_BIT_R    (::op_CB_77, 6, REG_A, DO_PREF_00)          // BIT 6,A
    OP_BIT_R    (::op_CB_78, 7, REG_B, DO_PREF_00)          // BIT 7,B
    OP_BIT_R    (::op_CB_79, 7, REG_C, DO_PREF_00)          // BIT 7,C
    OP_BIT_R    (::op_CB_7A, 7, REG_D, DO_PREF_00)          // BIT 7,D
    OP_BIT_R    (::op_CB_7B, 7, REG_E, DO_PREF_00)          // BIT 7,E
    OP_BIT_R    (::op_CB_7C, 7, REG_H, DO_PREF_00)          // BIT 7,H
    OP_BIT_R    (::op_CB_7D, 7, REG_L, DO_PREF_00)          // BIT 7,L
    OP_BIT_MHL  (::op_CB_7E, 7, DO_PREF_00)                 // BIT 7,(HL)
    OP_BIT_R    (::op_CB_7F, 7, REG_A, DO_PREF_00)          // BIT 7,A

    OP_RES_R    (::op_CB_80, 0, REG_B, DO_PREF_00)          // RES 0,B
    OP_RES_R    (::op_CB_81, 0, REG_C, DO_PREF_00)          // RES 0,C
    OP_RES_R    (::op_CB_82, 0, REG_D, DO_PREF_00)          // RES 0,D
    OP_RES_R    (::op_CB_83, 0, REG_E, DO_PREF_00)          // RES 0,E
    OP_RES_R    (::op_CB_84, 0, REG_H, DO_PREF_00)          // RES 0,H
    OP_RES_R    (::op_CB_85, 0, REG_L, DO_PREF_00)          // RES 0,L
    OP_RES_MHL  (::op_CB_86, 0, DO_PREF_00)                 // RES 0,(HL)
    OP_RES_R    (::op_CB_87, 0, REG_A, DO_PREF_00)          // RES 0,A
    OP_RES_R    (::op_CB_88, 1, REG_B, DO_PREF_00)          // RES 1,B
    OP_RES_R    (::op_CB_89, 1, REG_C, DO_PREF_00)          // RES 1,C
    OP_RES_R    (::op_CB_8A, 1, REG_D, DO_PREF_00)          // RES 1,D
    OP_RES_R    (::op_CB_8B, 1, REG_E, DO_PREF_00)          // RES 1,E
    OP_RES_R    (::op_CB_8C, 1, REG_H, DO_PREF_00)          // RES 1,H
    OP_RES_R    (::op_CB_8D, 1, REG_L, DO_PREF_00)          // RES 1,L
    OP_RES_MHL  (::op_CB_8E, 1, DO_PREF_00)                 // RES 1,(HL)
    OP_RES_R    (::op_CB_8F, 1, REG_A, DO_PREF_00)          // RES 1,A

    OP_RES_R    (::op_CB_90, 2, REG_B, DO_PREF_00)          // RES 2,B
    OP_RES_R    (::op_CB_91, 2, REG_C, DO_PREF_00)          // RES 2,C
    OP_RES_R    (::op_CB_92, 2, REG_D, DO_PREF_00)          // RES 2,D
    OP_RES_R    (::op_CB_93, 2, REG_E, DO_PREF_00)          // RES 2,E
    OP_RES_R    (::op_CB_94, 2, REG_H, DO_PREF_00)          // RES 2,H
    OP_RES_R    (::op_CB_95, 2, REG_L, DO_PREF_00)          // RES 2,L
    OP_RES_MHL  (::op_CB_96, 2, DO_PREF_00)                 // RES 2,(HL)
    OP_RES_R    (::op_CB_97, 2, REG_A, DO_PREF_00)          // RES 2,A
    OP_RES_R    (::op_CB_98, 3, REG_B, DO_PREF_00)          // RES 3,B
    OP_RES_R    (::op_CB_99, 3, REG_C, DO_PREF_00)          // RES 3,C
    OP_RES_R    (::op_CB_9A, 3, REG_D, DO_PREF_00)          // RES 3,D
    OP_RES_R    (::op_CB_9B, 3, REG_E, DO_PREF_00)          // RES 3,E
    OP_RES_R    (::op_CB_9C, 3, REG_H, DO_PREF_00)          // RES 3,H
    OP_RES_R    (::op_CB_9D, 3, REG_L, DO_PREF_00)          // RES 3,L
    OP_RES_MHL  (::op_CB_9E, 3, DO_PREF_00)                 // RES 3,(HL)
    OP_RES_R    (::op_CB_9F, 3, REG_A, DO_PREF_00)          // RES 3,A

    OP_RES_R    (::op_CB_A0, 4, REG_B, DO_PREF_00)          // RES 4,B
    OP_RES_R    (::op_CB_A1, 4, REG_C, DO_PREF_00)          // RES 4,C
    OP_RES_R    (::op_CB_A2, 4, REG_D, DO_PREF_00)          // RES 4,D
    OP_RES_R    (::op_CB_A3, 4, REG_E, DO_PREF_00)          // RES 4,E
    OP_RES_R    (::op_CB_A4, 4, REG_H, DO_PREF_00)          // RES 4,H
    OP_RES_R    (::op_CB_A5, 4, REG_L, DO_PREF_00)          // RES 4,L
    OP_RES_MHL  (::op_CB_A6, 4, DO_PREF_00)                 // RES 4,(HL)
    OP_RES_R    (::op_CB_A7, 4, REG_A, DO_PREF_00)          // RES 4,A
    OP_RES_R    (::op_CB_A8, 5, REG_B, DO_PREF_00)          // RES 5,B
    OP_RES_R    (::op_CB_A9, 5, REG_C, DO_PREF_00)          // RES 5,C
    OP_RES_R    (::op_CB_AA, 5, REG_D, DO_PREF_00)          // RES 5,D
    OP_RES_R    (::op_CB_AB, 5, REG_E, DO_PREF_00)          // RES 5,E
    OP_RES_R    (::op_CB_AC, 5, REG_H, DO_PREF_00)          // RES 5,H
    OP_RES_R    (::op_CB_AD, 5, REG_L, DO_PREF_00)          // RES 5,L
    OP_RES_MHL  (::op_CB_AE, 5, DO_PREF_00)                 // RES 5,(HL)
    OP_RES_R    (::op_CB_AF, 5, REG_A, DO_PREF_00)          // RES 5,A

    OP_RES_R    (::op_CB_B0, 6, REG_B, DO_PREF_00)          // RES 6,B
    OP_RES_R    (::op_CB_B1, 6, REG_C, DO_PREF_00)          // RES 6,C
    OP_RES_R    (::op_CB_B2, 6, REG_D, DO_PREF_00)          // RES 6,D
    OP_RES_R    (::op_CB_B3, 6, REG_E, DO_PREF_00)          // RES 6,E
    OP_RES_R    (::op_CB_B4, 6, REG_H, DO_PREF_00)          // RES 6,H
    OP_RES_R    (::op_CB_B5, 6, REG_L, DO_PREF_00)          // RES 6,L
    OP_RES_MHL  (::op_CB_B6, 6, DO_PREF_00)                 // RES 6,(HL)
    OP_RES_R    (::op_CB_B7, 6, REG_A, DO_PREF_00)          // RES 6,A
    OP_RES_R    (::op_CB_B8, 7, REG_B, DO_PREF_00)          // RES 7,B
    OP_RES_R    (::op_CB_B9, 7, REG_C, DO_PREF_00)          // RES 7,C
    OP_RES_R    (::op_CB_BA, 7, REG_D, DO_PREF_00)          // RES 7,D
    OP_RES_R    (::op_CB_BB, 7, REG_E, DO_PREF_00)          // RES 7,E
    OP_RES_R    (::op_CB_BC, 7, REG_H, DO_PREF_00)          // RES 7,H
    OP_RES_R    (::op_CB_BD, 7, REG_L, DO_PREF_00)          // RES 7,L
    OP_RES_MHL  (::op_CB_BE, 7, DO_PREF_00)                 // RES 7,(HL)
    OP_RES_R    (::op_CB_BF, 7, REG_A, DO_PREF_00)          // RES 7,A

    OP_SET_R    (::op_CB_C0, 0, REG_B, DO_PREF_00)          // SET 0,B
    OP_SET_R    (::op_CB_C1, 0, REG_C, DO_PREF_00)          // SET 0,C
    OP_SET_R    (::op_CB_C2, 0, REG_D, DO_PREF_00)          // SET 0,D
    OP_SET_R    (::op_CB_C3, 0, REG_E, DO_PREF_00)          // SET 0,E
    OP_SET_R    (::op_CB_C4, 0, REG_H, DO_PREF_00)          // SET 0,H
    OP_SET_R    (::op_CB_C5, 0, REG_L, DO_PREF_00)          // SET 0,L
    OP_SET_MHL  (::op_CB_C6, 0, DO_PREF_00)                 // SET 0,(HL)
    OP_SET_R    (::op_CB_C7, 0, REG_A, DO_PREF_00)          // SET 0,A
    OP_SET_R    (::op_CB_C8, 1, REG_B, DO_PREF_00)          // SET 1,B
    OP_SET_R    (::op_CB_C9, 1, REG_C, DO_PREF_00)          // SET 1,C
    OP_SET_R    (::op_CB_CA, 1, REG_D, DO_PREF_00)          // SET 1,D
    OP_SET_R    (::op_CB_CB, 1, REG_E, DO_PREF_00)          // SET 1,E
    OP_SET_R    (::op_CB_CC, 1, REG_H, DO_PREF_00)          // SET 1,H
    OP_SET_R    (::op_CB_CD, 1, REG_L, DO_PREF_00)          // SET 1,L
    OP_SET_MHL  (::op_CB_CE, 1, DO_PREF_00)                 // SET 1,(HL)
    OP_SET_R    (::op_CB_CF, 1, REG_A, DO_PREF_00)          // SET 1,A

    OP_SET_R    (::op_CB_D0, 2, REG_B, DO_PREF_00)          // SET 2,B
    OP_SET_R    (::op_CB_D1, 2, REG_C, DO_PREF_00)          // SET 2,C
    OP_SET_R    (::op_CB_D2, 2, REG_D, DO_PREF_00)          // SET 2,D
    OP_SET_R    (::op_CB_D3, 2, REG_E, DO_PREF_00)          // SET 2,E
    OP_SET_R    (::op_CB_D4, 2, REG_H, DO_PREF_00)          // SET 2,H
    OP_SET_R    (::op_CB_D5, 2, REG_L, DO_PREF_00)          // SET 2,L
    OP_SET_MHL  (::op_CB_D6, 2, DO_PREF_00)                 // SET 2,(HL)
    OP_SET_R    (::op_CB_D7, 2, REG_A, DO_PREF_00)          // SET 2,A
    OP_SET_R    (::op_CB_D8, 3, REG_B, DO_PREF_00)          // SET 3,B
    OP_SET_R    (::op_CB_D9, 3, REG_C, DO_PREF_00)          // SET 3,C
    OP_SET_R    (::op_CB_DA, 3, REG_D, DO_PREF_00)          // SET 3,D
    OP_SET_R    (::op_CB_DB, 3, REG_E, DO_PREF_00)          // SET 3,E
    OP_SET_R    (::op_CB_DC, 3, REG_H, DO_PREF_00)          // SET 3,H
    OP_SET_R    (::op_CB_DD, 3, REG_L, DO_PREF_00)          // SET 3,L
    OP_SET_MHL  (::op_CB_DE, 3, DO_PREF_00)                 // SET 3,(HL)
    OP_SET_R    (::op_CB_DF, 3, REG_A, DO_PREF_00)          // SET 3,A

    OP_SET_R    (::op_CB_E0, 4, REG_B, DO_PREF_00)          // SET 4,B
    OP_SET_R    (::op_CB_E1, 4, REG_C, DO_PREF_00)          // SET 4,C
    OP_SET_R    (::op_CB_E2, 4, REG_D, DO_PREF_00)          // SET 4,D
    OP_SET_R    (::op_CB_E3, 4, REG_E, DO_PREF_00)          // SET 4,E
    OP_SET_R    (::op_CB_E4, 4, REG_H, DO_PREF_00)          // SET 4,H
    OP_SET_R    (::op_CB_E5, 4, REG_L, DO_PREF_00)          // SET 4,L
    OP_SET_MHL  (::op_CB_E6, 4, DO_PREF_00)                 // SET 4,(HL)
    OP_SET_R    (::op_CB_E7, 4, REG_A, DO_PREF_00)          // SET 4,A
    OP_SET_R    (::op_CB_E8, 5, REG_B, DO_PREF_00)          // SET 5,B
    OP_SET_R    (::op_CB_E9, 5, REG_C, DO_PREF_00)          // SET 5,C
    OP_SET_R    (::op_CB_EA, 5, REG_D, DO_PREF_00)          // SET 5,D
    OP_SET_R    (::op_CB_EB, 5, REG_E, DO_PREF_00)          // SET 5,E
    OP_SET_R    (::op_CB_EC, 5, REG_H, DO_PREF_00)          // SET 5,H
    OP_SET_R    (::op_CB_ED, 5, REG_L, DO_PREF_00)          // SET 5,L
    OP_SET_MHL  (::op_CB_EE, 5, DO_PREF_00)                 // SET 5,(HL)
    OP_SET_R    (::op_CB_EF, 5, REG_A, DO_PREF_00)          // SET 5,A

    OP_SET_R    (::op_CB_F0, 6, REG_B, DO_PREF_00)          // SET 6,B
    OP_SET_R    (::op_CB_F1, 6, REG_C, DO_PREF_00)          // SET 6,C
    OP_SET_R    (::op_CB_F2, 6, REG_D, DO_PREF_00)          // SET 6,D
    OP_SET_R    (::op_CB_F3, 6, REG_E, DO_PREF_00)          // SET 6,E
    OP_SET_R    (::op_CB_F4, 6, REG_H, DO_PREF_00)          // SET 6,H
    OP_SET_R    (::op_CB_F5, 6, REG_L, DO_PREF_00)          // SET 6,L
    OP_SET_MHL  (::op_CB_F6, 6, DO_PREF_00)                 // SET 6,(HL)
    OP_SET_R    (::op_CB_F7, 6, REG_A, DO_PREF_00)          // SET 6,A
    OP_SET_R    (::op_CB_F8, 7, REG_B, DO_PREF_00)          // SET 7,B
    OP_SET_R    (::op_CB_F9, 7, REG_C, DO_PREF_00)          // SET 7,C
    OP_SET_R    (::op_CB_FA, 7, REG_D, DO_PREF_00)          // SET 7,D
    OP_SET_R    (::op_CB_FB, 7, REG_E, DO_PREF_00)          // SET 7,E
    OP_SET_R    (::op_CB_FC, 7, REG_H, DO_PREF_00)          // SET 7,H
    OP_SET_R    (::op_CB_FD, 7, REG_L, DO_PREF_00)          // SET 7,L
    OP_SET_MHL  (::op_CB_FE, 7, DO_PREF_00)                 // SET 7,(HL)
    OP_SET_R    (::op_CB_FF, 7, REG_A, DO_PREF_00)          // SET 7,A

    ::t_opcode optable_CB[0x100] = {
        ::op_CB_00, ::op_CB_01, ::op_CB_02, ::op_CB_03, ::op_CB_04, ::op_CB_05, ::op_CB_06, ::op_CB_07,
        ::op_CB_08, ::op_CB_09, ::op_CB_0A, ::op_CB_0B, ::op_CB_0C, ::op_CB_0D, ::op_CB_0E, ::op_CB_0F,
        ::op_CB_10, ::op_CB_11, ::op_CB_12, ::op_CB_13, ::op_CB_14, ::op_CB_15, ::op_CB_16, ::op_CB_17,
        ::op_CB_18, ::op_CB_19, ::op_CB_1A, ::op_CB_1B, ::op_CB_1C, ::op_CB_1D, ::op_CB_1E, ::op_CB_1F,
        ::op_CB_20, ::op_CB_21, ::op_CB_22, ::op_CB_23, ::op_CB_24, ::op_CB_25, ::op_CB_26, ::op_CB_27,
        ::op_CB_28, ::op_CB_29, ::op_CB_2A, ::op_CB_2B, ::op_CB_2C, ::op_CB_2D, ::op_CB_2E, ::op_CB_2F,
        ::op_CB_30, ::op_CB_31, ::op_CB_32, ::op_CB_33, ::op_CB_34, ::op_CB_35, ::op_CB_36, ::op_CB_37,
        ::op_CB_38, ::op_CB_39, ::op_CB_3A, ::op_CB_3B, ::op_CB_3C, ::op_CB_3D, ::op_CB_3E, ::op_CB_3F,
        ::op_CB_40, ::op_CB_41, ::op_CB_42, ::op_CB_43, ::op_CB_44, ::op_CB_45, ::op_CB_46, ::op_CB_47,
        ::op_CB_48, ::op_CB_49, ::op_CB_4A, ::op_CB_4B, ::op_CB_4C, ::op_CB_4D, ::op_CB_4E, ::op_CB_4F,
        ::op_CB_50, ::op_CB_51, ::op_CB_52, ::op_CB_53, ::op_CB_54, ::op_CB_55, ::op_CB_56, ::op_CB_57,
        ::op_CB_58, ::op_CB_59, ::op_CB_5A, ::op_CB_5B, ::op_CB_5C, ::op_CB_5D, ::op_CB_5E, ::op_CB_5F,
        ::op_CB_60, ::op_CB_61, ::op_CB_62, ::op_CB_63, ::op_CB_64, ::op_CB_65, ::op_CB_66, ::op_CB_67,
        ::op_CB_68, ::op_CB_69, ::op_CB_6A, ::op_CB_6B, ::op_CB_6C, ::op_CB_6D, ::op_CB_6E, ::op_CB_6F,
        ::op_CB_70, ::op_CB_71, ::op_CB_72, ::op_CB_73, ::op_CB_74, ::op_CB_75, ::op_CB_76, ::op_CB_77,
        ::op_CB_78, ::op_CB_79, ::op_CB_7A, ::op_CB_7B, ::op_CB_7C, ::op_CB_7D, ::op_CB_7E, ::op_CB_7F,
        ::op_CB_80, ::op_CB_81, ::op_CB_82, ::op_CB_83, ::op_CB_84, ::op_CB_85, ::op_CB_86, ::op_CB_87,
        ::op_CB_88, ::op_CB_89, ::op_CB_8A, ::op_CB_8B, ::op_CB_8C, ::op_CB_8D, ::op_CB_8E, ::op_CB_8F,
        ::op_CB_90, ::op_CB_91, ::op_CB_92, ::op_CB_93, ::op_CB_94, ::op_CB_95, ::op_CB_96, ::op_CB_97,
        ::op_CB_98, ::op_CB_99, ::op_CB_9A, ::op_CB_9B, ::op_CB_9C, ::op_CB_9D, ::op_CB_9E, ::op_CB_9F,
        ::op_CB_A0, ::op_CB_A1, ::op_CB_A2, ::op_CB_A3, ::op_CB_A4, ::op_CB_A5, ::op_CB_A6, ::op_CB_A7,
        ::op_CB_A8, ::op_CB_A9, ::op_CB_AA, ::op_CB_AB, ::op_CB_AC, ::op_CB_AD, ::op_CB_AE, ::op_CB_AF,
        ::op_CB_B0, ::op_CB_B1, ::op_CB_B2, ::op_CB_B3, ::op_CB_B4, ::op_CB_B5, ::op_CB_B6, ::op_CB_B7,
        ::op_CB_B8, ::op_CB_B9, ::op_CB_BA, ::op_CB_BB, ::op_CB_BC, ::op_CB_BD, ::op_CB_BE, ::op_CB_BF,
        ::op_CB_C0, ::op_CB_C1, ::op_CB_C2, ::op_CB_C3, ::op_CB_C4, ::op_CB_C5, ::op_CB_C6, ::op_CB_C7,
        ::op_CB_C8, ::op_CB_C9, ::op_CB_CA, ::op_CB_CB, ::op_CB_CC, ::op_CB_CD, ::op_CB_CE, ::op_CB_CF,
        ::op_CB_D0, ::op_CB_D1, ::op_CB_D2, ::op_CB_D3, ::op_CB_D4, ::op_CB_D5, ::op_CB_D6, ::op_CB_D7,
        ::op_CB_D8, ::op_CB_D9, ::op_CB_DA, ::op_CB_DB, ::op_CB_DC, ::op_CB_DD, ::op_CB_DE, ::op_CB_DF,
        ::op_CB_E0, ::op_CB_E1, ::op_CB_E2, ::op_CB_E3, ::op_CB_E4, ::op_CB_E5, ::op_CB_E6, ::op_CB_E7,
        ::op_CB_E8, ::op_CB_E9, ::op_CB_EA, ::op_CB_EB, ::op_CB_EC, ::op_CB_ED, ::op_CB_EE, ::op_CB_EF,
        ::op_CB_F0, ::op_CB_F1, ::op_CB_F2, ::op_CB_F3, ::op_CB_F4, ::op_CB_F5, ::op_CB_F6, ::op_CB_F7,
        ::op_CB_F8, ::op_CB_F9, ::op_CB_FA, ::op_CB_FB, ::op_CB_FC, ::op_CB_FD, ::op_CB_FE, ::op_CB_FF
    };
#end
