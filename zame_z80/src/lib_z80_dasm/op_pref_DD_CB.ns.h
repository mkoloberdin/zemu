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

#namespace Dasm
    const s_DasmItem ::optable_DD_CB[0x100] = {
        { "LD B,RLC (IX[SO])", NULL },      // 00
        { "LD C,RLC (IX[SO])", NULL },      // 01
        { "LD D,RLC (IX[SO])", NULL },      // 02
        { "LD E,RLC (IX[SO])", NULL },      // 03
        { "LD H,RLC (IX[SO])", NULL },      // 04
        { "LD L,RLC (IX[SO])", NULL },      // 05
        { "RLC (IX[SO])", NULL },           // 06
        { "LD A,RLC (IX[SO])", NULL },      // 07
        { "LD B,RRC (IX[SO])", NULL },      // 08
        { "LD C,RRC (IX[SO])", NULL },      // 09
        { "LD D,RRC (IX[SO])", NULL },      // 0A
        { "LD E,RRC (IX[SO])", NULL },      // 0B
        { "LD H,RRC (IX[SO])", NULL },      // 0C
        { "LD L,RRC (IX[SO])", NULL },      // 0D
        { "RRC (IX[SO])", NULL },           // 0E
        { "LD A,RRC (IX[SO])", NULL },      // 0F

        { "LD B,RL (IX[SO])", NULL },       // 10
        { "LD C,RL (IX[SO])", NULL },       // 11
        { "LD D,RL (IX[SO])", NULL },       // 12
        { "LD E,RL (IX[SO])", NULL },       // 13
        { "LD H,RL (IX[SO])", NULL },       // 14
        { "LD L,RL (IX[SO])", NULL },       // 15
        { "RL (IX[SO])", NULL },            // 16
        { "LD A,RL (IX[SO])", NULL },       // 17
        { "LD B,RR (IX[SO])", NULL },       // 18
        { "LD C,RR (IX[SO])", NULL },       // 19
        { "LD D,RR (IX[SO])", NULL },       // 1A
        { "LD E,RR (IX[SO])", NULL },       // 1B
        { "LD H,RR (IX[SO])", NULL },       // 1C
        { "LD L,RR (IX[SO])", NULL },       // 1D
        { "RR (IX[SO])", NULL },            // 1E
        { "LD A,RR (IX[SO])", NULL },       // 1F

        { "LD B,SLA (IX[SO])", NULL },      // 20
        { "LD C,SLA (IX[SO])", NULL },      // 21
        { "LD D,SLA (IX[SO])", NULL },      // 22
        { "LD E,SLA (IX[SO])", NULL },      // 23
        { "LD H,SLA (IX[SO])", NULL },      // 24
        { "LD L,SLA (IX[SO])", NULL },      // 25
        { "SLA (IX[SO])", NULL },           // 26
        { "LD A,SLA (IX[SO])", NULL },      // 27
        { "LD B,SRA (IX[SO])", NULL },      // 28
        { "LD C,SRA (IX[SO])", NULL },      // 29
        { "LD D,SRA (IX[SO])", NULL },      // 2A
        { "LD E,SRA (IX[SO])", NULL },      // 2B
        { "LD H,SRA (IX[SO])", NULL },      // 2C
        { "LD L,SRA (IX[SO])", NULL },      // 2D
        { "SRA (IX[SO])", NULL },           // 2E
        { "LD A,SRA (IX[SO])", NULL },      // 2F

        { "LD B,SLL (IX[SO])", NULL },      // 30
        { "LD C,SLL (IX[SO])", NULL },      // 31
        { "LD D,SLL (IX[SO])", NULL },      // 32
        { "LD E,SLL (IX[SO])", NULL },      // 33
        { "LD H,SLL (IX[SO])", NULL },      // 34
        { "LD L,SLL (IX[SO])", NULL },      // 35
        { "SLL (IX[SO])", NULL },           // 36
        { "LD A,SLL (IX[SO])", NULL },      // 37
        { "LD B,SRL (IX[SO])", NULL },      // 38
        { "LD C,SRL (IX[SO])", NULL },      // 39
        { "LD D,SRL (IX[SO])", NULL },      // 3A
        { "LD E,SRL (IX[SO])", NULL },      // 3B
        { "LD H,SRL (IX[SO])", NULL },      // 3C
        { "LD L,SRL (IX[SO])", NULL },      // 3D
        { "SRL (IX[SO])", NULL },           // 3E
        { "LD A,SRL (IX[SO])", NULL },      // 3F

        { "BIT 0,(IX[SO])", NULL },         // 40
        { "BIT 0,(IX[SO])", NULL },         // 41
        { "BIT 0,(IX[SO])", NULL },         // 42
        { "BIT 0,(IX[SO])", NULL },         // 43
        { "BIT 0,(IX[SO])", NULL },         // 44
        { "BIT 0,(IX[SO])", NULL },         // 45
        { "BIT 0,(IX[SO])", NULL },         // 46
        { "BIT 0,(IX[SO])", NULL },         // 47
        { "BIT 1,(IX[SO])", NULL },         // 48
        { "BIT 1,(IX[SO])", NULL },         // 49
        { "BIT 1,(IX[SO])", NULL },         // 4A
        { "BIT 1,(IX[SO])", NULL },         // 4B
        { "BIT 1,(IX[SO])", NULL },         // 4C
        { "BIT 1,(IX[SO])", NULL },         // 4D
        { "BIT 1,(IX[SO])", NULL },         // 4E
        { "BIT 1,(IX[SO])", NULL },         // 4F

        { "BIT 2,(IX[SO])", NULL },         // 50
        { "BIT 2,(IX[SO])", NULL },         // 51
        { "BIT 2,(IX[SO])", NULL },         // 52
        { "BIT 2,(IX[SO])", NULL },         // 53
        { "BIT 2,(IX[SO])", NULL },         // 54
        { "BIT 2,(IX[SO])", NULL },         // 55
        { "BIT 2,(IX[SO])", NULL },         // 56
        { "BIT 2,(IX[SO])", NULL },         // 57
        { "BIT 3,(IX[SO])", NULL },         // 58
        { "BIT 3,(IX[SO])", NULL },         // 59
        { "BIT 3,(IX[SO])", NULL },         // 5A
        { "BIT 3,(IX[SO])", NULL },         // 5B
        { "BIT 3,(IX[SO])", NULL },         // 5C
        { "BIT 3,(IX[SO])", NULL },         // 5D
        { "BIT 3,(IX[SO])", NULL },         // 5E
        { "BIT 3,(IX[SO])", NULL },         // 5F

        { "BIT 4,(IX[SO])", NULL },         // 60
        { "BIT 4,(IX[SO])", NULL },         // 61
        { "BIT 4,(IX[SO])", NULL },         // 62
        { "BIT 4,(IX[SO])", NULL },         // 63
        { "BIT 4,(IX[SO])", NULL },         // 64
        { "BIT 4,(IX[SO])", NULL },         // 65
        { "BIT 4,(IX[SO])", NULL },         // 66
        { "BIT 4,(IX[SO])", NULL },         // 67
        { "BIT 5,(IX[SO])", NULL },         // 68
        { "BIT 5,(IX[SO])", NULL },         // 69
        { "BIT 5,(IX[SO])", NULL },         // 6A
        { "BIT 5,(IX[SO])", NULL },         // 6B
        { "BIT 5,(IX[SO])", NULL },         // 6C
        { "BIT 5,(IX[SO])", NULL },         // 6D
        { "BIT 5,(IX[SO])", NULL },         // 6E
        { "BIT 5,(IX[SO])", NULL },         // 6F

        { "BIT 6,(IX[SO])", NULL },         // 70
        { "BIT 6,(IX[SO])", NULL },         // 71
        { "BIT 6,(IX[SO])", NULL },         // 72
        { "BIT 6,(IX[SO])", NULL },         // 73
        { "BIT 6,(IX[SO])", NULL },         // 74
        { "BIT 6,(IX[SO])", NULL },         // 75
        { "BIT 6,(IX[SO])", NULL },         // 76
        { "BIT 6,(IX[SO])", NULL },         // 77
        { "BIT 7,(IX[SO])", NULL },         // 78
        { "BIT 7,(IX[SO])", NULL },         // 79
        { "BIT 7,(IX[SO])", NULL },         // 7A
        { "BIT 7,(IX[SO])", NULL },         // 7B
        { "BIT 7,(IX[SO])", NULL },         // 7C
        { "BIT 7,(IX[SO])", NULL },         // 7D
        { "BIT 7,(IX[SO])", NULL },         // 7E
        { "BIT 7,(IX[SO])", NULL },         // 7F

        { "LD B,RES 0,(IX[SO])", NULL },    // 80
        { "LD C,RES 0,(IX[SO])", NULL },    // 81
        { "LD D,RES 0,(IX[SO])", NULL },    // 82
        { "LD E,RES 0,(IX[SO])", NULL },    // 83
        { "LD H,RES 0,(IX[SO])", NULL },    // 84
        { "LD L,RES 0,(IX[SO])", NULL },    // 85
        { "RES 0,(IX[SO])", NULL },         // 86
        { "LD A,RES 0,(IX[SO])", NULL },    // 87
        { "LD B,RES 1,(IX[SO])", NULL },    // 88
        { "LD C,RES 1,(IX[SO])", NULL },    // 89
        { "LD D,RES 1,(IX[SO])", NULL },    // 8A
        { "LD E,RES 1,(IX[SO])", NULL },    // 8B
        { "LD H,RES 1,(IX[SO])", NULL },    // 8C
        { "LD L,RES 1,(IX[SO])", NULL },    // 8D
        { "RES 1,(IX[SO])", NULL },         // 8E
        { "LD A,RES 1,(IX[SO])", NULL },    // 8F

        { "LD B,RES 2,(IX[SO])", NULL },    // 90
        { "LD C,RES 2,(IX[SO])", NULL },    // 91
        { "LD D,RES 2,(IX[SO])", NULL },    // 92
        { "LD E,RES 2,(IX[SO])", NULL },    // 93
        { "LD H,RES 2,(IX[SO])", NULL },    // 94
        { "LD L,RES 2,(IX[SO])", NULL },    // 95
        { "RES 2,(IX[SO])", NULL },         // 96
        { "LD A,RES 2,(IX[SO])", NULL },    // 97
        { "LD B,RES 3,(IX[SO])", NULL },    // 98
        { "LD C,RES 3,(IX[SO])", NULL },    // 99
        { "LD D,RES 3,(IX[SO])", NULL },    // 9A
        { "LD E,RES 3,(IX[SO])", NULL },    // 9B
        { "LD H,RES 3,(IX[SO])", NULL },    // 9C
        { "LD L,RES 3,(IX[SO])", NULL },    // 9D
        { "RES 3,(IX[SO])", NULL },         // 9E
        { "LD A,RES 3,(IX[SO])", NULL },    // 9F

        { "LD B,RES 4,(IX[SO])", NULL },    // A0
        { "LD C,RES 4,(IX[SO])", NULL },    // A1
        { "LD D,RES 4,(IX[SO])", NULL },    // A2
        { "LD E,RES 4,(IX[SO])", NULL },    // A3
        { "LD H,RES 4,(IX[SO])", NULL },    // A4
        { "LD L,RES 4,(IX[SO])", NULL },    // A5
        { "RES 4,(IX[SO])", NULL },         // A6
        { "LD A,RES 4,(IX[SO])", NULL },    // A7
        { "LD B,RES 5,(IX[SO])", NULL },    // A8
        { "LD C,RES 5,(IX[SO])", NULL },    // A9
        { "LD D,RES 5,(IX[SO])", NULL },    // AA
        { "LD E,RES 5,(IX[SO])", NULL },    // AB
        { "LD H,RES 5,(IX[SO])", NULL },    // AC
        { "LD L,RES 5,(IX[SO])", NULL },    // AD
        { "RES 5,(IX[SO])", NULL },         // AE
        { "LD A,RES 5,(IX[SO])", NULL },    // AF

        { "LD B,RES 6,(IX[SO])", NULL },    // B0
        { "LD C,RES 6,(IX[SO])", NULL },    // B1
        { "LD D,RES 6,(IX[SO])", NULL },    // B2
        { "LD E,RES 6,(IX[SO])", NULL },    // B3
        { "LD H,RES 6,(IX[SO])", NULL },    // B4
        { "LD L,RES 6,(IX[SO])", NULL },    // B5
        { "RES 6,(IX[SO])", NULL },         // B6
        { "LD A,RES 6,(IX[SO])", NULL },    // B7
        { "LD B,RES 7,(IX[SO])", NULL },    // B8
        { "LD C,RES 7,(IX[SO])", NULL },    // B9
        { "LD D,RES 7,(IX[SO])", NULL },    // BA
        { "LD E,RES 7,(IX[SO])", NULL },    // BB
        { "LD H,RES 7,(IX[SO])", NULL },    // BC
        { "LD L,RES 7,(IX[SO])", NULL },    // BD
        { "RES 7,(IX[SO])", NULL },         // BE
        { "LD A,RES 7,(IX[SO])", NULL },    // BF

        { "LD B,SET 0,(IX[SO])", NULL },    // C0
        { "LD C,SET 0,(IX[SO])", NULL },    // C1
        { "LD D,SET 0,(IX[SO])", NULL },    // C2
        { "LD E,SET 0,(IX[SO])", NULL },    // C3
        { "LD H,SET 0,(IX[SO])", NULL },    // C4
        { "LD L,SET 0,(IX[SO])", NULL },    // C5
        { "SET 0,(IX[SO])", NULL },         // C6
        { "LD A,SET 0,(IX[SO])", NULL },    // C7
        { "LD B,SET 1,(IX[SO])", NULL },    // C8
        { "LD C,SET 1,(IX[SO])", NULL },    // C9
        { "LD D,SET 1,(IX[SO])", NULL },    // CA
        { "LD E,SET 1,(IX[SO])", NULL },    // CB
        { "LD H,SET 1,(IX[SO])", NULL },    // CC
        { "LD L,SET 1,(IX[SO])", NULL },    // CD
        { "SET 1,(IX[SO])", NULL },         // CE
        { "LD A,SET 1,(IX[SO])", NULL },    // CF

        { "LD B,SET 2,(IX[SO])", NULL },    // D0
        { "LD C,SET 2,(IX[SO])", NULL },    // D1
        { "LD D,SET 2,(IX[SO])", NULL },    // D2
        { "LD E,SET 2,(IX[SO])", NULL },    // D3
        { "LD H,SET 2,(IX[SO])", NULL },    // D4
        { "LD L,SET 2,(IX[SO])", NULL },    // D5
        { "SET 2,(IX[SO])", NULL },         // D6
        { "LD A,SET 2,(IX[SO])", NULL },    // D7
        { "LD B,SET 3,(IX[SO])", NULL },    // D8
        { "LD C,SET 3,(IX[SO])", NULL },    // D9
        { "LD D,SET 3,(IX[SO])", NULL },    // DA
        { "LD E,SET 3,(IX[SO])", NULL },    // DB
        { "LD H,SET 3,(IX[SO])", NULL },    // DC
        { "LD L,SET 3,(IX[SO])", NULL },    // DD
        { "SET 3,(IX[SO])", NULL },         // DE
        { "LD A,SET 3,(IX[SO])", NULL },    // DF

        { "LD B,SET 4,(IX[SO])", NULL },    // E0
        { "LD C,SET 4,(IX[SO])", NULL },    // E1
        { "LD D,SET 4,(IX[SO])", NULL },    // E2
        { "LD E,SET 4,(IX[SO])", NULL },    // E3
        { "LD H,SET 4,(IX[SO])", NULL },    // E4
        { "LD L,SET 4,(IX[SO])", NULL },    // E5
        { "SET 4,(IX[SO])", NULL },         // E6
        { "LD A,SET 4,(IX[SO])", NULL },    // E7
        { "LD B,SET 5,(IX[SO])", NULL },    // E8
        { "LD C,SET 5,(IX[SO])", NULL },    // E9
        { "LD D,SET 5,(IX[SO])", NULL },    // EA
        { "LD E,SET 5,(IX[SO])", NULL },    // EB
        { "LD H,SET 5,(IX[SO])", NULL },    // EC
        { "LD L,SET 5,(IX[SO])", NULL },    // ED
        { "SET 5,(IX+[N]", NULL },          // EE
        { "LD A,SET 5,(IX[SO])", NULL },    // EF

        { "LD B,SET 6,(IX[SO])", NULL },    // F0
        { "LD C,SET 6,(IX[SO])", NULL },    // F1
        { "LD D,SET 6,(IX[SO])", NULL },    // F2
        { "LD E,SET 6,(IX[SO])", NULL },    // F3
        { "LD H,SET 6,(IX[SO])", NULL },    // F4
        { "LD L,SET 6,(IX[SO])", NULL },    // F5
        { "SET 6,(IX[SO])", NULL },         // F6
        { "LD A,SET 6,(IX[SO])", NULL },    // F7
        { "LD B,SET 7,(IX[SO])", NULL },    // F8
        { "LD C,SET 7,(IX[SO])", NULL },    // F9
        { "LD D,SET 7,(IX[SO])", NULL },    // FA
        { "LD E,SET 7,(IX[SO])", NULL },    // FB
        { "LD H,SET 7,(IX[SO])", NULL },    // FC
        { "LD L,SET 7,(IX[SO])", NULL },    // FD
        { "SET 7,(IX[SO])", NULL },         // FE
        { "LD A,SET 7,(IX[SO])", NULL },    // FF
    };
#end
