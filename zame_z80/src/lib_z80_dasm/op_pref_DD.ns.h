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
    const s_DasmItem ::optable_DD[0x100] = {
        { "*NOP", NULL },                   // 00
        { "*LD BC,[NN]", NULL },            // 01
        { "*LD (BC),A", NULL },             // 02
        { "*INC BC", NULL },                // 03
        { "*INC B", NULL },                 // 04
        { "*DEC B", NULL },                 // 05
        { "*LD B,[N]", NULL },              // 06
        { "*RLCA", NULL },                  // 07
        { "*EX AF,AF'", NULL },             // 08
        { "ADD IX,BC", NULL },              // 09
        { "*LD A,(BC)", NULL },             // 0A
        { "*DEC BC", NULL },                // 0B
        { "*INC C", NULL },                 // 0C
        { "*DEC C", NULL },                 // 0D
        { "*LD C,[N]", NULL },              // 0E
        { "*RRCA", NULL },                  // 0F

        { "*DJNZ [O]", NULL },              // 10
        { "*LD DE,[NN]", NULL },            // 11
        { "*LD (DE),A", NULL },             // 12
        { "*INC DE", NULL },                // 13
        { "*INC D", NULL },                 // 14
        { "*DEC D", NULL },                 // 15
        { "*LD D,[N]", NULL },              // 16
        { "*RLA", NULL },                   // 17
        { "*JR [O]", NULL },                // 18
        { "ADD IX,DE", NULL },              // 19
        { "*LD A,(DE)", NULL },             // 1A
        { "*DEC DE", NULL },                // 1B
        { "*INC E", NULL },                 // 1C
        { "*DEC E", NULL },                 // 1D
        { "*LD E,[N]", NULL },              // 1E
        { "*RRA", NULL },                   // 1F

        { "*JR NZ,[O]", NULL },             // 20
        { "LD IX,[NN]", NULL },             // 21
        { "LD ([NN]),IX", NULL },           // 22
        { "INC IX", NULL },                 // 23
        { "INC IXH", NULL },                // 24
        { "DEC IXH", NULL },                // 25
        { "LD IXH,[N]", NULL },             // 26
        { "*DAA", NULL },                   // 27
        { "*JR Z,[O]", NULL },              // 28
        { "ADD IX,IX", NULL },              // 29
        { "LD IX,([NN])", NULL },           // 2A
        { "DEC IX", NULL },                 // 2B
        { "INC IXL", NULL },                // 2C
        { "DEC IXL", NULL },                // 2D
        { "LD IXL,[N]", NULL },             // 2E
        { "*CPL", NULL },                   // 2F

        { "*JR NC,[O]", NULL },             // 30
        { "*LD SP,[NN]", NULL },            // 31
        { "*LD ([NN]),A", NULL },           // 32
        { "*INC SP", NULL },                // 33
        { "INC (IX[SO])", NULL },           // 34
        { "DEC (IX[SO])", NULL },           // 35
        { "LD (IX[SO]),[N]", NULL },        // 36
        { "*SCF", NULL },                   // 37
        { "*JR C,[O]", NULL },              // 38
        { "ADD IX,SP", NULL },              // 39
        { "*LD A,([NN])", NULL },           // 3A
        { "*DEC SP", NULL },                // 3B
        { "*INC A", NULL },                 // 3C
        { "*DEC A", NULL },                 // 3D
        { "*LD A,[N]", NULL },              // 3E
        { "*CCF", NULL },                   // 3F

        { "*LD B,B", NULL },                // 40
        { "*LD B,C", NULL },                // 41
        { "*LD B,D", NULL },                // 42
        { "*LD B,E", NULL },                // 43
        { "LD B,IXH", NULL },               // 44
        { "LD B,IXL", NULL },               // 45
        { "LD B,(IX[SO])", NULL },          // 46
        { "*LD B,A", NULL },                // 47
        { "*LD C,B", NULL },                // 48
        { "*LD C,C", NULL },                // 49
        { "*LD C,D", NULL },                // 4A
        { "*LD C,E", NULL },                // 4B
        { "LD C,IXH", NULL },               // 4C
        { "LD C,IXL", NULL },               // 4D
        { "LD C,(IX[SO])", NULL },          // 4E
        { "*LD C,A", NULL },                // 4F

        { "*LD D,B", NULL },                // 50
        { "*LD D,C", NULL },                // 51
        { "*LD D,D", NULL },                // 52
        { "*LD D,E", NULL },                // 53
        { "LD D,IXH", NULL },               // 54
        { "LD D,IXL", NULL },               // 55
        { "LD D,(IX[SO])", NULL },          // 56
        { "*LD D,A", NULL },                // 57
        { "*LD E,B", NULL },                // 58
        { "*LD E,C", NULL },                // 59
        { "*LD E,D", NULL },                // 5A
        { "*LD E,E", NULL },                // 5B
        { "LD E,IXH", NULL },               // 5C
        { "LD E,IXL", NULL },               // 5D
        { "LD E,(IX[SO])", NULL },          // 5E
        { "*LD E,A", NULL },                // 5F

        { "LD IXH,B", NULL },               // 60
        { "LD IXH,C", NULL },               // 61
        { "LD IXH,D", NULL },               // 62
        { "LD IXH,E", NULL },               // 63
        { "LD IXH,H", NULL },               // 64
        { "LD IXH,L", NULL },               // 65
        { "LD H,(IX[SO])", NULL },          // 66
        { "LD IXH,A", NULL },               // 67
        { "LD IXL,B", NULL },               // 68
        { "LD IXL,C", NULL },               // 69
        { "LD IXL,D", NULL },               // 6A
        { "LD IXL,E", NULL },               // 6B
        { "LD IXL,H", NULL },               // 6C
        { "LD IXL,L", NULL },               // 6D
        { "LD L,(IX[SO])", NULL },          // 6E
        { "LD IXL,A", NULL },               // 6F

        { "LD (IX[SO]),B", NULL },          // 70
        { "LD (IX[SO]),C", NULL },          // 71
        { "LD (IX[SO]),D", NULL },          // 72
        { "LD (IX[SO]),E", NULL },          // 73
        { "LD (IX[SO]),H", NULL },          // 74
        { "LD (IX[SO]),L", NULL },          // 75
        { "*HALT", NULL },                  // 76
        { "LD (IX[SO]),A", NULL },          // 77
        { "*LD A,B", NULL },                // 78
        { "*LD A,C", NULL },                // 79
        { "*LD A,D", NULL },                // 7A
        { "*LD A,E", NULL },                // 7B
        { "LD A,IXH", NULL },               // 7C
        { "LD A,IXL", NULL },               // 7D
        { "LD A,(IX[SO])", NULL },          // 7E
        { "*LD A,A", NULL },                // 7F

        { "*ADD A,B", NULL },               // 80
        { "*ADD A,C", NULL },               // 81
        { "*ADD A,D", NULL },               // 82
        { "*ADD A,E", NULL },               // 83
        { "ADD A,IXH", NULL },              // 84
        { "ADD A,IXL", NULL },              // 85
        { "ADD A,(IX[SO])", NULL },         // 86
        { "*ADD A,A", NULL },               // 87
        { "*ADC A,B", NULL },               // 88
        { "*ADC A,C", NULL },               // 89
        { "*ADC A,D", NULL },               // 8A
        { "*ADC A,E", NULL },               // 8B
        { "ADC A,IXH", NULL },              // 8C
        { "ADC A,IXL", NULL },              // 8D
        { "ADC A,(IX[SO])", NULL },         // 8E
        { "*ADC A,A", NULL },               // 8F

        { "*SUB B", NULL },                 // 90
        { "*SUB C", NULL },                 // 91
        { "*SUB D", NULL },                 // 92
        { "*SUB E", NULL },                 // 93
        { "SUB IXH", NULL },                // 94
        { "SUB IXL", NULL },                // 95
        { "SUB (IX[SO])", NULL },           // 96
        { "*SUB A", NULL },                 // 97
        { "*SBC A,B", NULL },               // 98
        { "*SBC A,C", NULL },               // 99
        { "*SBC A,D", NULL },               // 9A
        { "*SBC A,E", NULL },               // 9B
        { "SBC A,IXH", NULL },              // 9C
        { "SBC A,IXL", NULL },              // 9D
        { "SBC A,(IX[SO])", NULL },         // 9E
        { "*SBC A,A", NULL },               // 9F

        { "*AND B", NULL },                 // A0
        { "*AND C", NULL },                 // A1
        { "*AND D", NULL },                 // A2
        { "*AND E", NULL },                 // A3
        { "AND IXH", NULL },                // A4
        { "AND IXL", NULL },                // A5
        { "AND (IX[SO])", NULL },           // A6
        { "*AND A", NULL },                 // A7
        { "*XOR B", NULL },                 // A8
        { "*XOR C", NULL },                 // A9
        { "*XOR D", NULL },                 // AA
        { "*XOR E", NULL },                 // AB
        { "XOR IXH", NULL },                // AC
        { "XOR IXL", NULL },                // AD
        { "XOR (IX[SO])", NULL },           // AE
        { "*XOR A", NULL },                 // AF

        { "*OR B", NULL },                  // B0
        { "*OR C", NULL },                  // B1
        { "*OR D", NULL },                  // B2
        { "*OR E", NULL },                  // B3
        { "OR IXH", NULL },                 // B4
        { "OR IXL", NULL },                 // B5
        { "OR (IX[SO])", NULL },            // B6
        { "*OR A", NULL },                  // B7
        { "*CP B", NULL },                  // B8
        { "*CP C", NULL },                  // B9
        { "*CP D", NULL },                  // BA
        { "*CP E", NULL },                  // BB
        { "CP IXH", NULL },                 // BC
        { "CP IXL", NULL },                 // BD
        { "CP (IX[SO])", NULL },            // BE
        { "*CP A", NULL },                  // BF

        { "*RET NZ", NULL },                // C0
        { "*POP BC", NULL },                // C1
        { "*JP NZ,[NN]", NULL },            // C2
        { "*JP [NN]", NULL },               // C3
        { "*CALL NZ,[NN]", NULL },          // C4
        { "*PUSH BC", NULL },               // C5
        { "*ADD A,[N]", NULL },             // C6
        { "*RST #00", NULL },               // C7
        { "*RET Z", NULL },                 // C8
        { "*RET", NULL },                   // C9
        { "*JP Z,[NN]", NULL },             // CA
        { NULL, ::optable_DD_CB },          // CB
        { "*CALL Z,[NN]", NULL },           // CC
        { "*CALL [NN]", NULL },             // CD
        { "*ADC A,[N]", NULL },             // CE
        { "*RST #08", NULL },               // CF

        { "*RET NC", NULL },                // D0
        { "*POP DE", NULL },                // D1
        { "*JP NC,[NN]", NULL },            // D2
        { "*OUT ([N]),A", NULL },           // D3
        { "*CALL NC,[NN]", NULL },          // D4
        { "*PUSH DE", NULL },               // D5
        { "*SUB [N]", NULL },               // D6
        { "*RST #10", NULL },               // D7
        { "*RET C", NULL },                 // D8
        { "*EXX", NULL },                   // D9
        { "*JP C,[NN]", NULL },             // DA
        { "*IN A,([N])", NULL },            // DB
        { "*CALL C,[NN]", NULL },           // DC
        { "**NOP", ::optable_DD },          // DD
        { "*SBC A,[N]", NULL },             // DE
        { "*RST #18", NULL },               // DF

        { "*RET PO", NULL },                // E0
        { "POP IX", NULL },                 // E1
        { "*JP PO,[NN]", NULL },            // E2
        { "EX (SP),IX", NULL },             // E3
        { "*CALL PO,[NN]", NULL },          // E4
        { "PUSH IX", NULL },                // E5
        { "*AND [N]", NULL },               // E6
        { "*RST #20", NULL },               // E7
        { "*RET PE", NULL },                // E8
        { "JP IX", NULL },                  // E9
        { "*JP PE,[NN]", NULL },            // EA
        { "*EX DE,HL", NULL },              // EB
        { "*CALL PE,[NN]", NULL },          // EC
        { "**NOP", ::optable_ED },          // ED
        { "*XOR [N]", NULL },               // EE
        { "*RST #28", NULL },               // EF

        { "*RET P", NULL },                 // F0
        { "*POP AF", NULL },                // F1
        { "*JP P,[NN]", NULL },             // F2
        { "*DI", NULL },                    // F3
        { "*CALL P,[NN]", NULL },           // F4
        { "*PUSH AF", NULL },               // F5
        { "*OR [N]", NULL },                // F6
        { "*RST #30", NULL },               // F7
        { "*RET M", NULL },                 // F8
        { "LD SP,IX", NULL },               // F9
        { "*JP M,[NN]", NULL },             // FA
        { "*EI", NULL },                    // FB
        { "*CALL M,[NN]", NULL },           // FC
        { "**NOP", ::optable_FD },          // FD
        { "*CP [N]", NULL },                // FE
        { "*RST #38", NULL },               // FF
    };
#end
