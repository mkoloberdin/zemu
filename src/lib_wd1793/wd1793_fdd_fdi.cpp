#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "wd1793_fdd.h"

int C_Fdd::read_fdi()
{
	newdisk(snbuf[4], snbuf[6]);
	strncpy(dsc, (char *)snbuf + WORD2(snbuf[8], snbuf[9]), sizeof(dsc));

	int res = 1;
	uint8_t *trk = snbuf + 0x0E + WORD2(snbuf[0x0C], snbuf[0x0D]);
	uint8_t *dat = snbuf + WORD2(snbuf[0x0A], snbuf[0x0B]);

	for (unsigned c = 0; c < snbuf[4]; c++)
	{
		for (unsigned s = 0; s < snbuf[6]; s++)
		{
			t.seek(this, c,s, JUST_SEEK);

			uint8_t *t0 = dat + WORD4(trk[0], trk[1], trk[2], trk[3]);
			unsigned ns = trk[6];
			trk += 7;

			for (unsigned sec = 0; sec < ns; sec++)
			{
				t.hdr[sec].c = trk[0];
				t.hdr[sec].s = trk[1];
				t.hdr[sec].n = trk[2];
				t.hdr[sec].l = trk[3];
				t.hdr[sec].c1 = 0;

				if (trk[4] & 0x40)
				{
					t.hdr[sec].data = NULL;
				}
				else
				{
					t.hdr[sec].data = t0 + WORD2(trk[5], trk[6]);
					if (t.hdr[sec].data+128 > snbuf+snapsize) return 0;
					t.hdr[sec].c2 = ((trk[4] & (1 << trk[3])) ? 0 : 2);
				}

				if (t.hdr[sec].l > 5)
				{
					t.hdr[sec].data = NULL;
					if (!(trk[4] & 0x40)) res = 0;
				}

				trk += 7;
			}

			t.s = ns;
			t.format();
		}
	}

	return res;
}

int C_Fdd::write_fdi(FILE *ff)
{
	unsigned c, s, total_s = 0;

	for (c = 0; c < cyls; c++)
	{
		for (s = 0; s < sides; s++)
		{
			t.seek(this, c, s, LOAD_SECTORS);
			total_s += t.s;
		}
	}

	unsigned tlen = strlen(dsc) + 1;
	unsigned hsize = 14 + (total_s + cyls * sides) * 7;

	snbuf[0] = 'F';
	snbuf[1] = 'D';
	snbuf[2] = 'I';
	snbuf[3] = 0;

	snbuf[4] = (cyls & 0xFF);
	snbuf[5] = (cyls >> 8);

	snbuf[6] = (sides & 0xFF);
	snbuf[7] = (sides >> 8);

	snbuf[8] = (hsize & 0xFF);
	snbuf[9] = (hsize >> 8);

	snbuf[0x0A] = ((hsize + tlen) & 0xFF);
	snbuf[0x0B] = ((hsize + tlen) >> 8);

	snbuf[0x0C] = 0;
	snbuf[0x0D] = 0;

	if (fwrite(snbuf, 1, 14, ff) != 14) DEBUG_MESSAGE("fwrite failed");
	unsigned trkoffs = 0;

	for (c = 0; c < cyls; c++)
	{
		for (s = 0; s < sides; s++)
		{
			t.seek(this, c, s, LOAD_SECTORS);
			unsigned secoffs = 0;

			snbuf[0] = (trkoffs & 0xFF);
			snbuf[1] = ((trkoffs >> 8) & 0xFF);
			snbuf[2] = ((trkoffs >> 16) & 0xFF);
			snbuf[3] = (trkoffs >> 24);

			*((unsigned *)(snbuf + 4)) = 0;

			snbuf[6] = t.s;
			if (fwrite(snbuf, 1, 7, ff) != 7) DEBUG_MESSAGE("fwrite failed");

			for (unsigned se = 0; se < t.s; se++)
			{
				snbuf[0] = t.hdr[se].c;
				snbuf[1] = t.hdr[se].s;
				snbuf[2] = t.hdr[se].n;
				snbuf[3] = t.hdr[se].l;

				snbuf[4] = (t.hdr[se].c2 ? (1 << t.hdr[se].l) : 0);
				if (t.hdr[se].data && t.hdr[se].data[-1]==0xF8) snbuf[4] |= 0x80;
				if (!t.hdr[se].data) snbuf[4] |= 0x40;

				snbuf[5] = (secoffs & 0xFF);
				snbuf[6] = ((secoffs >> 8) & 0xFF);
				snbuf[7] = ((secoffs >> 16) & 0xFF);
				snbuf[8] = (secoffs >> 24);

				if (fwrite(snbuf, 1, 7, ff) != 7) DEBUG_MESSAGE("fwrite failed");
				secoffs += t.hdr[se].datlen;
			}

			trkoffs += secoffs;
		}
	}

	fseek(ff, hsize, SEEK_SET);
	if (fwrite(dsc, 1, tlen, ff) != tlen) DEBUG_MESSAGE("fwrite failed");

	for (c = 0; c < cyls; c++)
	{
		for (s = 0; s < sides; s++)
		{
			t.seek(this, c, s, LOAD_SECTORS);

			for (unsigned se = 0; se < t.s; se++) {
				if (t.hdr[se].data) {
					if (fwrite(t.hdr[se].data, 1, t.hdr[se].datlen, ff) != t.hdr[se].datlen) return 0;
				}
			}
		}
	}

	return 1;
}
