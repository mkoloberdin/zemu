#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "wd1793_fdd.h"

int C_Fdd::write_td0(FILE* ff) {
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

    if (fwrite(td0hdr, 1, 12, ff) != 12) {
        DEBUG_MESSAGE("fwrite failed");
    }

    if (*dsc) {
        uint8_t inf[0x200] = { 0 };
        strcpy((char*)inf + 10, dsc);
        unsigned len = strlen(dsc) + 1;

        inf[2] = (len & 0xFF);
        inf[3] = ((len >> 8) & 0xFF);
        inf[4] = ((len >> 16) & 0xFF);
        inf[5] = (len >> 24);

        crc16 = wd1793_crc16(inf + 2, len + 8);
        inf[0] = (crc16 & 0xFF);
        inf[1] = (crc16 >> 8);

        if (fwrite(inf, 1, len + 10, ff) != (len + 10)) {
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

            if (fwrite(bf, 1, 4, ff) != 4) {
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

                if (fwrite(bf, 1, 9, ff) != 9) {
                    DEBUG_MESSAGE("fwrite failed");
                }

                if (fwrite(t.hdr[sec].data, 1, t.hdr[sec].datlen, ff) != t.hdr[sec].datlen) {
                    return 0;
                }
            }
        }
    }

    uint8_t ccb[4] = { 0xFF, 0, 0, 0 };

    if (fwrite(ccb, 1, 4, ff) != 4) {
        return 0;
    }

    return 1;
}

unsigned unpack_lzh(uint8_t* src, unsigned size, uint8_t* buf);

int C_Fdd::read_td0() {
    if (WORD2(snbuf[0], snbuf[1]) == WORD2('t', 'd')) {
        // packed disk
        uint8_t* tmp = (uint8_t*)malloc(snapsize);
        memcpy(tmp, snbuf + 12, snapsize - 12);
        snapsize = 12 + unpack_lzh(tmp, snapsize - 12, snbuf + 12);
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

        max_cyl = zmax(max_cyl, td0_src[1]);
        max_head = zmax(max_head, td0_src[2]);
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

                    for (unsigned i = 0; i < n; i--) {
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

uint8_t* packed_ptr;
uint8_t* packed_end;

int readChar(void) {
    if (packed_ptr < packed_end) {
        return (*(packed_ptr++));
    }

    return -1;
}

uint8_t d_code[256] = {
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

uint8_t d_len[256] = {
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

const int N = 4096; // buffer size
const int F = 60; // lookahead buffer size
const int THRESHOLD = 2;
const int NIL = N; // leaf of tree

uint8_t text_buf[N + F - 1];

const int N_CHAR = (256 - THRESHOLD + F); // kinds of characters (character code = 0..N_CHAR-1)
const int T = (N_CHAR * 2 - 1); // size of table
const int R = (T - 1); // position of root
const int MAX_FREQ = 0x8000; // updates tree when the root frequency comes to this value.

uint16_t freq[T + 1]; // frequency table

// pointers to parent nodes, except for the
// elements [T..T + N_CHAR - 1] which are used to get
// the positions of leaves corresponding to the codes.
short prnt[T + N_CHAR];

short son[T]; // pointers to child nodes (son[], son[] + 1)

int r;

unsigned getbuf;
uint8_t getlen;

// get one bit
int GetBit(void) {
    int i;

    while (getlen <= 8) {
        if ((i = readChar()) == -1) {
            i = 0;
        }

        getbuf |= i << (8 - getlen);
        getlen += 8;
    }

    i = getbuf;
    getbuf <<= 1;
    getlen--;

    return ((i >> 15) & 1);
}

// get one byte
int GetByte(void) {
    unsigned i;

    while (getlen <= 8) {
        if ((i = readChar()) == ~(unsigned)0) {
            i = 0;
        }

        getbuf |= i << (8 - getlen);
        getlen += 8;
    }

    i = getbuf;
    getbuf <<= 8;
    getlen -= 8;

    return (i >> 8) & 0xFF;
}

void StartHuff(void) {
    int i;
    int j;

    getbuf = 0;
    getlen = 0;

    for (i = 0; i < N_CHAR; i++) {
        freq[i] = 1;
        son[i] = i + T;
        prnt[i + T] = i;
    }

    i = 0;
    j = N_CHAR;

    while (j <= R) {
        freq[j] = freq[i] + freq[i + 1];
        son[j] = i;
        prnt[i] = prnt[i + 1] = j;
        i += 2;
        j++;
    }

    freq[T] = 0xffff;
    prnt[R] = 0;

    for (i = 0; i < N - F; i++) {
        text_buf[i] = ' ';
    }

    r = N - F;
}

// reconstruction of tree
void reconst(void)
{
    int i;
    int j;
    int k;
    int f;
    int l;

    // collect leaf nodes in the first half of the table
    // and replace the freq by (freq + 1) / 2.
    j = 0;

    for (i = 0; i < T; i++) {
        if (son[i] >= T) {
            freq[j] = (freq[i] + 1) / 2;
            son[j] = son[i];
            j++;
        }
    }

    // begin constructing tree by connecting sons
    for (i = 0, j = N_CHAR; j < T; i += 2, j++) {
        k = i + 1;
        f = freq[j] = freq[i] + freq[k];

        for (k = j - 1; f < freq[k]; k--);
        k++;

        l = (j - k) * sizeof(*freq);

        memmove(&freq[k + 1], &freq[k], l);
        freq[k] = f;

        memmove(&son[k + 1], &son[k], l);
        son[k] = i;
    }

    // connect prnt
    for (i = 0; i < T; i++) {
        if ((k = son[i]) >= T) {
            prnt[k] = i;
        } else {
            prnt[k] = prnt[k + 1] = i;
        }
    }
}

// increment frequency of given code by one, and update tree
void update(int c) {
    int i;
    int j;
    int k;
    int l;

    if (freq[R] == MAX_FREQ) {
        reconst();
    }

    c = prnt[c + T];

    do {
        k = ++freq[c];

        // if the order is disturbed, exchange nodes
        if (k > freq[l = c + 1]) {
            while (k > freq[++l]);
            l--;

            freq[c] = freq[l];
            freq[l] = k;

            i = son[c];
            prnt[i] = l;

            if (i < T) {
                prnt[i + 1] = l;
            }

            j = son[l];
            son[l] = i;
            prnt[j] = c;

            if (j < T) {
                prnt[j + 1] = c;
            }

            son[c] = j;
            c = l;
        }
    } while ((c = prnt[c]) != 0); // repeat up to root
}

int DecodeChar(void) {
    int c = son[R];

    // travel from root to leaf,
    // choosing the smaller child node (son[]) if the read bit is 0,
    // the bigger (son[]+1} if 1
    while (c < T) {
        c = son[c + GetBit()];
    }

    c -= T;
    update(c);

    return c;
}

int DecodePosition(void) {
    int i;
    int j;
    int c;

    // recover upper 6 bits from table
    i = GetByte();
    c = (int)d_code[i] << 6;
    j = d_len[i];

    // read lower 6 bits verbatim
    j -= 2;

    while (j--) {
        i = (i << 1) + GetBit();
    }

    return c | (i & 0x3f);
}

unsigned unpack_lzh(uint8_t* src, unsigned size, uint8_t* buf) {
    packed_ptr = src;
    packed_end = src + size;

    int i;
    int j;
    int k;
    int c;
    unsigned count = 0;

    StartHuff();

    // while (count < textsize) // textsize - sizeof unpacked data
    while (packed_ptr < packed_end) {
        c = DecodeChar();

        if (c < 256) {
            *(buf++) = c;
            text_buf[r++] = c;
            r &= (N - 1);
            count++;
        } else {
            i = (r - DecodePosition() - 1) & (N - 1);
            j = c - 255 + THRESHOLD;

            for (k = 0; k < j; k++) {
                c = text_buf[(i + k) & (N - 1)];
                *(buf++) = c;
                text_buf[r++] = c;
                r &= (N - 1);
                count++;
            }
        }
    }

    return count;
}
