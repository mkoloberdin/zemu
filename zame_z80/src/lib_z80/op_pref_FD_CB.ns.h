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
    OP_LD_DO_R_PORP (::op_FD_CB_00, DO_RLC_8, REG_B, REG_IY)    // LD B,RLC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_01, DO_RLC_8, REG_C, REG_IY)    // LD C,RLC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_02, DO_RLC_8, REG_D, REG_IY)    // LD D,RLC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_03, DO_RLC_8, REG_E, REG_IY)    // LD E,RLC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_04, DO_RLC_8, REG_H, REG_IY)    // LD H,RLC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_05, DO_RLC_8, REG_L, REG_IY)    // LD L,RLC (IY+N)
    OP_DO_PORP      (::op_FD_CB_06, DO_RLC_8, REG_IY)           // RLC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_07, DO_RLC_8, REG_A, REG_IY)    // LD A,RLC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_08, DO_RRC_8, REG_B, REG_IY)    // LD B,RRC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_09, DO_RRC_8, REG_C, REG_IY)    // LD C,RRC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_0A, DO_RRC_8, REG_D, REG_IY)    // LD D,RRC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_0B, DO_RRC_8, REG_E, REG_IY)    // LD E,RRC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_0C, DO_RRC_8, REG_H, REG_IY)    // LD H,RRC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_0D, DO_RRC_8, REG_L, REG_IY)    // LD L,RRC (IY+N)
    OP_DO_PORP      (::op_FD_CB_0E, DO_RRC_8, REG_IY)           // RRC (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_0F, DO_RRC_8, REG_A, REG_IY)    // LD A,RRC (IY+N)

    OP_LD_DO_R_PORP (::op_FD_CB_10, DO_RL_8, REG_B, REG_IY)     // LD B,RL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_11, DO_RL_8, REG_C, REG_IY)     // LD C,RL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_12, DO_RL_8, REG_D, REG_IY)     // LD D,RL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_13, DO_RL_8, REG_E, REG_IY)     // LD E,RL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_14, DO_RL_8, REG_H, REG_IY)     // LD H,RL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_15, DO_RL_8, REG_L, REG_IY)     // LD L,RL (IY+N)
    OP_DO_PORP      (::op_FD_CB_16, DO_RL_8, REG_IY)            // RL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_17, DO_RL_8, REG_A, REG_IY)     // LD A,RL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_18, DO_RR_8, REG_B, REG_IY)     // LD B,RR (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_19, DO_RR_8, REG_C, REG_IY)     // LD C,RR (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_1A, DO_RR_8, REG_D, REG_IY)     // LD D,RR (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_1B, DO_RR_8, REG_E, REG_IY)     // LD E,RR (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_1C, DO_RR_8, REG_H, REG_IY)     // LD H,RR (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_1D, DO_RR_8, REG_L, REG_IY)     // LD L,RR (IY+N)
    OP_DO_PORP      (::op_FD_CB_1E, DO_RR_8, REG_IY)            // RR (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_1F, DO_RR_8, REG_A, REG_IY)     // LD A,RR (IY+N)

    OP_LD_DO_R_PORP (::op_FD_CB_20, DO_SLA_8, REG_B, REG_IY)    // LD B,SLA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_21, DO_SLA_8, REG_C, REG_IY)    // LD C,SLA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_22, DO_SLA_8, REG_D, REG_IY)    // LD D,SLA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_23, DO_SLA_8, REG_E, REG_IY)    // LD E,SLA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_24, DO_SLA_8, REG_H, REG_IY)    // LD H,SLA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_25, DO_SLA_8, REG_L, REG_IY)    // LD L,SLA (IY+N)
    OP_DO_PORP      (::op_FD_CB_26, DO_SLA_8, REG_IY)           // SLA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_27, DO_SLA_8, REG_A, REG_IY)    // LD A,SLA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_28, DO_SRA_8, REG_B, REG_IY)    // LD B,SRA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_29, DO_SRA_8, REG_C, REG_IY)    // LD C,SRA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_2A, DO_SRA_8, REG_D, REG_IY)    // LD D,SRA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_2B, DO_SRA_8, REG_E, REG_IY)    // LD E,SRA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_2C, DO_SRA_8, REG_H, REG_IY)    // LD H,SRA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_2D, DO_SRA_8, REG_L, REG_IY)    // LD L,SRA (IY+N)
    OP_DO_PORP      (::op_FD_CB_2E, DO_SRA_8, REG_IY)           // SRA (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_2F, DO_SRA_8, REG_A, REG_IY)    // LD A,SRA (IY+N)

    OP_LD_DO_R_PORP (::op_FD_CB_30, DO_SLL_8, REG_B, REG_IY)    // LD B,SLL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_31, DO_SLL_8, REG_C, REG_IY)    // LD C,SLL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_32, DO_SLL_8, REG_D, REG_IY)    // LD D,SLL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_33, DO_SLL_8, REG_E, REG_IY)    // LD E,SLL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_34, DO_SLL_8, REG_H, REG_IY)    // LD H,SLL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_35, DO_SLL_8, REG_L, REG_IY)    // LD L,SLL (IY+N)
    OP_DO_PORP      (::op_FD_CB_36, DO_SLL_8, REG_IY)           // SLL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_37, DO_SLL_8, REG_A, REG_IY)    // LD A,SLL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_38, DO_SRL_8, REG_B, REG_IY)    // LD B,SRL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_39, DO_SRL_8, REG_C, REG_IY)    // LD C,SRL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_3A, DO_SRL_8, REG_D, REG_IY)    // LD D,SRL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_3B, DO_SRL_8, REG_E, REG_IY)    // LD E,SRL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_3C, DO_SRL_8, REG_H, REG_IY)    // LD H,SRL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_3D, DO_SRL_8, REG_L, REG_IY)    // LD L,SRL (IY+N)
    OP_DO_PORP      (::op_FD_CB_3E, DO_SRL_8, REG_IY)           // SRL (IY+N)
    OP_LD_DO_R_PORP (::op_FD_CB_3F, DO_SRL_8, REG_A, REG_IY)    // LD A,SRL (IY+N)

    #define          ::op_FD_CB_40 ::op_FD_CB_46
    #define          ::op_FD_CB_41 ::op_FD_CB_46
    #define          ::op_FD_CB_42 ::op_FD_CB_46
    #define          ::op_FD_CB_43 ::op_FD_CB_46
    #define          ::op_FD_CB_44 ::op_FD_CB_46
    #define          ::op_FD_CB_45 ::op_FD_CB_46
    OP_BIT_PORP     (::op_FD_CB_46, 0, REG_IY)                  // BIT 0,(IY+N)
    #define          ::op_FD_CB_47 ::op_FD_CB_46
    #define          ::op_FD_CB_48 ::op_FD_CB_4E
    #define          ::op_FD_CB_49 ::op_FD_CB_4E
    #define          ::op_FD_CB_4A ::op_FD_CB_4E
    #define          ::op_FD_CB_4B ::op_FD_CB_4E
    #define          ::op_FD_CB_4C ::op_FD_CB_4E
    #define          ::op_FD_CB_4D ::op_FD_CB_4E
    OP_BIT_PORP     (::op_FD_CB_4E, 1, REG_IY)                  // BIT 1,(IY+N)
    #define          ::op_FD_CB_4F ::op_FD_CB_4E

    #define          ::op_FD_CB_50 ::op_FD_CB_56
    #define          ::op_FD_CB_51 ::op_FD_CB_56
    #define          ::op_FD_CB_52 ::op_FD_CB_56
    #define          ::op_FD_CB_53 ::op_FD_CB_56
    #define          ::op_FD_CB_54 ::op_FD_CB_56
    #define          ::op_FD_CB_55 ::op_FD_CB_56
    OP_BIT_PORP     (::op_FD_CB_56, 2, REG_IY)                  // BIT 2,(IY+N)
    #define          ::op_FD_CB_57 ::op_FD_CB_56
    #define          ::op_FD_CB_58 ::op_FD_CB_5E
    #define          ::op_FD_CB_59 ::op_FD_CB_5E
    #define          ::op_FD_CB_5A ::op_FD_CB_5E
    #define          ::op_FD_CB_5B ::op_FD_CB_5E
    #define          ::op_FD_CB_5C ::op_FD_CB_5E
    #define          ::op_FD_CB_5D ::op_FD_CB_5E
    OP_BIT_PORP     (::op_FD_CB_5E, 3, REG_IY)                  // BIT 3,(IY+N)
    #define          ::op_FD_CB_5F ::op_FD_CB_5E

    #define          ::op_FD_CB_60 ::op_FD_CB_66
    #define          ::op_FD_CB_61 ::op_FD_CB_66
    #define          ::op_FD_CB_62 ::op_FD_CB_66
    #define          ::op_FD_CB_63 ::op_FD_CB_66
    #define          ::op_FD_CB_64 ::op_FD_CB_66
    #define          ::op_FD_CB_65 ::op_FD_CB_66
    OP_BIT_PORP     (::op_FD_CB_66, 4, REG_IY)                  // BIT 4,(IY+N)
    #define          ::op_FD_CB_67 ::op_FD_CB_66
    #define          ::op_FD_CB_68 ::op_FD_CB_6E
    #define          ::op_FD_CB_69 ::op_FD_CB_6E
    #define          ::op_FD_CB_6A ::op_FD_CB_6E
    #define          ::op_FD_CB_6B ::op_FD_CB_6E
    #define          ::op_FD_CB_6C ::op_FD_CB_6E
    #define          ::op_FD_CB_6D ::op_FD_CB_6E
    OP_BIT_PORP     (::op_FD_CB_6E, 5, REG_IY)                  // BIT 5,(IY+N)
    #define          ::op_FD_CB_6F ::op_FD_CB_6E

    #define          ::op_FD_CB_70 ::op_FD_CB_76
    #define          ::op_FD_CB_71 ::op_FD_CB_76
    #define          ::op_FD_CB_72 ::op_FD_CB_76
    #define          ::op_FD_CB_73 ::op_FD_CB_76
    #define          ::op_FD_CB_74 ::op_FD_CB_76
    #define          ::op_FD_CB_75 ::op_FD_CB_76
    OP_BIT_PORP     (::op_FD_CB_76, 6, REG_IY)                  // BIT 6,(IY+N)
    #define          ::op_FD_CB_77 ::op_FD_CB_76
    #define          ::op_FD_CB_78 ::op_FD_CB_7E
    #define          ::op_FD_CB_79 ::op_FD_CB_7E
    #define          ::op_FD_CB_7A ::op_FD_CB_7E
    #define          ::op_FD_CB_7B ::op_FD_CB_7E
    #define          ::op_FD_CB_7C ::op_FD_CB_7E
    #define          ::op_FD_CB_7D ::op_FD_CB_7E
    OP_BIT_PORP     (::op_FD_CB_7E, 7, REG_IY)                  // BIT 7,(IY+N)
    #define          ::op_FD_CB_7F ::op_FD_CB_7E

    OP_LD_RES_PORP  (::op_FD_CB_80, 0, REG_B, REG_IY)           // LD B,RES 0,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_81, 0, REG_C, REG_IY)           // LD C,RES 0,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_82, 0, REG_D, REG_IY)           // LD D,RES 0,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_83, 0, REG_E, REG_IY)           // LD E,RES 0,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_84, 0, REG_H, REG_IY)           // LD H,RES 0,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_85, 0, REG_L, REG_IY)           // LD L,RES 0,(IY+N)
    OP_RES_PORP     (::op_FD_CB_86, 0, REG_IY)                  // RES 0,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_87, 0, REG_A, REG_IY)           // LD A,RES 0,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_88, 1, REG_B, REG_IY)           // LD B,RES 1,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_89, 1, REG_C, REG_IY)           // LD C,RES 1,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_8A, 1, REG_D, REG_IY)           // LD D,RES 1,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_8B, 1, REG_E, REG_IY)           // LD E,RES 1,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_8C, 1, REG_H, REG_IY)           // LD H,RES 1,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_8D, 1, REG_L, REG_IY)           // LD L,RES 1,(IY+N)
    OP_RES_PORP     (::op_FD_CB_8E, 1, REG_IY)                  // RES 1,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_8F, 1, REG_A, REG_IY)           // LD A,RES 1,(IY+N)

    OP_LD_RES_PORP  (::op_FD_CB_90, 2, REG_B, REG_IY)           // LD B,RES 2,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_91, 2, REG_C, REG_IY)           // LD C,RES 2,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_92, 2, REG_D, REG_IY)           // LD D,RES 2,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_93, 2, REG_E, REG_IY)           // LD E,RES 2,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_94, 2, REG_H, REG_IY)           // LD H,RES 2,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_95, 2, REG_L, REG_IY)           // LD L,RES 2,(IY+N)
    OP_RES_PORP     (::op_FD_CB_96, 2, REG_IY)                  // RES 2,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_97, 2, REG_A, REG_IY)           // LD A,RES 2,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_98, 3, REG_B, REG_IY)           // LD B,RES 3,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_99, 3, REG_C, REG_IY)           // LD C,RES 3,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_9A, 3, REG_D, REG_IY)           // LD D,RES 3,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_9B, 3, REG_E, REG_IY)           // LD E,RES 3,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_9C, 3, REG_H, REG_IY)           // LD H,RES 3,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_9D, 3, REG_L, REG_IY)           // LD L,RES 3,(IY+N)
    OP_RES_PORP     (::op_FD_CB_9E, 3, REG_IY)                  // RES 3,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_9F, 3, REG_A, REG_IY)           // LD A,RES 3,(IY+N)

    OP_LD_RES_PORP  (::op_FD_CB_A0, 4, REG_B, REG_IY)           // LD B,RES 4,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_A1, 4, REG_C, REG_IY)           // LD C,RES 4,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_A2, 4, REG_D, REG_IY)           // LD D,RES 4,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_A3, 4, REG_E, REG_IY)           // LD E,RES 4,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_A4, 4, REG_H, REG_IY)           // LD H,RES 4,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_A5, 4, REG_L, REG_IY)           // LD L,RES 4,(IY+N)
    OP_RES_PORP     (::op_FD_CB_A6, 4, REG_IY)                  // RES 4,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_A7, 4, REG_A, REG_IY)           // LD A,RES 4,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_A8, 5, REG_B, REG_IY)           // LD B,RES 5,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_A9, 5, REG_C, REG_IY)           // LD C,RES 5,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_AA, 5, REG_D, REG_IY)           // LD D,RES 5,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_AB, 5, REG_E, REG_IY)           // LD E,RES 5,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_AC, 5, REG_H, REG_IY)           // LD H,RES 5,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_AD, 5, REG_L, REG_IY)           // LD L,RES 5,(IY+N)
    OP_RES_PORP     (::op_FD_CB_AE, 5, REG_IY)                  // RES 5,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_AF, 5, REG_A, REG_IY)           // LD A,RES 5,(IY+N)

    OP_LD_RES_PORP  (::op_FD_CB_B0, 6, REG_B, REG_IY)           // LD B,RES 6,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_B1, 6, REG_C, REG_IY)           // LD C,RES 6,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_B2, 6, REG_D, REG_IY)           // LD D,RES 6,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_B3, 6, REG_E, REG_IY)           // LD E,RES 6,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_B4, 6, REG_H, REG_IY)           // LD H,RES 6,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_B5, 6, REG_L, REG_IY)           // LD L,RES 6,(IY+N)
    OP_RES_PORP     (::op_FD_CB_B6, 6, REG_IY)                  // RES 6,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_B7, 6, REG_A, REG_IY)           // LD A,RES 6,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_B8, 7, REG_B, REG_IY)           // LD B,RES 7,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_B9, 7, REG_C, REG_IY)           // LD C,RES 7,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_BA, 7, REG_D, REG_IY)           // LD D,RES 7,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_BB, 7, REG_E, REG_IY)           // LD E,RES 7,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_BC, 7, REG_H, REG_IY)           // LD H,RES 7,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_BD, 7, REG_L, REG_IY)           // LD L,RES 7,(IY+N)
    OP_RES_PORP     (::op_FD_CB_BE, 7, REG_IY)                  // RES 7,(IY+N)
    OP_LD_RES_PORP  (::op_FD_CB_BF, 7, REG_A, REG_IY)           // LD A,RES 7,(IY+N)

    OP_LD_SET_PORP  (::op_FD_CB_C0, 0, REG_B, REG_IY)           // LD B,SET 0,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_C1, 0, REG_C, REG_IY)           // LD C,SET 0,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_C2, 0, REG_D, REG_IY)           // LD D,SET 0,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_C3, 0, REG_E, REG_IY)           // LD E,SET 0,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_C4, 0, REG_H, REG_IY)           // LD H,SET 0,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_C5, 0, REG_L, REG_IY)           // LD L,SET 0,(IY+N)
    OP_SET_PORP     (::op_FD_CB_C6, 0, REG_IY)                  // SET 0,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_C7, 0, REG_A, REG_IY)           // LD A,SET 0,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_C8, 1, REG_B, REG_IY)           // LD B,SET 1,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_C9, 1, REG_C, REG_IY)           // LD C,SET 1,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_CA, 1, REG_D, REG_IY)           // LD D,SET 1,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_CB, 1, REG_E, REG_IY)           // LD E,SET 1,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_CC, 1, REG_H, REG_IY)           // LD H,SET 1,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_CD, 1, REG_L, REG_IY)           // LD L,SET 1,(IY+N)
    OP_SET_PORP     (::op_FD_CB_CE, 1, REG_IY)                  // SET 1,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_CF, 1, REG_A, REG_IY)           // LD A,SET 1,(IY+N)

    OP_LD_SET_PORP  (::op_FD_CB_D0, 2, REG_B, REG_IY)           // LD B,SET 2,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_D1, 2, REG_C, REG_IY)           // LD C,SET 2,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_D2, 2, REG_D, REG_IY)           // LD D,SET 2,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_D3, 2, REG_E, REG_IY)           // LD E,SET 2,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_D4, 2, REG_H, REG_IY)           // LD H,SET 2,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_D5, 2, REG_L, REG_IY)           // LD L,SET 2,(IY+N)
    OP_SET_PORP     (::op_FD_CB_D6, 2, REG_IY)                  // SET 2,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_D7, 2, REG_A, REG_IY)           // LD A,SET 2,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_D8, 3, REG_B, REG_IY)           // LD B,SET 3,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_D9, 3, REG_C, REG_IY)           // LD C,SET 3,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_DA, 3, REG_D, REG_IY)           // LD D,SET 3,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_DB, 3, REG_E, REG_IY)           // LD E,SET 3,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_DC, 3, REG_H, REG_IY)           // LD H,SET 3,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_DD, 3, REG_L, REG_IY)           // LD L,SET 3,(IY+N)
    OP_SET_PORP     (::op_FD_CB_DE, 3, REG_IY)                  // SET 3,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_DF, 3, REG_A, REG_IY)           // LD A,SET 3,(IY+N)

    OP_LD_SET_PORP  (::op_FD_CB_E0, 4, REG_B, REG_IY)           // LD B,SET 4,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_E1, 4, REG_C, REG_IY)           // LD C,SET 4,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_E2, 4, REG_D, REG_IY)           // LD D,SET 4,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_E3, 4, REG_E, REG_IY)           // LD E,SET 4,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_E4, 4, REG_H, REG_IY)           // LD H,SET 4,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_E5, 4, REG_L, REG_IY)           // LD L,SET 4,(IY+N)
    OP_SET_PORP     (::op_FD_CB_E6, 4, REG_IY)                  // SET 4,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_E7, 4, REG_A, REG_IY)           // LD A,SET 4,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_E8, 5, REG_B, REG_IY)           // LD B,SET 5,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_E9, 5, REG_C, REG_IY)           // LD C,SET 5,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_EA, 5, REG_D, REG_IY)           // LD D,SET 5,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_EB, 5, REG_E, REG_IY)           // LD E,SET 5,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_EC, 5, REG_H, REG_IY)           // LD H,SET 5,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_ED, 5, REG_L, REG_IY)           // LD L,SET 5,(IY+N)
    OP_SET_PORP     (::op_FD_CB_EE, 5, REG_IY)                  // SET 5,(IY+N
    OP_LD_SET_PORP  (::op_FD_CB_EF, 5, REG_A, REG_IY)           // LD A,SET 5,(IY+N)

    OP_LD_SET_PORP  (::op_FD_CB_F0, 6, REG_B, REG_IY)           // LD B,SET 6,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_F1, 6, REG_C, REG_IY)           // LD C,SET 6,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_F2, 6, REG_D, REG_IY)           // LD D,SET 6,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_F3, 6, REG_E, REG_IY)           // LD E,SET 6,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_F4, 6, REG_H, REG_IY)           // LD H,SET 6,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_F5, 6, REG_L, REG_IY)           // LD L,SET 6,(IY+N)
    OP_SET_PORP     (::op_FD_CB_F6, 6, REG_IY)                  // SET 6,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_F7, 6, REG_A, REG_IY)           // LD A,SET 6,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_F8, 7, REG_B, REG_IY)           // LD B,SET 7,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_F9, 7, REG_C, REG_IY)           // LD C,SET 7,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_FA, 7, REG_D, REG_IY)           // LD D,SET 7,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_FB, 7, REG_E, REG_IY)           // LD E,SET 7,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_FC, 7, REG_H, REG_IY)           // LD H,SET 7,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_FD, 7, REG_L, REG_IY)           // LD L,SET 7,(IY+N)
    OP_SET_PORP     (::op_FD_CB_FE, 7, REG_IY)                  // SET 7,(IY+N)
    OP_LD_SET_PORP  (::op_FD_CB_FF, 7, REG_A, REG_IY)           // LD A,SET 7,(IY+N)

    ::t_opcode optable_FD_CB[0x100] = {
        ::op_FD_CB_00, ::op_FD_CB_01, ::op_FD_CB_02, ::op_FD_CB_03, ::op_FD_CB_04, ::op_FD_CB_05, ::op_FD_CB_06, ::op_FD_CB_07,
        ::op_FD_CB_08, ::op_FD_CB_09, ::op_FD_CB_0A, ::op_FD_CB_0B, ::op_FD_CB_0C, ::op_FD_CB_0D, ::op_FD_CB_0E, ::op_FD_CB_0F,
        ::op_FD_CB_10, ::op_FD_CB_11, ::op_FD_CB_12, ::op_FD_CB_13, ::op_FD_CB_14, ::op_FD_CB_15, ::op_FD_CB_16, ::op_FD_CB_17,
        ::op_FD_CB_18, ::op_FD_CB_19, ::op_FD_CB_1A, ::op_FD_CB_1B, ::op_FD_CB_1C, ::op_FD_CB_1D, ::op_FD_CB_1E, ::op_FD_CB_1F,
        ::op_FD_CB_20, ::op_FD_CB_21, ::op_FD_CB_22, ::op_FD_CB_23, ::op_FD_CB_24, ::op_FD_CB_25, ::op_FD_CB_26, ::op_FD_CB_27,
        ::op_FD_CB_28, ::op_FD_CB_29, ::op_FD_CB_2A, ::op_FD_CB_2B, ::op_FD_CB_2C, ::op_FD_CB_2D, ::op_FD_CB_2E, ::op_FD_CB_2F,
        ::op_FD_CB_30, ::op_FD_CB_31, ::op_FD_CB_32, ::op_FD_CB_33, ::op_FD_CB_34, ::op_FD_CB_35, ::op_FD_CB_36, ::op_FD_CB_37,
        ::op_FD_CB_38, ::op_FD_CB_39, ::op_FD_CB_3A, ::op_FD_CB_3B, ::op_FD_CB_3C, ::op_FD_CB_3D, ::op_FD_CB_3E, ::op_FD_CB_3F,
        ::op_FD_CB_40, ::op_FD_CB_41, ::op_FD_CB_42, ::op_FD_CB_43, ::op_FD_CB_44, ::op_FD_CB_45, ::op_FD_CB_46, ::op_FD_CB_47,
        ::op_FD_CB_48, ::op_FD_CB_49, ::op_FD_CB_4A, ::op_FD_CB_4B, ::op_FD_CB_4C, ::op_FD_CB_4D, ::op_FD_CB_4E, ::op_FD_CB_4F,
        ::op_FD_CB_50, ::op_FD_CB_51, ::op_FD_CB_52, ::op_FD_CB_53, ::op_FD_CB_54, ::op_FD_CB_55, ::op_FD_CB_56, ::op_FD_CB_57,
        ::op_FD_CB_58, ::op_FD_CB_59, ::op_FD_CB_5A, ::op_FD_CB_5B, ::op_FD_CB_5C, ::op_FD_CB_5D, ::op_FD_CB_5E, ::op_FD_CB_5F,
        ::op_FD_CB_60, ::op_FD_CB_61, ::op_FD_CB_62, ::op_FD_CB_63, ::op_FD_CB_64, ::op_FD_CB_65, ::op_FD_CB_66, ::op_FD_CB_67,
        ::op_FD_CB_68, ::op_FD_CB_69, ::op_FD_CB_6A, ::op_FD_CB_6B, ::op_FD_CB_6C, ::op_FD_CB_6D, ::op_FD_CB_6E, ::op_FD_CB_6F,
        ::op_FD_CB_70, ::op_FD_CB_71, ::op_FD_CB_72, ::op_FD_CB_73, ::op_FD_CB_74, ::op_FD_CB_75, ::op_FD_CB_76, ::op_FD_CB_77,
        ::op_FD_CB_78, ::op_FD_CB_79, ::op_FD_CB_7A, ::op_FD_CB_7B, ::op_FD_CB_7C, ::op_FD_CB_7D, ::op_FD_CB_7E, ::op_FD_CB_7F,
        ::op_FD_CB_80, ::op_FD_CB_81, ::op_FD_CB_82, ::op_FD_CB_83, ::op_FD_CB_84, ::op_FD_CB_85, ::op_FD_CB_86, ::op_FD_CB_87,
        ::op_FD_CB_88, ::op_FD_CB_89, ::op_FD_CB_8A, ::op_FD_CB_8B, ::op_FD_CB_8C, ::op_FD_CB_8D, ::op_FD_CB_8E, ::op_FD_CB_8F,
        ::op_FD_CB_90, ::op_FD_CB_91, ::op_FD_CB_92, ::op_FD_CB_93, ::op_FD_CB_94, ::op_FD_CB_95, ::op_FD_CB_96, ::op_FD_CB_97,
        ::op_FD_CB_98, ::op_FD_CB_99, ::op_FD_CB_9A, ::op_FD_CB_9B, ::op_FD_CB_9C, ::op_FD_CB_9D, ::op_FD_CB_9E, ::op_FD_CB_9F,
        ::op_FD_CB_A0, ::op_FD_CB_A1, ::op_FD_CB_A2, ::op_FD_CB_A3, ::op_FD_CB_A4, ::op_FD_CB_A5, ::op_FD_CB_A6, ::op_FD_CB_A7,
        ::op_FD_CB_A8, ::op_FD_CB_A9, ::op_FD_CB_AA, ::op_FD_CB_AB, ::op_FD_CB_AC, ::op_FD_CB_AD, ::op_FD_CB_AE, ::op_FD_CB_AF,
        ::op_FD_CB_B0, ::op_FD_CB_B1, ::op_FD_CB_B2, ::op_FD_CB_B3, ::op_FD_CB_B4, ::op_FD_CB_B5, ::op_FD_CB_B6, ::op_FD_CB_B7,
        ::op_FD_CB_B8, ::op_FD_CB_B9, ::op_FD_CB_BA, ::op_FD_CB_BB, ::op_FD_CB_BC, ::op_FD_CB_BD, ::op_FD_CB_BE, ::op_FD_CB_BF,
        ::op_FD_CB_C0, ::op_FD_CB_C1, ::op_FD_CB_C2, ::op_FD_CB_C3, ::op_FD_CB_C4, ::op_FD_CB_C5, ::op_FD_CB_C6, ::op_FD_CB_C7,
        ::op_FD_CB_C8, ::op_FD_CB_C9, ::op_FD_CB_CA, ::op_FD_CB_CB, ::op_FD_CB_CC, ::op_FD_CB_CD, ::op_FD_CB_CE, ::op_FD_CB_CF,
        ::op_FD_CB_D0, ::op_FD_CB_D1, ::op_FD_CB_D2, ::op_FD_CB_D3, ::op_FD_CB_D4, ::op_FD_CB_D5, ::op_FD_CB_D6, ::op_FD_CB_D7,
        ::op_FD_CB_D8, ::op_FD_CB_D9, ::op_FD_CB_DA, ::op_FD_CB_DB, ::op_FD_CB_DC, ::op_FD_CB_DD, ::op_FD_CB_DE, ::op_FD_CB_DF,
        ::op_FD_CB_E0, ::op_FD_CB_E1, ::op_FD_CB_E2, ::op_FD_CB_E3, ::op_FD_CB_E4, ::op_FD_CB_E5, ::op_FD_CB_E6, ::op_FD_CB_E7,
        ::op_FD_CB_E8, ::op_FD_CB_E9, ::op_FD_CB_EA, ::op_FD_CB_EB, ::op_FD_CB_EC, ::op_FD_CB_ED, ::op_FD_CB_EE, ::op_FD_CB_EF,
        ::op_FD_CB_F0, ::op_FD_CB_F1, ::op_FD_CB_F2, ::op_FD_CB_F3, ::op_FD_CB_F4, ::op_FD_CB_F5, ::op_FD_CB_F6, ::op_FD_CB_F7,
        ::op_FD_CB_F8, ::op_FD_CB_F9, ::op_FD_CB_FA, ::op_FD_CB_FB, ::op_FD_CB_FC, ::op_FD_CB_FD, ::op_FD_CB_FE, ::op_FD_CB_FF
    };
#end
