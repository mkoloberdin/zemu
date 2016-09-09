#ifndef _WD1793_DEFS_H_INCLUDED_
#define _WD1793_DEFS_H_INCLUDED_

#include <stdint.h>

#define align_by(a,b) (((unsigned)(a)+((b)-1)) & ~((b)-1))
#define WORD4(a,b,c,d) (((unsigned)(a))+((unsigned)(b))*0x100L+((unsigned)(c))*0x10000L+((unsigned)(d))*0x1000000L)
#define WORD2(a,b) (((unsigned)(a))+((unsigned)(b))*0x100)

struct s_SecHdr
{
	uint8_t c, s, n, l;
	uint16_t crc;
	uint8_t c1, c2; // flags: correct CRCs in address and data
	uint8_t *data, *id;
	unsigned datlen;
	unsigned crcd;        // used to load specific CRC from FDI-file
};

enum SEEK_MODE
{
	JUST_SEEK = 0,
	LOAD_SECTORS = 1
};

#endif
