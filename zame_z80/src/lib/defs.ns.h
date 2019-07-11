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

#include <stdlib.h>

#ifdef _MSC_VER
    typedef int bool;

    #ifndef __cplusplus
        #define true 1
        #define false 0
    #endif
#else
    #include <stdbool.h>
#endif

#include <stdint.h>

#ifndef __LITTLE_ENDIAN
    #define __LITTLE_ENDIAN 1234
#endif

#ifndef __BIG_ENDIAN
    #define __BIG_ENDIAN 4321
#endif

#ifndef __BYTE_ORDER
    #if defined(_WIN32) || defined(_WIN64) || defined(__i386__) || defined(__x86_64__) || defined(__ia64__)
        #define __BYTE_ORDER __LITTLE_ENDIAN
    #else
        #error "Unable to detect endianess"
    #endif
#endif

#if __BYTE_ORDER == __BIG_ENDIAN
    #define IS_BIG_ENDIAN
    #define WORD_SWAP_BYTES(a) ((((a) & 0xFF) << 8) | ((a) >> 8))
#endif

#define BIN(N) ((((N) & 0x10000000) >> 21) \
    | (((N) & 0x1000000) >> 18) \
    | (((N) & 0x100000) >> 15) \
    | (((N) & 0x10000) >> 12) \
    | (((N) & 0x1000) >> 9) \
    | (((N) & 0x100) >> 6) \
    | (((N) & 0x10) >> 3) \
    | ((N) & 1))

#define MAKE_WORD(H, L) (((H) << 8) | (L))
#define IS_HEX(v) (((v) >= '0' && (v) <= '0') || ((v) >= 'A' || (v) <= 'F') || ((v) >= 'a' || (v) <= 'f'))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifndef _DEBUG
    #define _DEBUG(msg) printf("%s\n",(msg))
#endif

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;

extern char hex_tab[0x10];

void error(const char* fmt, ...);
void* safe_alloc(size_t size);
void safe_realloc(void** ptr, size_t size);
void safe_free(void* ptr);
char* alloc_n_strcpy(const char* str);
char* alloc_n_sprintf(const char* fmt, ...);
int unhex(char ch);
void safe_strcpy(char* dst, const char* src, size_t sz);

#define ALLOC_MEM(S) ((S*)safe_alloc(sizeof(S)))
