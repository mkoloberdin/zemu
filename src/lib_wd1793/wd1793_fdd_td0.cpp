// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "defines.h"
#include "wd1793_fdd.h"

int C_Fdd::write_td0(DataWriterPtr& writer) {
    uint8_t zerosec[256] = { 0 };
    uint8_t td0hdr[12] = { 0 };
    unsigned cc;

    td0hdr[0] = 'T';
    td0hdr[1] = 'D';

    td0hdr[4] = 21;
    td0hdr[6] = 2;
    td0hdr[9] = (uint8_t)sides;

    if (*dsc) {
        td0hdr[7] = 0x80;
    }

    uint16_t crc16 = wd1793_crc16(td0hdr, 10);
    td0hdr[10] = (crc16 & 0xFF);
    td0hdr[11] = (crc16 >> 8);

    if (!writer->writeBlock(td0hdr, 12)) {
        DEBUG_MESSAGE("fwrite failed");
    }

    if (*dsc) {
        uint8_t inf[0x200 + 10] = { 0 };
        strcpy((char*)inf + 10, dsc);
        unsigned len = strlen(dsc) + 1;

        inf[2] = (len & 0xFF);
        inf[3] = ((len >> 8) & 0xFF);
        inf[4] = ((len >> 16) & 0xFF);
        inf[5] = (len >> 24);

        crc16 = wd1793_crc16(inf + 2, len + 8);
        inf[0] = (crc16 & 0xFF);
        inf[1] = (crc16 >> 8);

        if (!writer->writeBlock(inf, len + 10)) {
            DEBUG_MESSAGE("fwrite failed");
        }
    }

    for (cc = 0; cc < cyls; cc++) {
        for (unsigned s = 0; s < sides; s++) {
            t.seek(this, cc, s, LOAD_SECTORS);

            uint8_t bf[16];
            bf[0] = t.s;
            bf[1] = cc;
            bf[2] = s;
            bf[3] = (uint8_t)wd1793_crc16(bf, 3);

            if (!writer->writeBlock(bf, 4)) {
                DEBUG_MESSAGE("fwrite failed");
            }

            for (unsigned sec = 0; sec < t.s; sec++) {
                if (!t.hdr[sec].data) {
                    t.hdr[sec].data = zerosec;
                    t.hdr[sec].datlen = 256;
                    t.hdr[sec].l = 1;
                }

                bf[0] = t.hdr[sec].c;
                bf[1] = t.hdr[sec].s;
                bf[2] = t.hdr[sec].n;
                bf[3] = t.hdr[sec].l;
                bf[4] = 0; // flags
                bf[5] = (uint8_t)wd1793_crc16(t.hdr[sec].data, t.hdr[sec].datlen);
                bf[6] = ((t.hdr[sec].datlen + 1) & 0xFF);
                bf[7] = ((t.hdr[sec].datlen + 1) >> 8);
                bf[8] = 0; // compression type = none

                if (!writer->writeBlock(bf, 9)) {
                    DEBUG_MESSAGE("fwrite failed");
                }

                if (!writer->writeBlock(t.hdr[sec].data, t.hdr[sec].datlen)) {
                    return 0;
                }
            }
        }
    }

    uint8_t ccb[4] = { 0xFF, 0, 0, 0 };

    if (!writer->writeBlock(ccb, 4)) {
        return 0;
    }

    return 1;
}

unsigned lzh_unpack(uint8_t* src, unsigned size, uint8_t* buf);

int C_Fdd::read_td0() {
    if (WORD2(snbuf[0], snbuf[1]) == WORD2('t', 'd')) {
        // packed disk
        uint8_t* tmp = (uint8_t*)malloc(snapsize);

        if (!tmp) {
            StrikeError("Failed to allocate %zu bytes of memory", snapsize);
        }

        memcpy(tmp, snbuf + 12, snapsize - 12);
        snapsize = 12 + lzh_unpack(tmp, snapsize - 12, snbuf + 12);
        ::free(tmp);
    }

    uint8_t dscbuffer[sizeof(dsc)];
    dscbuffer[0] = 0;

    uint8_t* start = snbuf + 12;

    if (snbuf[7] & 0x80) {
        start += 10;
        unsigned len = WORD2(snbuf[14], snbuf[15]);
        start += len;

        if (len >= sizeof(dsc)) {
            len = sizeof(dsc) - 1;
        }

        memcpy(dscbuffer, snbuf + 12 + 10, len);
        dscbuffer[len] = 0;
    }

    uint8_t* td0_src = start;
    unsigned max_cyl = 0;
    unsigned max_head = 0;

    for (;;) {
        uint8_t s = *td0_src;

        if (s == 0xFF) {
            break;
        }

        max_cyl = std::max(max_cyl, (unsigned)td0_src[1]);
        max_head = std::max(max_head, (unsigned)td0_src[2]);
        td0_src += 4;

        for (; s; s--) {
            td0_src += 6;

            if (td0_src > snbuf + snapsize) {
                return 0;
            }

            td0_src += WORD2(td0_src[0], td0_src[1]) + 2;
        }
    }

    newdisk(max_cyl + 1, max_head + 1);
    memcpy(dsc, dscbuffer, sizeof dsc);

    td0_src = start;

    for (;;) {
        uint8_t t0[16384];
        uint8_t* dst = t0;
        uint8_t* trkh = td0_src;

        td0_src += 4;

        if (*trkh == 0xFF) {
            break;
        }

        t.seek(this, trkh[1], trkh[2], JUST_SEEK);
        t.s = trkh[0];

        for (unsigned se = 0; se < trkh[0]; se++) {
            unsigned size = 0x80 << td0_src[3];

            t.hdr[se].c = td0_src[0];
            t.hdr[se].s = td0_src[1];
            t.hdr[se].n = td0_src[2];
            t.hdr[se].l = td0_src[3];
            t.hdr[se].c1 = 0;
            t.hdr[se].c2 = 0;
            t.hdr[se].data = dst;
            td0_src += 6;

            unsigned src_size = WORD2(td0_src[0], td0_src[1]);
            td0_src += 2;

            uint8_t* end_packed_data = td0_src + src_size;
            memset(dst, 0, size);

            switch (*(td0_src++)) {
                case 0:
                    memcpy(dst, td0_src, src_size - 1);
                    break;

                case 1: {
                    unsigned n = WORD2(td0_src[0], td0_src[1]);
                    td0_src += 2;
                    uint16_t data = WORD2(td0_src[0], td0_src[1]);

                    for (unsigned i = 0; i < n; i++) {
                        dst[2 * i] = (data & 0xFF);
                        dst[2 * i + 1] = (data >> 8);
                    }

                    break;
                }

                case 2: {
                    uint16_t data;
                    uint8_t s;
                    uint8_t *d0 = dst;

                    do {
                        switch (*(td0_src++)) {
                            case 0:
                                for (s = *(td0_src++); s; s--) {
                                    *(dst++) = *(td0_src++);
                                }

                                break;

                            case 1:
                                s = *(td0_src++);
                                data = WORD2(td0_src[0], td0_src[1]);
                                td0_src += 2;

                                for (; s; s--) {
                                    *(dst++) = (data & 0xFF);
                                    *(dst++) = (data >> 8);
                                }

                                break;

                            default:
                                return 0; // bad TD0 file
                        }
                    } while (td0_src < end_packed_data);

                    dst = d0;
                    break;
                }

                default:
                    return 0; // bad TD0 file
            }

            dst += size;
            td0_src = end_packed_data;
        }

        t.format();
    }

    return 1;
}

// ------------------------------------------------------ LZH unpacker

uint8_t* lzh_packed_ptr;
uint8_t* lzh_packed_end;

int lzh_read_char(void) {
    if (lzh_packed_ptr < lzh_packed_end) {
        return (*(lzh_packed_ptr++));
    }

    return -1;
}

uint8_t lzh_d_code[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
    0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D,
    0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
    0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11,
    0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
    0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15,
    0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
    0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B,
    0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F,
    0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23,
    0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27,
    0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B,
    0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};

uint8_t lzh_d_len[256] = {
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
};

const int LZH_BUFFER_SIZE = 4096; // buffer size
const int LZH_LOOKAHEAD_BUFFER_SIZE = 60; // lookahead buffer size
const int LZH_THRESHOLD = 2;
const int LZH_MAX_CHAR = (256 - LZH_THRESHOLD + LZH_LOOKAHEAD_BUFFER_SIZE); // kinds of characters (character code = 0..LZH_MAX_CHAR-1)
const int LZH_TABLE_SIZE = (LZH_MAX_CHAR * 2 - 1); // size of table
const int LZH_ROOT_POSITION = (LZH_TABLE_SIZE - 1); // position of root
const int LZH_MAX_FREQ = 0x8000; // updates tree when the root frequency comes to this value.

uint8_t lzh_text_buf[LZH_BUFFER_SIZE + LZH_LOOKAHEAD_BUFFER_SIZE - 1];
uint16_t lzh_freq_tab[LZH_TABLE_SIZE + 1]; // frequency table

// pointers to parent nodes, except for the
// elements [LZH_TABLE_SIZE..LZH_TABLE_SIZE + LZH_MAX_CHAR - 1] which are used to get
// the positions of leaves corresponding to the codes.
short lzh_parent_ptrs[LZH_TABLE_SIZE + LZH_MAX_CHAR];

short lzh_child_ptrs[LZH_TABLE_SIZE]; // pointers to child nodes (lzh_child_ptrs[], lzh_child_ptrs[] + 1)
int lzh_unpack_pos;
unsigned lzh_get_buf;
uint8_t lzh_get_len;

// get one bit
int lzh_get_bit(void) {
    int i;

    while (lzh_get_len <= 8) {
        if ((i = lzh_read_char()) == -1) {
            i = 0;
        }

        lzh_get_buf |= i << (8 - lzh_get_len);
        lzh_get_len += 8;
    }

    i = lzh_get_buf;
    lzh_get_buf <<= 1;
    lzh_get_len--;

    return ((i >> 15) & 1);
}

// get one byte
int lzh_get_byte(void) {
    unsigned i;

    while (lzh_get_len <= 8) {
        if ((i = lzh_read_char()) == ~(unsigned)0) {
            i = 0;
        }

        lzh_get_buf |= i << (8 - lzh_get_len);
        lzh_get_len += 8;
    }

    i = lzh_get_buf;
    lzh_get_buf <<= 8;
    lzh_get_len -= 8;

    return (i >> 8) & 0xFF;
}

void lzh_unpack_start_huff(void) {
    int i;
    int j;

    lzh_get_buf = 0;
    lzh_get_len = 0;

    for (i = 0; i < LZH_MAX_CHAR; i++) {
        lzh_freq_tab[i] = 1;
        lzh_child_ptrs[i] = i + LZH_TABLE_SIZE;
        lzh_parent_ptrs[i + LZH_TABLE_SIZE] = i;
    }

    i = 0;
    j = LZH_MAX_CHAR;

    while (j <= LZH_ROOT_POSITION) {
        lzh_freq_tab[j] = lzh_freq_tab[i] + lzh_freq_tab[i + 1];
        lzh_child_ptrs[j] = i;
        lzh_parent_ptrs[i] = lzh_parent_ptrs[i + 1] = j;
        i += 2;
        j++;
    }

    lzh_freq_tab[LZH_TABLE_SIZE] = 0xffff;
    lzh_parent_ptrs[LZH_ROOT_POSITION] = 0;

    for (i = 0; i < LZH_BUFFER_SIZE - LZH_LOOKAHEAD_BUFFER_SIZE; i++) {
        lzh_text_buf[i] = ' ';
    }

    lzh_unpack_pos = LZH_BUFFER_SIZE - LZH_LOOKAHEAD_BUFFER_SIZE;
}

// reconstruction of tree
void lzh_reconst(void) {
    int i;
    int k;

    // collect leaf nodes in the first half of the table
    // and replace the freq by (freq + 1) / 2.
    int j = 0;

    for (i = 0; i < LZH_TABLE_SIZE; i++) {
        if (lzh_child_ptrs[i] >= LZH_TABLE_SIZE) {
            lzh_freq_tab[j] = (lzh_freq_tab[i] + 1) / 2;
            lzh_child_ptrs[j] = lzh_child_ptrs[i];
            j++;
        }
    }

    // begin constructing tree by connecting children
    for (i = 0, j = LZH_MAX_CHAR; j < LZH_TABLE_SIZE; i += 2, j++) {
        k = i + 1;
        int f = lzh_freq_tab[j] = lzh_freq_tab[i] + lzh_freq_tab[k];

        for (k = j - 1; f < lzh_freq_tab[k]; k--);
        k++;

        int l = (j - k) * sizeof(*lzh_freq_tab);

        memmove(&lzh_freq_tab[k + 1], &lzh_freq_tab[k], l);
        lzh_freq_tab[k] = f;

        memmove(&lzh_child_ptrs[k + 1], &lzh_child_ptrs[k], l);
        lzh_child_ptrs[k] = i;
    }

    // connect lzh_parent_ptrs
    for (i = 0; i < LZH_TABLE_SIZE; i++) {
        if ((k = lzh_child_ptrs[i]) >= LZH_TABLE_SIZE) {
            lzh_parent_ptrs[k] = i;
        } else {
            lzh_parent_ptrs[k] = lzh_parent_ptrs[k + 1] = i;
        }
    }
}

// increment frequency of given code by one, and update tree
void lzh_update(int c) {
    if (lzh_freq_tab[LZH_ROOT_POSITION] == LZH_MAX_FREQ) {
        lzh_reconst();
    }

    c = lzh_parent_ptrs[c + LZH_TABLE_SIZE];

    do {
        int k = ++lzh_freq_tab[c];
        int l = c + 1;

        // if the order is disturbed, exchange nodes
        if (k > lzh_freq_tab[l]) {
            while (k > lzh_freq_tab[++l]);
            l--;

            lzh_freq_tab[c] = lzh_freq_tab[l];
            lzh_freq_tab[l] = k;

            int i = lzh_child_ptrs[c];
            lzh_parent_ptrs[i] = l;

            if (i < LZH_TABLE_SIZE) {
                lzh_parent_ptrs[i + 1] = l;
            }

            int j = lzh_child_ptrs[l];
            lzh_child_ptrs[l] = i;
            lzh_parent_ptrs[j] = c;

            if (j < LZH_TABLE_SIZE) {
                lzh_parent_ptrs[j + 1] = c;
            }

            lzh_child_ptrs[c] = j;
            c = l;
        }
    } while ((c = lzh_parent_ptrs[c]) != 0); // repeat up to root
}

int lzh_decode_char(void) {
    int c = lzh_child_ptrs[LZH_ROOT_POSITION];

    // travel from root to leaf,
    // choosing the smaller child node (lzh_child_ptrs[]) if the read bit is 0,
    // the bigger (lzh_child_ptrs[]+1} if 1
    while (c < LZH_TABLE_SIZE) {
        c = lzh_child_ptrs[c + lzh_get_bit()];
    }

    c -= LZH_TABLE_SIZE;
    lzh_update(c);

    return c;
}

int lzh_decode_position(void) {
    // recover upper 6 bits from table
    int i = lzh_get_byte();
    int c = (int)lzh_d_code[i] << 6;
    int j = lzh_d_len[i];

    // read lower 6 bits verbatim
    j -= 2;

    while (j--) {
        i = (i << 1) + lzh_get_bit();
    }

    return c | (i & 0x3f);
}

unsigned lzh_unpack(uint8_t* src, unsigned size, uint8_t* buf) {
    lzh_packed_ptr = src;
    lzh_packed_end = src + size;

    lzh_unpack_start_huff();
    unsigned count = 0;

    // while (count < textsize) // textsize - sizeof unpacked data
    while (lzh_packed_ptr < lzh_packed_end) {
        int c = lzh_decode_char();

        if (c < 256) {
            *(buf++) = c;
            lzh_text_buf[lzh_unpack_pos++] = c;
            lzh_unpack_pos &= (LZH_BUFFER_SIZE - 1);
            count++;
        } else {
            int i = (lzh_unpack_pos - lzh_decode_position() - 1) & (LZH_BUFFER_SIZE - 1);
            int j = c - 255 + LZH_THRESHOLD;

            for (int k = 0; k < j; k++) {
                c = lzh_text_buf[(i + k) & (LZH_BUFFER_SIZE - 1)];
                *(buf++) = c;
                lzh_text_buf[lzh_unpack_pos++] = c;
                lzh_unpack_pos &= (LZH_BUFFER_SIZE - 1);
                count++;
            }
        }
    }

    return count;
}
