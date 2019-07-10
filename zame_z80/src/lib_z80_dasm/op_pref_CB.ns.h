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
    const s_DasmItem ::optable_CB[0x100] = {
        { "RLC B", NULL },                  // 00
        { "RLC C", NULL },                  // 01
        { "RLC D", NULL },                  // 02
        { "RLC E", NULL },                  // 03
        { "RLC H", NULL },                  // 04
        { "RLC L", NULL },                  // 05
        { "RLC (HL)", NULL },               // 06
        { "RLC A", NULL },                  // 07
        { "RRC B", NULL },                  // 08
        { "RRC C", NULL },                  // 09
        { "RRC D", NULL },                  // 0A
        { "RRC E", NULL },                  // 0B
        { "RRC H", NULL },                  // 0C
        { "RRC L", NULL },                  // 0D
        { "RRC (HL)", NULL },               // 0E
        { "RRC A", NULL },                  // 0F

        { "RL B", NULL },                   // 10
        { "RL C", NULL },                   // 11
        { "RL D", NULL },                   // 12
        { "RL E", NULL },                   // 13
        { "RL H", NULL },                   // 14
        { "RL L", NULL },                   // 15
        { "RL (HL)", NULL },                // 16
        { "RL A", NULL },                   // 17
        { "RR B", NULL },                   // 18
        { "RR C", NULL },                   // 19
        { "RR D", NULL },                   // 1A
        { "RR E", NULL },                   // 1B
        { "RR H", NULL },                   // 1C
        { "RR L", NULL },                   // 1D
        { "RR (HL)", NULL },                // 1E
        { "RR A", NULL },                   // 1F

        { "SLA B", NULL },                  // 20
        { "SLA C", NULL },                  // 21
        { "SLA D", NULL },                  // 22
        { "SLA E", NULL },                  // 23
        { "SLA H", NULL },                  // 24
        { "SLA L", NULL },                  // 25
        { "SLA (HL)", NULL },               // 26
        { "SLA A", NULL },                  // 27
        { "SRA B", NULL },                  // 28
        { "SRA C", NULL },                  // 29
        { "SRA D", NULL },                  // 2A
        { "SRA E", NULL },                  // 2B
        { "SRA H", NULL },                  // 2C
        { "SRA L", NULL },                  // 2D
        { "SRA (HL)", NULL },               // 2E
        { "SRA A", NULL },                  // 2F

        { "SLL B", NULL },                  // 30
        { "SLL C", NULL },                  // 31
        { "SLL D", NULL },                  // 32
        { "SLL E", NULL },                  // 33
        { "SLL H", NULL },                  // 34
        { "SLL L", NULL },                  // 35
        { "SLL (HL)", NULL },               // 36
        { "SLL A", NULL },                  // 37
        { "SRL B", NULL },                  // 38
        { "SRL C", NULL },                  // 39
        { "SRL D", NULL },                  // 3A
        { "SRL E", NULL },                  // 3B
        { "SRL H", NULL },                  // 3C
        { "SRL L", NULL },                  // 3D
        { "SRL (HL)", NULL },               // 3E
        { "SRL A", NULL },                  // 3F

        { "BIT 0,B", NULL },                // 40
        { "BIT 0,C", NULL },                // 41
        { "BIT 0,D", NULL },                // 42
        { "BIT 0,E", NULL },                // 43
        { "BIT 0,H", NULL },                // 44
        { "BIT 0,L", NULL },                // 45
        { "BIT 0,(HL)", NULL },             // 46
        { "BIT 0,A", NULL },                // 47
        { "BIT 1,B", NULL },                // 48
        { "BIT 1,C", NULL },                // 49
        { "BIT 1,D", NULL },                // 4A
        { "BIT 1,E", NULL },                // 4B
        { "BIT 1,H", NULL },                // 4C
        { "BIT 1,L", NULL },                // 4D
        { "BIT 1,(HL)", NULL },             // 4E
        { "BIT 1,A", NULL },                // 4F

        { "BIT 2,B", NULL },                // 50
        { "BIT 2,C", NULL },                // 51
        { "BIT 2,D", NULL },                // 52
        { "BIT 2,E", NULL },                // 53
        { "BIT 2,H", NULL },                // 54
        { "BIT 2,L", NULL },                // 55
        { "BIT 2,(HL)", NULL },             // 56
        { "BIT 2,A", NULL },                // 57
        { "BIT 3,B", NULL },                // 58
        { "BIT 3,C", NULL },                // 59
        { "BIT 3,D", NULL },                // 5A
        { "BIT 3,E", NULL },                // 5B
        { "BIT 3,H", NULL },                // 5C
        { "BIT 3,L", NULL },                // 5D
        { "BIT 3,(HL)", NULL },             // 5E
        { "BIT 3,A", NULL },                // 5F

        { "BIT 4,B", NULL },                // 60
        { "BIT 4,C", NULL },                // 61
        { "BIT 4,D", NULL },                // 62
        { "BIT 4,E", NULL },                // 63
        { "BIT 4,H", NULL },                // 64
        { "BIT 4,L", NULL },                // 65
        { "BIT 4,(HL)", NULL },             // 66
        { "BIT 4,A", NULL },                // 67
        { "BIT 5,B", NULL },                // 68
        { "BIT 5,C", NULL },                // 69
        { "BIT 5,D", NULL },                // 6A
        { "BIT 5,E", NULL },                // 6B
        { "BIT 5,H", NULL },                // 6C
        { "BIT 5,L", NULL },                // 6D
        { "BIT 5,(HL)", NULL },             // 6E
        { "BIT 5,A", NULL },                // 6F

        { "BIT 6,B", NULL },                // 70
        { "BIT 6,C", NULL },                // 71
        { "BIT 6,D", NULL },                // 72
        { "BIT 6,E", NULL },                // 73
        { "BIT 6,H", NULL },                // 74
        { "BIT 6,L", NULL },                // 75
        { "BIT 6,(HL)", NULL },             // 76
        { "BIT 6,A", NULL },                // 77
        { "BIT 7,B", NULL },                // 78
        { "BIT 7,C", NULL },                // 79
        { "BIT 7,D", NULL },                // 7A
        { "BIT 7,E", NULL },                // 7B
        { "BIT 7,H", NULL },                // 7C
        { "BIT 7,L", NULL },                // 7D
        { "BIT 7,(HL)", NULL },             // 7E
        { "BIT 7,A", NULL },                // 7F

        { "RES 0,B", NULL },                // 80
        { "RES 0,C", NULL },                // 81
        { "RES 0,D", NULL },                // 82
        { "RES 0,E", NULL },                // 83
        { "RES 0,H", NULL },                // 84
        { "RES 0,L", NULL },                // 85
        { "RES 0,(HL)", NULL },             // 86
        { "RES 0,A", NULL },                // 87
        { "RES 1,B", NULL },                // 88
        { "RES 1,C", NULL },                // 89
        { "RES 1,D", NULL },                // 8A
        { "RES 1,E", NULL },                // 8B
        { "RES 1,H", NULL },                // 8C
        { "RES 1,L", NULL },                // 8D
        { "RES 1,(HL)", NULL },             // 8E
        { "RES 1,A", NULL },                // 8F

        { "RES 2,B", NULL },                // 90
        { "RES 2,C", NULL },                // 91
        { "RES 2,D", NULL },                // 92
        { "RES 2,E", NULL },                // 93
        { "RES 2,H", NULL },                // 94
        { "RES 2,L", NULL },                // 95
        { "RES 2,(HL)", NULL },             // 96
        { "RES 2,A", NULL },                // 97
        { "RES 3,B", NULL },                // 98
        { "RES 3,C", NULL },                // 99
        { "RES 3,D", NULL },                // 9A
        { "RES 3,E", NULL },                // 9B
        { "RES 3,H", NULL },                // 9C
        { "RES 3,L", NULL },                // 9D
        { "RES 3,(HL)", NULL },             // 9E
        { "RES 3,A", NULL },                // 9F

        { "RES 4,B", NULL },                // A0
        { "RES 4,C", NULL },                // A1
        { "RES 4,D", NULL },                // A2
        { "RES 4,E", NULL },                // A3
        { "RES 4,H", NULL },                // A4
        { "RES 4,L", NULL },                // A5
        { "RES 4,(HL)", NULL },             // A6
        { "RES 4,A", NULL },                // A7
        { "RES 5,B", NULL },                // A8
        { "RES 5,C", NULL },                // A9
        { "RES 5,D", NULL },                // AA
        { "RES 5,E", NULL },                // AB
        { "RES 5,H", NULL },                // AC
        { "RES 5,L", NULL },                // AD
        { "RES 5,(HL)", NULL },             // AE
        { "RES 5,A", NULL },                // AF

        { "RES 6,B", NULL },                // B0
        { "RES 6,C", NULL },                // B1
        { "RES 6,D", NULL },                // B2
        { "RES 6,E", NULL },                // B3
        { "RES 6,H", NULL },                // B4
        { "RES 6,L", NULL },                // B5
        { "RES 6,(HL)", NULL },             // B6
        { "RES 6,A", NULL },                // B7
        { "RES 7,B", NULL },                // B8
        { "RES 7,C", NULL },                // B9
        { "RES 7,D", NULL },                // BA
        { "RES 7,E", NULL },                // BB
        { "RES 7,H", NULL },                // BC
        { "RES 7,L", NULL },                // BD
        { "RES 7,(HL)", NULL },             // BE
        { "RES 7,A", NULL },                // BF

        { "SET 0,B", NULL },                // C0
        { "SET 0,C", NULL },                // C1
        { "SET 0,D", NULL },                // C2
        { "SET 0,E", NULL },                // C3
        { "SET 0,H", NULL },                // C4
        { "SET 0,L", NULL },                // C5
        { "SET 0,(HL)", NULL },             // C6
        { "SET 0,A", NULL },                // C7
        { "SET 1,B", NULL },                // C8
        { "SET 1,C", NULL },                // C9
        { "SET 1,D", NULL },                // CA
        { "SET 1,E", NULL },                // CB
        { "SET 1,H", NULL },                // CC
        { "SET 1,L", NULL },                // CD
        { "SET 1,(HL)", NULL },             // CE
        { "SET 1,A", NULL },                // CF

        { "SET 2,B", NULL },                // D0
        { "SET 2,C", NULL },                // D1
        { "SET 2,D", NULL },                // D2
        { "SET 2,E", NULL },                // D3
        { "SET 2,H", NULL },                // D4
        { "SET 2,L", NULL },                // D5
        { "SET 2,(HL)", NULL },             // D6
        { "SET 2,A", NULL },                // D7
        { "SET 3,B", NULL },                // D8
        { "SET 3,C", NULL },                // D9
        { "SET 3,D", NULL },                // DA
        { "SET 3,E", NULL },                // DB
        { "SET 3,H", NULL },                // DC
        { "SET 3,L", NULL },                // DD
        { "SET 3,(HL)", NULL },             // DE
        { "SET 3,A", NULL },                // DF

        { "SET 4,B", NULL },                // E0
        { "SET 4,C", NULL },                // E1
        { "SET 4,D", NULL },                // E2
        { "SET 4,E", NULL },                // E3
        { "SET 4,H", NULL },                // E4
        { "SET 4,L", NULL },                // E5
        { "SET 4,(HL)", NULL },             // E6
        { "SET 4,A", NULL },                // E7
        { "SET 5,B", NULL },                // E8
        { "SET 5,C", NULL },                // E9
        { "SET 5,D", NULL },                // EA
        { "SET 5,E", NULL },                // EB
        { "SET 5,H", NULL },                // EC
        { "SET 5,L", NULL },                // ED
        { "SET 5,(HL)", NULL },             // EE
        { "SET 5,A", NULL },                // EF

        { "SET 6,B", NULL },                // F0
        { "SET 6,C", NULL },                // F1
        { "SET 6,D", NULL },                // F2
        { "SET 6,E", NULL },                // F3
        { "SET 6,H", NULL },                // F4
        { "SET 6,L", NULL },                // F5
        { "SET 6,(HL)", NULL },             // F6
        { "SET 6,A", NULL },                // F7
        { "SET 7,B", NULL },                // F8
        { "SET 7,C", NULL },                // F9
        { "SET 7,D", NULL },                // FA
        { "SET 7,E", NULL },                // FB
        { "SET 7,H", NULL },                // FC
        { "SET 7,L", NULL },                // FD
        { "SET 7,(HL)", NULL },             // FE
        { "SET 7,A", NULL },                // FF
    };
#end
