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
    const s_DasmItem ::optable_ED[0x100] = {
        { "*NOP", NULL },                   // 00
        { "*NOP", NULL },                   // 01
        { "*NOP", NULL },                   // 02
        { "*NOP", NULL },                   // 03
        { "*NOP", NULL },                   // 04
        { "*NOP", NULL },                   // 05
        { "*NOP", NULL },                   // 06
        { "*NOP", NULL },                   // 07
        { "*NOP", NULL },                   // 08
        { "*NOP", NULL },                   // 09
        { "*NOP", NULL },                   // 0A
        { "*NOP", NULL },                   // 0B
        { "*NOP", NULL },                   // 0C
        { "*NOP", NULL },                   // 0D
        { "*NOP", NULL },                   // 0E
        { "*NOP", NULL },                   // 0F

        { "*NOP", NULL },                   // 10
        { "*NOP", NULL },                   // 11
        { "*NOP", NULL },                   // 12
        { "*NOP", NULL },                   // 13
        { "*NOP", NULL },                   // 14
        { "*NOP", NULL },                   // 15
        { "*NOP", NULL },                   // 16
        { "*NOP", NULL },                   // 17
        { "*NOP", NULL },                   // 18
        { "*NOP", NULL },                   // 19
        { "*NOP", NULL },                   // 1A
        { "*NOP", NULL },                   // 1B
        { "*NOP", NULL },                   // 1C
        { "*NOP", NULL },                   // 1D
        { "*NOP", NULL },                   // 1E
        { "*NOP", NULL },                   // 1F

        { "*NOP", NULL },                   // 20
        { "*NOP", NULL },                   // 21
        { "*NOP", NULL },                   // 22
        { "*NOP", NULL },                   // 23
        { "*NOP", NULL },                   // 24
        { "*NOP", NULL },                   // 25
        { "*NOP", NULL },                   // 26
        { "*NOP", NULL },                   // 27
        { "*NOP", NULL },                   // 28
        { "*NOP", NULL },                   // 29
        { "*NOP", NULL },                   // 2A
        { "*NOP", NULL },                   // 2B
        { "*NOP", NULL },                   // 2C
        { "*NOP", NULL },                   // 2D
        { "*NOP", NULL },                   // 2E
        { "*NOP", NULL },                   // 2F

        { "*NOP", NULL },                   // 30
        { "*NOP", NULL },                   // 31
        { "*NOP", NULL },                   // 32
        { "*NOP", NULL },                   // 33
        { "*NOP", NULL },                   // 34
        { "*NOP", NULL },                   // 35
        { "*NOP", NULL },                   // 36
        { "*NOP", NULL },                   // 37
        { "*NOP", NULL },                   // 38
        { "*NOP", NULL },                   // 39
        { "*NOP", NULL },                   // 3A
        { "*NOP", NULL },                   // 3B
        { "*NOP", NULL },                   // 3C
        { "*NOP", NULL },                   // 3D
        { "*NOP", NULL },                   // 3E
        { "*NOP", NULL },                   // 3F

        { "IN B,(C)", NULL },               // 40
        { "OUT (C),B", NULL },              // 41
        { "SBC HL,BC", NULL },              // 42
        { "LD ([NN]),BC", NULL },           // 43
        { "NEG", NULL },                    // 44
        { "RETN", NULL },                   // 45
        { "IM 0", NULL },                   // 46
        { "LD I,A", NULL },                 // 47
        { "IN C,(C)", NULL },               // 48
        { "OUT (C),C", NULL },              // 49
        { "ADC HL,BC", NULL },              // 4A
        { "LD BC,([NN])", NULL },           // 4B
        { "NEG", NULL },                    // 4C
        { "RETI", NULL },                   // 4D
        { "IM 0", NULL },                   // 4E
        { "LD R,A", NULL },                 // 4F

        { "IN D,(C)", NULL },               // 50
        { "OUT (C),D", NULL },              // 51
        { "SBC HL,DE", NULL },              // 52
        { "LD ([NN]),DE", NULL },           // 53
        { "NEG", NULL },                    // 54
        { "RETN", NULL },                   // 55
        { "IM 1", NULL },                   // 56
        { "LD A,I", NULL },                 // 57
        { "IN E,(C)", NULL },               // 58
        { "OUT (C),E", NULL },              // 59
        { "ADC HL,DE", NULL },              // 5A
        { "LD DE,([NN])", NULL },           // 5B
        { "NEG", NULL },                    // 5C
        { "RETN", NULL },                   // 5D
        { "IM 2", NULL },                   // 5E
        { "LD A,R", NULL },                 // 5F

        { "IN H,(C)", NULL },               // 60
        { "OUT (H),D", NULL },              // 61
        { "SBC HL,HL", NULL },              // 62
        { "LD ([NN]),HL", NULL },           // 63
        { "NEG", NULL },                    // 64
        { "RETN", NULL },                   // 65
        { "IM 0", NULL },                   // 66
        { "RRD", NULL },                    // 67
        { "IN L,(C)", NULL },               // 68
        { "OUT (C),L", NULL },              // 69
        { "ADC HL,HL", NULL },              // 6A
        { "LD HL,([NN])", NULL },           // 6B
        { "NEG", NULL },                    // 6C
        { "RETN", NULL },                   // 6D
        { "IM 0", NULL },                   // 6E
        { "RLD", NULL },                    // 6F

        { "IN F,(C)", NULL },               // 70
        { "OUT (C),0", NULL },              // 71
        { "SBC HL,SP", NULL },              // 72
        { "LD ([NN]),SP", NULL },           // 73
        { "NEG", NULL },                    // 74
        { "RETN", NULL },                   // 75
        { "IM 1", NULL },                   // 76
        { "LD (HL),A", NULL },              // 77
        { "IN A,(C)", NULL },               // 78
        { "OUT (C),A", NULL },              // 79
        { "ADC HL,SP", NULL },              // 7A
        { "LD SP,([NN])", NULL },           // 7B
        { "NEG", NULL },                    // 7C
        { "RETN", NULL },                   // 7D
        { "IM 2", NULL },                   // 7E
        { "*NOP", NULL },                   // 7F

        { "*NOP", NULL },                   // 80
        { "*NOP", NULL },                   // 81
        { "*NOP", NULL },                   // 82
        { "*NOP", NULL },                   // 83
        { "*NOP", NULL },                   // 84
        { "*NOP", NULL },                   // 85
        { "*NOP", NULL },                   // 86
        { "*NOP", NULL },                   // 87
        { "*NOP", NULL },                   // 88
        { "*NOP", NULL },                   // 89
        { "*NOP", NULL },                   // 8A
        { "*NOP", NULL },                   // 8B
        { "*NOP", NULL },                   // 8C
        { "*NOP", NULL },                   // 8D
        { "*NOP", NULL },                   // 8E
        { "*NOP", NULL },                   // 8F

        { "*NOP", NULL },                   // 90
        { "*NOP", NULL },                   // 91
        { "*NOP", NULL },                   // 92
        { "*NOP", NULL },                   // 93
        { "*NOP", NULL },                   // 94
        { "*NOP", NULL },                   // 95
        { "*NOP", NULL },                   // 96
        { "*NOP", NULL },                   // 97
        { "*NOP", NULL },                   // 98
        { "*NOP", NULL },                   // 99
        { "*NOP", NULL },                   // 9A
        { "*NOP", NULL },                   // 9B
        { "*NOP", NULL },                   // 9C
        { "*NOP", NULL },                   // 9D
        { "*NOP", NULL },                   // 9E
        { "*NOP", NULL },                   // 9F

        { "LDI", NULL },                    // A0
        { "CPI", NULL },                    // A1
        { "INI", NULL },                    // A2
        { "OUTI", NULL },                   // A3
        { "*NOP", NULL },                   // A4
        { "*NOP", NULL },                   // A5
        { "*NOP", NULL },                   // A6
        { "*NOP", NULL },                   // A7
        { "LDD", NULL },                    // A8
        { "CPD", NULL },                    // A9
        { "IND", NULL },                    // AA
        { "OUTD", NULL },                   // AB
        { "*NOP", NULL },                   // AC
        { "*NOP", NULL },                   // AD
        { "*NOP", NULL },                   // AE
        { "*NOP", NULL },                   // AF

        { "LDIR", NULL },                   // B0
        { "CPIR", NULL },                   // B1
        { "INIR", NULL },                   // B2
        { "OTIR", NULL },                   // B3
        { "*NOP", NULL },                   // B4
        { "*NOP", NULL },                   // B5
        { "*NOP", NULL },                   // B6
        { "*NOP", NULL },                   // B7
        { "LDDR", NULL },                   // B8
        { "CPDR", NULL },                   // B9
        { "INDR", NULL },                   // BA
        { "OTDR", NULL },                   // BB
        { "*NOP", NULL },                   // BC
        { "*NOP", NULL },                   // BD
        { "*NOP", NULL },                   // BE
        { "*NOP", NULL },                   // BF

        { "*NOP", NULL },                   // C0
        { "*NOP", NULL },                   // C1
        { "*NOP", NULL },                   // C2
        { "*NOP", NULL },                   // C3
        { "*NOP", NULL },                   // C4
        { "*NOP", NULL },                   // C5
        { "*NOP", NULL },                   // C6
        { "*NOP", NULL },                   // C7
        { "*NOP", NULL },                   // C8
        { "*NOP", NULL },                   // C9
        { "*NOP", NULL },                   // CA
        { "*NOP", NULL },                   // CB
        { "*NOP", NULL },                   // CC
        { "*NOP", NULL },                   // CD
        { "*NOP", NULL },                   // CE
        { "*NOP", NULL },                   // CF

        { "*NOP", NULL },                   // D0
        { "*NOP", NULL },                   // D1
        { "*NOP", NULL },                   // D2
        { "*NOP", NULL },                   // D3
        { "*NOP", NULL },                   // D4
        { "*NOP", NULL },                   // D5
        { "*NOP", NULL },                   // D6
        { "*NOP", NULL },                   // D7
        { "*NOP", NULL },                   // D8
        { "*NOP", NULL },                   // D9
        { "*NOP", NULL },                   // DA
        { "*NOP", NULL },                   // DB
        { "*NOP", NULL },                   // DC
        { "*NOP", NULL },                   // DD
        { "*NOP", NULL },                   // DE
        { "*NOP", NULL },                   // DF

        { "*NOP", NULL },                   // E0
        { "*NOP", NULL },                   // E1
        { "*NOP", NULL },                   // E2
        { "*NOP", NULL },                   // E3
        { "*NOP", NULL },                   // E4
        { "*NOP", NULL },                   // E5
        { "*NOP", NULL },                   // E6
        { "*NOP", NULL },                   // E7
        { "*NOP", NULL },                   // E8
        { "*NOP", NULL },                   // E9
        { "*NOP", NULL },                   // EA
        { "*NOP", NULL },                   // EB
        { "*NOP", NULL },                   // EC
        { "*NOP", NULL },                   // ED
        { "*NOP", NULL },                   // EE
        { "*NOP", NULL },                   // EF

        { "*NOP", NULL },                   // F0
        { "*NOP", NULL },                   // F1
        { "*NOP", NULL },                   // F2
        { "*NOP", NULL },                   // F3
        { "*NOP", NULL },                   // F4
        { "*NOP", NULL },                   // F5
        { "*NOP", NULL },                   // F6
        { "*NOP", NULL },                   // F7
        { "*NOP", NULL },                   // F8
        { "*NOP", NULL },                   // F9
        { "*NOP", NULL },                   // FA
        { "*NOP", NULL },                   // FB
        { "*NOP", NULL },                   // FC
        { "*NOP", NULL },                   // FD
        { "*NOP", NULL },                   // FE
        { "*NOP", NULL },                   // FF
    };
#end
