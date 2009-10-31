#include <stdlib.h>
#include <string.h>
#include "wd1793_fdd.h"

int C_Fdd::read_udi()
{
	free();

	unsigned c, s;
	unsigned mem = 0;
	unsigned char *ptr = snbuf + 0x10;

	for (c = 0; c <= snbuf[9]; c++)
	{
		for (s = 0; s <= snbuf[10]; s++)
		{
			if (*ptr) return 0;

			unsigned sz = WORD2(ptr[1], ptr[2]);
			sz += (sz / 8) + ((sz & 7) ? 1 : 0);
			mem += sz;
			ptr += 3 + sz;

			if (ptr > snbuf+snapsize) return 0;
		}
	}

	cyls = snbuf[9] + 1;
	sides = snbuf[10] + 1;
	rawsize = align_by(mem, 4096);
	rawdata = (unsigned char *)malloc(rawsize);
	ptr = snbuf + 0x10;
	unsigned char *dst = rawdata;

	for (c = 0; c < cyls; c++)
	{
		for (s = 0; s < sides; s++)
		{
			unsigned sz = WORD2(ptr[1], ptr[2]);
			trklen[c][s] = sz;
			trkd[c][s] = dst;
			trki[c][s] = dst + sz;
			sz += (sz / 8) + ((sz & 7) ? 1 : 0);

			memcpy(dst, ptr+3, sz);
			ptr += 3 + sz;
			dst += sz;
		}
	}

	if (snbuf[11] & 1) strcpy(dsc, (char*)ptr);
	return 1;
}

int C_Fdd::write_udi(FILE *ff)
{
	memset(snbuf, 0, 0x10);

	snbuf[0] = 'U';
	snbuf[1] = 'D';
	snbuf[2] = 'I';
	snbuf[3] = '!';
	snbuf[8] = snbuf[11] = 0;
	snbuf[9] = cyls-1;
	snbuf[10] = sides-1;
	*((unsigned *)(snbuf + 12)) = 0;

	unsigned char *dst = snbuf + 0x10;

	for (unsigned c = 0; c < cyls; c++)
	{
		for (unsigned s = 0; s < sides; s++)
		{
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

	if (*dsc)
	{
		strcpy((char*)dst, dsc);
		dst += strlen(dsc) + 1;
		snbuf[11] = 1;
	}

	unsigned sz = dst - snbuf;
	snbuf[4] = (sz & 0xFF);
	snbuf[5] = ((sz >> 8) & 0xFF);
	snbuf[6] = ((sz >> 16) & 0xFF);
	snbuf[7] = (sz >> 24);

	int crc = -1;
	wd1793_crc32(crc, snbuf, dst-snbuf);

	*(dst++) = (((unsigned)crc) & 0xFF);
	*(dst++) = ((((unsigned)crc) >> 8) & 0xFF);
	*(dst++) = ((((unsigned)crc) >> 16) & 0xFF);
	*(dst++) = (((unsigned)crc) >> 24);

	if (fwrite(snbuf, 1, dst-snbuf, ff) != (unsigned)(dst-snbuf)) {
		return 0;
	}

	return 1;
}
