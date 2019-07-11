// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

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

#include "dasm.h"
#include <stdio.h>
#include <string.h>

extern const s_DasmItem Dasm::optable_00[0x100];
extern const s_DasmItem Dasm::optable_CB[0x100];
extern const s_DasmItem Dasm::optable_DD[0x100];
extern const s_DasmItem Dasm::optable_DD_CB[0x100];
extern const s_DasmItem Dasm::optable_ED[0x100];
extern const s_DasmItem Dasm::optable_FD[0x100];
extern const s_DasmItem Dasm::optable_FD_CB[0x100];

#include "op_pref_00.h"
#include "op_pref_CB.h"
#include "op_pref_DD.h"
#include "op_pref_DD_CB.h"
#include "op_pref_ED.h"
#include "op_pref_FD.h"
#include "op_pref_FD_CB.h"

#namespace Dasm
    unsigned ::disassemble(
        char* buffer,
        unsigned buffer_size,
        word addr,
        ::t_read ptr_read,
        void* data_read
    ) {
        if (!buffer_size) {
            return 0;
        }

        const s_DasmItem* optable = ::optable_00;
        const char* opcode_ptr;
        unsigned size = 0;
        int8_t offset = 0;

        for (;;) {
            const s_DasmItem* item = &optable[(unsigned)ptr_read(addr, data_read)];
            addr++;
            size++;

            if (!item->transfer) {
                opcode_ptr = item->opcode;
                break;
            }

            if (item->opcode) {
                size--;
                opcode_ptr = item->opcode;
                break;
            }

            optable = item->transfer;

            if (optable == ::optable_DD_CB || optable == ::optable_FD_CB) {
                offset = ptr_read(addr, data_read);
                addr++;
                size++;
            }
        }

        buffer_size--; // reserve space for 0-terminator

        while (*opcode_ptr && buffer_size) {
            if (*opcode_ptr != '[') {
                *(buffer++) = *(opcode_ptr++);
                buffer_size--;
                continue;
            }

            opcode_ptr++;

            char tmp[0x100];
            char* ptr = tmp;

            while (*opcode_ptr && *opcode_ptr!=']') {
                *(ptr++) = *(opcode_ptr++);
            }

            opcode_ptr++;
            *ptr = 0;

            if (!strcmp(tmp, "N")) {
                sprintf(tmp, "#%02X", (unsigned)ptr_read(addr, data_read));
                addr++;
                size++;
            } else if (!strcmp(tmp, "NN")) {
                sprintf(tmp, "#%04X", (unsigned)ptr_read(addr, data_read) | (((unsigned)ptr_read(addr + 1, data_read)) << 8));
                addr += 2;
                size += 2;
            } else if (!strcmp(tmp, "O")) {
                sprintf(tmp, "#%04X", (word)((addr + 1) + (int8_t)ptr_read(addr, data_read)));
                addr++;
                size++;
            } else if (!strcmp(tmp, "SO")) {
                if (optable != ::optable_DD_CB && optable != ::optable_FD_CB) {
                    offset = ptr_read(addr, data_read);
                    addr++;
                    size++;
                }

                if (offset < 0) {
                    sprintf(tmp, "-#%02X", (unsigned)(- offset));
                } else {
                    sprintf(tmp, "+#%02X", offset);
                }
            }

            ptr = tmp;

            while (*ptr && buffer_size) {
                *(buffer++) = *(ptr++);
                buffer_size--;
            }
        }

        *buffer = 0;
        return size;
    }
#end
