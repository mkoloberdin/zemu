// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdlib.h>
#include <string.h>
#include "wd1793_fdd.h"
#include "defines.h"

int C_Fdd::read_udi() {
    free();

    unsigned c;
    unsigned s;
    unsigned mem = 0;
    uint8_t *ptr = snbuf + 0x10;

    for (c = 0; c <= snbuf[9]; c++) {
        for (s = 0; s <= snbuf[10]; s++) {
            if (*ptr) {
                return 0;
            }

            unsigned sz = WORD2(ptr[1], ptr[2]);
            sz += (sz / 8) + ((sz & 7) ? 1 : 0);
            mem += sz;
            ptr += 3 + sz;

            if (ptr > snbuf + snapsize) {
                return 0;
            }
        }
    }

    cyls = snbuf[9] + 1;
    sides = snbuf[10] + 1;
    rawsize = align_by(mem, 4096);
    rawdata = (uint8_t*)malloc(rawsize);

    if (!rawdata) {
        StrikeError("Failed to allocate %zu bytes of memory", rawsize);
    }

    ptr = snbuf + 0x10;
    uint8_t* dst = rawdata;

    for (c = 0; c < cyls; c++) {
        for (s = 0; s < sides; s++) {
            unsigned sz = WORD2(ptr[1], ptr[2]);
            trklen[c][s] = sz;
            trkd[c][s] = dst;
            trki[c][s] = dst + sz;
            sz += (sz / 8) + ((sz & 7) ? 1 : 0);

            memcpy(dst, ptr + 3, sz);
            ptr += 3 + sz;
            dst += sz;
        }
    }

    if (snbuf[11] & 1) {
        strcpy(dsc, (char*)ptr);
    }

    return 1;
}

int C_Fdd::write_udi(DataWriterPtr& writer) {
    memset(snbuf, 0, 0x10); //-V512

    snbuf[0] = 'U';
    snbuf[1] = 'D';
    snbuf[2] = 'I';
    snbuf[3] = '!';
    snbuf[8] = 0;
    snbuf[9] = cyls - 1;
    snbuf[10] = sides - 1;
    snbuf[11] = 0;
    *((unsigned *)(snbuf + 12)) = 0; //-V1032

    uint8_t* dst = snbuf + 0x10;

    for (unsigned c = 0; c < cyls; c++) {
        for (unsigned s = 0; s < sides; s++) {
            *(dst++) = 0;

            unsigned len = trklen[c][s];
            *(dst++) = (len & 0xFF);
            *(dst++) = (len >> 8);

            memcpy(dst, trkd[c][s], len);
            dst += len;

            len = (len + 7) / 8;

            memcpy(dst, trki[c][s], len);
            dst += len;
        }
    }

    if (*dsc) {
        strcpy((char*)dst, dsc);
        dst += strlen(dsc) + 1;
        snbuf[11] = 1;
    }

    unsigned sz = dst - snbuf;
    snbuf[4] = (sz & 0xFF);
    snbuf[5] = ((sz >> 8) & 0xFF);
    snbuf[6] = ((sz >> 16) & 0xFF);
    snbuf[7] = (sz >> 24);

    int32_t crc = wd1793_crc32(-1, snbuf, dst - snbuf);

    *(dst++) = (((uint32_t)crc) & 0xFF);
    *(dst++) = ((((uint32_t)crc) >> 8) & 0xFF);
    *(dst++) = ((((uint32_t)crc) >> 16) & 0xFF);
    *(dst++) = (((uint32_t)crc) >> 24);

    if (!writer->writeBlock(snbuf, dst - snbuf)) {
        return 0;
    }

    return 1;
}
