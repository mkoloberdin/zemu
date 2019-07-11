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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "defs.h"

char hex_tab[0x10] = "0123456789ABCDEF";

void error(const char* fmt, ...) {
    va_list arg_ptr;
    char buf[0x1000];

    va_start(arg_ptr, fmt);
    vsnprintf(buf, sizeof(buf), fmt, arg_ptr);
    va_end(arg_ptr);

    printf("[E] %s\n", buf);
    exit(0);
}

void* safe_alloc(size_t size) {
    void* ptr = malloc(size);

    if (!ptr) {
        error("Can't allocate %d bytes", (int)size);
    }

    return ptr;
}

void safe_realloc(void** ptr, size_t size) {
    void* new_ptr = realloc(*ptr, size);

    if (size && !(*ptr)) {
        error("Can't allocate %d bytes", (int)size);
    }

    *ptr = new_ptr;
}

void safe_free(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}

char* alloc_n_strcpy(const char* str) {
    char* res = safe_alloc(strlen(str) + 1);
    strcpy(res, str);
    return res;
}

char* alloc_n_sprintf(const char* fmt, ...) {
    va_list arg_ptr;
    char buf[0x1000];

    va_start(arg_ptr, fmt);
    vsnprintf(buf, sizeof(buf), fmt, arg_ptr);
    va_end(arg_ptr);

    char* res = safe_alloc(strlen(buf) + 1);
    strcpy(res, buf);
    return res;
}

int unhex(char ch) {
    switch (ch) {
        case '1':
            return 1;

        case '2':
            return 2;

        case '3':
            return 3;

        case '4':
            return 4;

        case '5':
            return 5;

        case '6':
            return 6;

        case '7':
            return 7;

        case '8':
            return 8;

        case '9':
            return 9;

        case 'a': // fallthrough
        case 'A':
            return 10;

        case 'b': // fallthrough
        case 'B':
            return 11;

        case 'c': // fallthrough
        case 'C':
            return 12;

        case 'd': // fallthrough
        case 'D':
            return 13;

        case 'e': // fallthrough
        case 'E':
            return 14;

        case 'f': // fallthrough
        case 'F':
            return 15;

        default:
            return 0;
    }
}

//
// man strncpy:
// If there is no null byte among the first n bytes of src, the string placed in dest will not be null terminated.
//
void safe_strcpy(char* dst, const char* src, size_t sz) {
    strncpy(dst, src, sz);
    dst[sz - 1] = '\0';
}
