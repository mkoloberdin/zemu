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
    OP_LD_DO_R_PORP (::op_DD_CB_00, DO_RLC_8, REG_B, REG_IX)    // LD B,RLC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_01, DO_RLC_8, REG_C, REG_IX)    // LD C,RLC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_02, DO_RLC_8, REG_D, REG_IX)    // LD D,RLC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_03, DO_RLC_8, REG_E, REG_IX)    // LD E,RLC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_04, DO_RLC_8, REG_H, REG_IX)    // LD H,RLC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_05, DO_RLC_8, REG_L, REG_IX)    // LD L,RLC (IX+N)
    OP_DO_PORP      (::op_DD_CB_06, DO_RLC_8, REG_IX)           // RLC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_07, DO_RLC_8, REG_A, REG_IX)    // LD A,RLC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_08, DO_RRC_8, REG_B, REG_IX)    // LD B,RRC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_09, DO_RRC_8, REG_C, REG_IX)    // LD C,RRC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_0A, DO_RRC_8, REG_D, REG_IX)    // LD D,RRC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_0B, DO_RRC_8, REG_E, REG_IX)    // LD E,RRC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_0C, DO_RRC_8, REG_H, REG_IX)    // LD H,RRC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_0D, DO_RRC_8, REG_L, REG_IX)    // LD L,RRC (IX+N)
    OP_DO_PORP      (::op_DD_CB_0E, DO_RRC_8, REG_IX)           // RRC (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_0F, DO_RRC_8, REG_A, REG_IX)    // LD A,RRC (IX+N)

    OP_LD_DO_R_PORP (::op_DD_CB_10, DO_RL_8, REG_B, REG_IX)     // LD B,RL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_11, DO_RL_8, REG_C, REG_IX)     // LD C,RL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_12, DO_RL_8, REG_D, REG_IX)     // LD D,RL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_13, DO_RL_8, REG_E, REG_IX)     // LD E,RL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_14, DO_RL_8, REG_H, REG_IX)     // LD H,RL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_15, DO_RL_8, REG_L, REG_IX)     // LD L,RL (IX+N)
    OP_DO_PORP      (::op_DD_CB_16, DO_RL_8, REG_IX)            // RL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_17, DO_RL_8, REG_A, REG_IX)     // LD A,RL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_18, DO_RR_8, REG_B, REG_IX)     // LD B,RR (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_19, DO_RR_8, REG_C, REG_IX)     // LD C,RR (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_1A, DO_RR_8, REG_D, REG_IX)     // LD D,RR (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_1B, DO_RR_8, REG_E, REG_IX)     // LD E,RR (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_1C, DO_RR_8, REG_H, REG_IX)     // LD H,RR (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_1D, DO_RR_8, REG_L, REG_IX)     // LD L,RR (IX+N)
    OP_DO_PORP      (::op_DD_CB_1E, DO_RR_8, REG_IX)            // RR (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_1F, DO_RR_8, REG_A, REG_IX)     // LD A,RR (IX+N)

    OP_LD_DO_R_PORP (::op_DD_CB_20, DO_SLA_8, REG_B, REG_IX)    // LD B,SLA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_21, DO_SLA_8, REG_C, REG_IX)    // LD C,SLA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_22, DO_SLA_8, REG_D, REG_IX)    // LD D,SLA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_23, DO_SLA_8, REG_E, REG_IX)    // LD E,SLA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_24, DO_SLA_8, REG_H, REG_IX)    // LD H,SLA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_25, DO_SLA_8, REG_L, REG_IX)    // LD L,SLA (IX+N)
    OP_DO_PORP      (::op_DD_CB_26, DO_SLA_8, REG_IX)           // SLA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_27, DO_SLA_8, REG_A, REG_IX)    // LD A,SLA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_28, DO_SRA_8, REG_B, REG_IX)    // LD B,SRA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_29, DO_SRA_8, REG_C, REG_IX)    // LD C,SRA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_2A, DO_SRA_8, REG_D, REG_IX)    // LD D,SRA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_2B, DO_SRA_8, REG_E, REG_IX)    // LD E,SRA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_2C, DO_SRA_8, REG_H, REG_IX)    // LD H,SRA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_2D, DO_SRA_8, REG_L, REG_IX)    // LD L,SRA (IX+N)
    OP_DO_PORP      (::op_DD_CB_2E, DO_SRA_8, REG_IX)           // SRA (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_2F, DO_SRA_8, REG_A, REG_IX)    // LD A,SRA (IX+N)

    OP_LD_DO_R_PORP (::op_DD_CB_30, DO_SLL_8, REG_B, REG_IX)    // LD B,SLL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_31, DO_SLL_8, REG_C, REG_IX)    // LD C,SLL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_32, DO_SLL_8, REG_D, REG_IX)    // LD D,SLL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_33, DO_SLL_8, REG_E, REG_IX)    // LD E,SLL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_34, DO_SLL_8, REG_H, REG_IX)    // LD H,SLL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_35, DO_SLL_8, REG_L, REG_IX)    // LD L,SLL (IX+N)
    OP_DO_PORP      (::op_DD_CB_36, DO_SLL_8, REG_IX)           // SLL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_37, DO_SLL_8, REG_A, REG_IX)    // LD A,SLL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_38, DO_SRL_8, REG_B, REG_IX)    // LD B,SRL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_39, DO_SRL_8, REG_C, REG_IX)    // LD C,SRL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_3A, DO_SRL_8, REG_D, REG_IX)    // LD D,SRL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_3B, DO_SRL_8, REG_E, REG_IX)    // LD E,SRL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_3C, DO_SRL_8, REG_H, REG_IX)    // LD H,SRL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_3D, DO_SRL_8, REG_L, REG_IX)    // LD L,SRL (IX+N)
    OP_DO_PORP      (::op_DD_CB_3E, DO_SRL_8, REG_IX)           // SRL (IX+N)
    OP_LD_DO_R_PORP (::op_DD_CB_3F, DO_SRL_8, REG_A, REG_IX)    // LD A,SRL (IX+N)

    #define          ::op_DD_CB_40 ::op_DD_CB_46
    #define          ::op_DD_CB_41 ::op_DD_CB_46
    #define          ::op_DD_CB_42 ::op_DD_CB_46
    #define          ::op_DD_CB_43 ::op_DD_CB_46
    #define          ::op_DD_CB_44 ::op_DD_CB_46
    #define          ::op_DD_CB_45 ::op_DD_CB_46
    OP_BIT_PORP     (::op_DD_CB_46, 0, REG_IX)                  // BIT 0,(IX+N)
    #define          ::op_DD_CB_47 ::op_DD_CB_46
    #define          ::op_DD_CB_48 ::op_DD_CB_4E
    #define          ::op_DD_CB_49 ::op_DD_CB_4E
    #define          ::op_DD_CB_4A ::op_DD_CB_4E
    #define          ::op_DD_CB_4B ::op_DD_CB_4E
    #define          ::op_DD_CB_4C ::op_DD_CB_4E
    #define          ::op_DD_CB_4D ::op_DD_CB_4E
    OP_BIT_PORP     (::op_DD_CB_4E, 1, REG_IX)                  // BIT 1,(IX+N)
    #define          ::op_DD_CB_4F ::op_DD_CB_4E

    #define          ::op_DD_CB_50 ::op_DD_CB_56
    #define          ::op_DD_CB_51 ::op_DD_CB_56
    #define          ::op_DD_CB_52 ::op_DD_CB_56
    #define          ::op_DD_CB_53 ::op_DD_CB_56
    #define          ::op_DD_CB_54 ::op_DD_CB_56
    #define          ::op_DD_CB_55 ::op_DD_CB_56
    OP_BIT_PORP     (::op_DD_CB_56, 2, REG_IX)                  // BIT 2,(IX+N)
    #define          ::op_DD_CB_57 ::op_DD_CB_56
    #define          ::op_DD_CB_58 ::op_DD_CB_5E
    #define          ::op_DD_CB_59 ::op_DD_CB_5E
    #define          ::op_DD_CB_5A ::op_DD_CB_5E
    #define          ::op_DD_CB_5B ::op_DD_CB_5E
    #define          ::op_DD_CB_5C ::op_DD_CB_5E
    #define          ::op_DD_CB_5D ::op_DD_CB_5E
    OP_BIT_PORP     (::op_DD_CB_5E, 3, REG_IX)                  // BIT 3,(IX+N)
    #define          ::op_DD_CB_5F ::op_DD_CB_5E

    #define          ::op_DD_CB_60 ::op_DD_CB_66
    #define          ::op_DD_CB_61 ::op_DD_CB_66
    #define          ::op_DD_CB_62 ::op_DD_CB_66
    #define          ::op_DD_CB_63 ::op_DD_CB_66
    #define          ::op_DD_CB_64 ::op_DD_CB_66
    #define          ::op_DD_CB_65 ::op_DD_CB_66
    OP_BIT_PORP     (::op_DD_CB_66, 4, REG_IX)                  // BIT 4,(IX+N)
    #define          ::op_DD_CB_67 ::op_DD_CB_66
    #define          ::op_DD_CB_68 ::op_DD_CB_6E
    #define          ::op_DD_CB_69 ::op_DD_CB_6E
    #define          ::op_DD_CB_6A ::op_DD_CB_6E
    #define          ::op_DD_CB_6B ::op_DD_CB_6E
    #define          ::op_DD_CB_6C ::op_DD_CB_6E
    #define          ::op_DD_CB_6D ::op_DD_CB_6E
    OP_BIT_PORP     (::op_DD_CB_6E, 5, REG_IX)                  // BIT 5,(IX+N)
    #define          ::op_DD_CB_6F ::op_DD_CB_6E

    #define          ::op_DD_CB_70 ::op_DD_CB_76
    #define          ::op_DD_CB_71 ::op_DD_CB_76
    #define          ::op_DD_CB_72 ::op_DD_CB_76
    #define          ::op_DD_CB_73 ::op_DD_CB_76
    #define          ::op_DD_CB_74 ::op_DD_CB_76
    #define          ::op_DD_CB_75 ::op_DD_CB_76
    OP_BIT_PORP     (::op_DD_CB_76, 6, REG_IX)                  // BIT 6,(IX+N)
    #define          ::op_DD_CB_77 ::op_DD_CB_76
    #define          ::op_DD_CB_78 ::op_DD_CB_7E
    #define          ::op_DD_CB_79 ::op_DD_CB_7E
    #define          ::op_DD_CB_7A ::op_DD_CB_7E
    #define          ::op_DD_CB_7B ::op_DD_CB_7E
    #define          ::op_DD_CB_7C ::op_DD_CB_7E
    #define          ::op_DD_CB_7D ::op_DD_CB_7E
    OP_BIT_PORP     (::op_DD_CB_7E, 7, REG_IX)                  // BIT 7,(IX+N)
    #define          ::op_DD_CB_7F ::op_DD_CB_7E

    OP_LD_RES_PORP  (::op_DD_CB_80, 0, REG_B, REG_IX)           // LD B,RES 0,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_81, 0, REG_C, REG_IX)           // LD C,RES 0,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_82, 0, REG_D, REG_IX)           // LD D,RES 0,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_83, 0, REG_E, REG_IX)           // LD E,RES 0,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_84, 0, REG_H, REG_IX)           // LD H,RES 0,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_85, 0, REG_L, REG_IX)           // LD L,RES 0,(IX+N)
    OP_RES_PORP     (::op_DD_CB_86, 0, REG_IX)                  // RES 0,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_87, 0, REG_A, REG_IX)           // LD A,RES 0,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_88, 1, REG_B, REG_IX)           // LD B,RES 1,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_89, 1, REG_C, REG_IX)           // LD C,RES 1,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_8A, 1, REG_D, REG_IX)           // LD D,RES 1,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_8B, 1, REG_E, REG_IX)           // LD E,RES 1,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_8C, 1, REG_H, REG_IX)           // LD H,RES 1,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_8D, 1, REG_L, REG_IX)           // LD L,RES 1,(IX+N)
    OP_RES_PORP     (::op_DD_CB_8E, 1, REG_IX)                  // RES 1,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_8F, 1, REG_A, REG_IX)           // LD A,RES 1,(IX+N)

    OP_LD_RES_PORP  (::op_DD_CB_90, 2, REG_B, REG_IX)           // LD B,RES 2,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_91, 2, REG_C, REG_IX)           // LD C,RES 2,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_92, 2, REG_D, REG_IX)           // LD D,RES 2,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_93, 2, REG_E, REG_IX)           // LD E,RES 2,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_94, 2, REG_H, REG_IX)           // LD H,RES 2,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_95, 2, REG_L, REG_IX)           // LD L,RES 2,(IX+N)
    OP_RES_PORP     (::op_DD_CB_96, 2, REG_IX)                  // RES 2,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_97, 2, REG_A, REG_IX)           // LD A,RES 2,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_98, 3, REG_B, REG_IX)           // LD B,RES 3,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_99, 3, REG_C, REG_IX)           // LD C,RES 3,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_9A, 3, REG_D, REG_IX)           // LD D,RES 3,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_9B, 3, REG_E, REG_IX)           // LD E,RES 3,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_9C, 3, REG_H, REG_IX)           // LD H,RES 3,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_9D, 3, REG_L, REG_IX)           // LD L,RES 3,(IX+N)
    OP_RES_PORP     (::op_DD_CB_9E, 3, REG_IX)                  // RES 3,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_9F, 3, REG_A, REG_IX)           // LD A,RES 3,(IX+N)

    OP_LD_RES_PORP  (::op_DD_CB_A0, 4, REG_B, REG_IX)           // LD B,RES 4,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_A1, 4, REG_C, REG_IX)           // LD C,RES 4,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_A2, 4, REG_D, REG_IX)           // LD D,RES 4,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_A3, 4, REG_E, REG_IX)           // LD E,RES 4,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_A4, 4, REG_H, REG_IX)           // LD H,RES 4,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_A5, 4, REG_L, REG_IX)           // LD L,RES 4,(IX+N)
    OP_RES_PORP     (::op_DD_CB_A6, 4, REG_IX)                  // RES 4,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_A7, 4, REG_A, REG_IX)           // LD A,RES 4,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_A8, 5, REG_B, REG_IX)           // LD B,RES 5,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_A9, 5, REG_C, REG_IX)           // LD C,RES 5,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_AA, 5, REG_D, REG_IX)           // LD D,RES 5,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_AB, 5, REG_E, REG_IX)           // LD E,RES 5,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_AC, 5, REG_H, REG_IX)           // LD H,RES 5,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_AD, 5, REG_L, REG_IX)           // LD L,RES 5,(IX+N)
    OP_RES_PORP     (::op_DD_CB_AE, 5, REG_IX)                  // RES 5,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_AF, 5, REG_A, REG_IX)           // LD A,RES 5,(IX+N)

    OP_LD_RES_PORP  (::op_DD_CB_B0, 6, REG_B, REG_IX)           // LD B,RES 6,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_B1, 6, REG_C, REG_IX)           // LD C,RES 6,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_B2, 6, REG_D, REG_IX)           // LD D,RES 6,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_B3, 6, REG_E, REG_IX)           // LD E,RES 6,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_B4, 6, REG_H, REG_IX)           // LD H,RES 6,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_B5, 6, REG_L, REG_IX)           // LD L,RES 6,(IX+N)
    OP_RES_PORP     (::op_DD_CB_B6, 6, REG_IX)                  // RES 6,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_B7, 6, REG_A, REG_IX)           // LD A,RES 6,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_B8, 7, REG_B, REG_IX)           // LD B,RES 7,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_B9, 7, REG_C, REG_IX)           // LD C,RES 7,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_BA, 7, REG_D, REG_IX)           // LD D,RES 7,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_BB, 7, REG_E, REG_IX)           // LD E,RES 7,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_BC, 7, REG_H, REG_IX)           // LD H,RES 7,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_BD, 7, REG_L, REG_IX)           // LD L,RES 7,(IX+N)
    OP_RES_PORP     (::op_DD_CB_BE, 7, REG_IX)                  // RES 7,(IX+N)
    OP_LD_RES_PORP  (::op_DD_CB_BF, 7, REG_A, REG_IX)           // LD A,RES 7,(IX+N)

    OP_LD_SET_PORP  (::op_DD_CB_C0, 0, REG_B, REG_IX)           // LD B,SET 0,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_C1, 0, REG_C, REG_IX)           // LD C,SET 0,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_C2, 0, REG_D, REG_IX)           // LD D,SET 0,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_C3, 0, REG_E, REG_IX)           // LD E,SET 0,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_C4, 0, REG_H, REG_IX)           // LD H,SET 0,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_C5, 0, REG_L, REG_IX)           // LD L,SET 0,(IX+N)
    OP_SET_PORP     (::op_DD_CB_C6, 0, REG_IX)                  // SET 0,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_C7, 0, REG_A, REG_IX)           // LD A,SET 0,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_C8, 1, REG_B, REG_IX)           // LD B,SET 1,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_C9, 1, REG_C, REG_IX)           // LD C,SET 1,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_CA, 1, REG_D, REG_IX)           // LD D,SET 1,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_CB, 1, REG_E, REG_IX)           // LD E,SET 1,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_CC, 1, REG_H, REG_IX)           // LD H,SET 1,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_CD, 1, REG_L, REG_IX)           // LD L,SET 1,(IX+N)
    OP_SET_PORP     (::op_DD_CB_CE, 1, REG_IX)                  // SET 1,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_CF, 1, REG_A, REG_IX)           // LD A,SET 1,(IX+N)

    OP_LD_SET_PORP  (::op_DD_CB_D0, 2, REG_B, REG_IX)           // LD B,SET 2,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_D1, 2, REG_C, REG_IX)           // LD C,SET 2,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_D2, 2, REG_D, REG_IX)           // LD D,SET 2,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_D3, 2, REG_E, REG_IX)           // LD E,SET 2,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_D4, 2, REG_H, REG_IX)           // LD H,SET 2,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_D5, 2, REG_L, REG_IX)           // LD L,SET 2,(IX+N)
    OP_SET_PORP     (::op_DD_CB_D6, 2, REG_IX)                  // SET 2,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_D7, 2, REG_A, REG_IX)           // LD A,SET 2,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_D8, 3, REG_B, REG_IX)           // LD B,SET 3,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_D9, 3, REG_C, REG_IX)           // LD C,SET 3,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_DA, 3, REG_D, REG_IX)           // LD D,SET 3,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_DB, 3, REG_E, REG_IX)           // LD E,SET 3,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_DC, 3, REG_H, REG_IX)           // LD H,SET 3,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_DD, 3, REG_L, REG_IX)           // LD L,SET 3,(IX+N)
    OP_SET_PORP     (::op_DD_CB_DE, 3, REG_IX)                  // SET 3,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_DF, 3, REG_A, REG_IX)           // LD A,SET 3,(IX+N)

    OP_LD_SET_PORP  (::op_DD_CB_E0, 4, REG_B, REG_IX)           // LD B,SET 4,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_E1, 4, REG_C, REG_IX)           // LD C,SET 4,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_E2, 4, REG_D, REG_IX)           // LD D,SET 4,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_E3, 4, REG_E, REG_IX)           // LD E,SET 4,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_E4, 4, REG_H, REG_IX)           // LD H,SET 4,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_E5, 4, REG_L, REG_IX)           // LD L,SET 4,(IX+N)
    OP_SET_PORP     (::op_DD_CB_E6, 4, REG_IX)                  // SET 4,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_E7, 4, REG_A, REG_IX)           // LD A,SET 4,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_E8, 5, REG_B, REG_IX)           // LD B,SET 5,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_E9, 5, REG_C, REG_IX)           // LD C,SET 5,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_EA, 5, REG_D, REG_IX)           // LD D,SET 5,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_EB, 5, REG_E, REG_IX)           // LD E,SET 5,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_EC, 5, REG_H, REG_IX)           // LD H,SET 5,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_ED, 5, REG_L, REG_IX)           // LD L,SET 5,(IX+N)
    OP_SET_PORP     (::op_DD_CB_EE, 5, REG_IX)                  // SET 5,(IX+N
    OP_LD_SET_PORP  (::op_DD_CB_EF, 5, REG_A, REG_IX)           // LD A,SET 5,(IX+N)

    OP_LD_SET_PORP  (::op_DD_CB_F0, 6, REG_B, REG_IX)           // LD B,SET 6,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_F1, 6, REG_C, REG_IX)           // LD C,SET 6,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_F2, 6, REG_D, REG_IX)           // LD D,SET 6,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_F3, 6, REG_E, REG_IX)           // LD E,SET 6,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_F4, 6, REG_H, REG_IX)           // LD H,SET 6,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_F5, 6, REG_L, REG_IX)           // LD L,SET 6,(IX+N)
    OP_SET_PORP     (::op_DD_CB_F6, 6, REG_IX)                  // SET 6,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_F7, 6, REG_A, REG_IX)           // LD A,SET 6,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_F8, 7, REG_B, REG_IX)           // LD B,SET 7,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_F9, 7, REG_C, REG_IX)           // LD C,SET 7,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_FA, 7, REG_D, REG_IX)           // LD D,SET 7,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_FB, 7, REG_E, REG_IX)           // LD E,SET 7,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_FC, 7, REG_H, REG_IX)           // LD H,SET 7,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_FD, 7, REG_L, REG_IX)           // LD L,SET 7,(IX+N)
    OP_SET_PORP     (::op_DD_CB_FE, 7, REG_IX)                  // SET 7,(IX+N)
    OP_LD_SET_PORP  (::op_DD_CB_FF, 7, REG_A, REG_IX)           // LD A,SET 7,(IX+N)

    ::t_opcode optable_DD_CB[0x100] = {
        ::op_DD_CB_00, ::op_DD_CB_01, ::op_DD_CB_02, ::op_DD_CB_03, ::op_DD_CB_04, ::op_DD_CB_05, ::op_DD_CB_06, ::op_DD_CB_07,
        ::op_DD_CB_08, ::op_DD_CB_09, ::op_DD_CB_0A, ::op_DD_CB_0B, ::op_DD_CB_0C, ::op_DD_CB_0D, ::op_DD_CB_0E, ::op_DD_CB_0F,
        ::op_DD_CB_10, ::op_DD_CB_11, ::op_DD_CB_12, ::op_DD_CB_13, ::op_DD_CB_14, ::op_DD_CB_15, ::op_DD_CB_16, ::op_DD_CB_17,
        ::op_DD_CB_18, ::op_DD_CB_19, ::op_DD_CB_1A, ::op_DD_CB_1B, ::op_DD_CB_1C, ::op_DD_CB_1D, ::op_DD_CB_1E, ::op_DD_CB_1F,
        ::op_DD_CB_20, ::op_DD_CB_21, ::op_DD_CB_22, ::op_DD_CB_23, ::op_DD_CB_24, ::op_DD_CB_25, ::op_DD_CB_26, ::op_DD_CB_27,
        ::op_DD_CB_28, ::op_DD_CB_29, ::op_DD_CB_2A, ::op_DD_CB_2B, ::op_DD_CB_2C, ::op_DD_CB_2D, ::op_DD_CB_2E, ::op_DD_CB_2F,
        ::op_DD_CB_30, ::op_DD_CB_31, ::op_DD_CB_32, ::op_DD_CB_33, ::op_DD_CB_34, ::op_DD_CB_35, ::op_DD_CB_36, ::op_DD_CB_37,
        ::op_DD_CB_38, ::op_DD_CB_39, ::op_DD_CB_3A, ::op_DD_CB_3B, ::op_DD_CB_3C, ::op_DD_CB_3D, ::op_DD_CB_3E, ::op_DD_CB_3F,
        ::op_DD_CB_40, ::op_DD_CB_41, ::op_DD_CB_42, ::op_DD_CB_43, ::op_DD_CB_44, ::op_DD_CB_45, ::op_DD_CB_46, ::op_DD_CB_47,
        ::op_DD_CB_48, ::op_DD_CB_49, ::op_DD_CB_4A, ::op_DD_CB_4B, ::op_DD_CB_4C, ::op_DD_CB_4D, ::op_DD_CB_4E, ::op_DD_CB_4F,
        ::op_DD_CB_50, ::op_DD_CB_51, ::op_DD_CB_52, ::op_DD_CB_53, ::op_DD_CB_54, ::op_DD_CB_55, ::op_DD_CB_56, ::op_DD_CB_57,
        ::op_DD_CB_58, ::op_DD_CB_59, ::op_DD_CB_5A, ::op_DD_CB_5B, ::op_DD_CB_5C, ::op_DD_CB_5D, ::op_DD_CB_5E, ::op_DD_CB_5F,
        ::op_DD_CB_60, ::op_DD_CB_61, ::op_DD_CB_62, ::op_DD_CB_63, ::op_DD_CB_64, ::op_DD_CB_65, ::op_DD_CB_66, ::op_DD_CB_67,
        ::op_DD_CB_68, ::op_DD_CB_69, ::op_DD_CB_6A, ::op_DD_CB_6B, ::op_DD_CB_6C, ::op_DD_CB_6D, ::op_DD_CB_6E, ::op_DD_CB_6F,
        ::op_DD_CB_70, ::op_DD_CB_71, ::op_DD_CB_72, ::op_DD_CB_73, ::op_DD_CB_74, ::op_DD_CB_75, ::op_DD_CB_76, ::op_DD_CB_77,
        ::op_DD_CB_78, ::op_DD_CB_79, ::op_DD_CB_7A, ::op_DD_CB_7B, ::op_DD_CB_7C, ::op_DD_CB_7D, ::op_DD_CB_7E, ::op_DD_CB_7F,
        ::op_DD_CB_80, ::op_DD_CB_81, ::op_DD_CB_82, ::op_DD_CB_83, ::op_DD_CB_84, ::op_DD_CB_85, ::op_DD_CB_86, ::op_DD_CB_87,
        ::op_DD_CB_88, ::op_DD_CB_89, ::op_DD_CB_8A, ::op_DD_CB_8B, ::op_DD_CB_8C, ::op_DD_CB_8D, ::op_DD_CB_8E, ::op_DD_CB_8F,
        ::op_DD_CB_90, ::op_DD_CB_91, ::op_DD_CB_92, ::op_DD_CB_93, ::op_DD_CB_94, ::op_DD_CB_95, ::op_DD_CB_96, ::op_DD_CB_97,
        ::op_DD_CB_98, ::op_DD_CB_99, ::op_DD_CB_9A, ::op_DD_CB_9B, ::op_DD_CB_9C, ::op_DD_CB_9D, ::op_DD_CB_9E, ::op_DD_CB_9F,
        ::op_DD_CB_A0, ::op_DD_CB_A1, ::op_DD_CB_A2, ::op_DD_CB_A3, ::op_DD_CB_A4, ::op_DD_CB_A5, ::op_DD_CB_A6, ::op_DD_CB_A7,
        ::op_DD_CB_A8, ::op_DD_CB_A9, ::op_DD_CB_AA, ::op_DD_CB_AB, ::op_DD_CB_AC, ::op_DD_CB_AD, ::op_DD_CB_AE, ::op_DD_CB_AF,
        ::op_DD_CB_B0, ::op_DD_CB_B1, ::op_DD_CB_B2, ::op_DD_CB_B3, ::op_DD_CB_B4, ::op_DD_CB_B5, ::op_DD_CB_B6, ::op_DD_CB_B7,
        ::op_DD_CB_B8, ::op_DD_CB_B9, ::op_DD_CB_BA, ::op_DD_CB_BB, ::op_DD_CB_BC, ::op_DD_CB_BD, ::op_DD_CB_BE, ::op_DD_CB_BF,
        ::op_DD_CB_C0, ::op_DD_CB_C1, ::op_DD_CB_C2, ::op_DD_CB_C3, ::op_DD_CB_C4, ::op_DD_CB_C5, ::op_DD_CB_C6, ::op_DD_CB_C7,
        ::op_DD_CB_C8, ::op_DD_CB_C9, ::op_DD_CB_CA, ::op_DD_CB_CB, ::op_DD_CB_CC, ::op_DD_CB_CD, ::op_DD_CB_CE, ::op_DD_CB_CF,
        ::op_DD_CB_D0, ::op_DD_CB_D1, ::op_DD_CB_D2, ::op_DD_CB_D3, ::op_DD_CB_D4, ::op_DD_CB_D5, ::op_DD_CB_D6, ::op_DD_CB_D7,
        ::op_DD_CB_D8, ::op_DD_CB_D9, ::op_DD_CB_DA, ::op_DD_CB_DB, ::op_DD_CB_DC, ::op_DD_CB_DD, ::op_DD_CB_DE, ::op_DD_CB_DF,
        ::op_DD_CB_E0, ::op_DD_CB_E1, ::op_DD_CB_E2, ::op_DD_CB_E3, ::op_DD_CB_E4, ::op_DD_CB_E5, ::op_DD_CB_E6, ::op_DD_CB_E7,
        ::op_DD_CB_E8, ::op_DD_CB_E9, ::op_DD_CB_EA, ::op_DD_CB_EB, ::op_DD_CB_EC, ::op_DD_CB_ED, ::op_DD_CB_EE, ::op_DD_CB_EF,
        ::op_DD_CB_F0, ::op_DD_CB_F1, ::op_DD_CB_F2, ::op_DD_CB_F3, ::op_DD_CB_F4, ::op_DD_CB_F5, ::op_DD_CB_F6, ::op_DD_CB_F7,
        ::op_DD_CB_F8, ::op_DD_CB_F9, ::op_DD_CB_FA, ::op_DD_CB_FB, ::op_DD_CB_FC, ::op_DD_CB_FD, ::op_DD_CB_FE, ::op_DD_CB_FF
    };
#end
