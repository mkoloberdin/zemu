#include <stdlib.h>
#include <string.h>
#include "wd1793_trkcache.h"
#include "wd1793_fdd.h"

C_TrkCache::C_TrkCache()
{
	clear();
}

void C_TrkCache::set_i(unsigned pos)
{
	trki[pos / 8] |= (1 << (pos & 7));
}

void C_TrkCache::clr_i(unsigned pos)
{
	trki[pos / 8] &= ~(1 << (pos & 7));
}

uint8_t C_TrkCache::test_i(unsigned pos)
{
	return (trki[pos / 8] & (1 << (pos & 7)));
}

void C_TrkCache::write(unsigned pos, uint8_t byte, char index)
{
	trkd[pos] = byte;

	if (index) set_i(pos);
	else clr_i(pos);
}

void C_TrkCache::clear()
{
	drive = 0;
	trkd = 0;
}

void C_TrkCache::seek(C_Fdd *d, unsigned cyl, unsigned side, SEEK_MODE fs)
{
	// if (!( ((int)d - (int)drive) | (sf - fs) | (cyl - this->cyl) | (side - this->side) )) return;
	if ( (d == drive) && (sf == fs) && (cyl == this->cyl) && (side == this->side) ) return;

	drive = d;
	sf = fs;
	s = 0;

	this->cyl = cyl;
	this->side = side;

	if (cyl >= d->cyls || !d->rawdata)
	{
		trkd = 0;
		return;
	}

	trkd = d->trkd[cyl][side];
	trki = d->trki[cyl][side];
	trklen = d->trklen[cyl][side];

	if (!trklen)
	{
		trkd = 0;
		return;
	}

	ts_byte = Z80FQ / (trklen * FDD_RPS);
	if (fs == JUST_SEEK) return; // else find sectors

	for (unsigned i = 0; i < trklen-8; i++)
	{
		if (trkd[i]!=0xA1 || trkd[i+1]!=0xFE || !test_i(i)) continue;

		if (s >= (unsigned)MAX_SEC)
		{
			printf("too many sectors\n");
			return;
		}

		s_SecHdr *h = &hdr[s++];
		h->id = trkd+i+2;
		h->c = h->id[0];
		h->s = h->id[1];
		h->n = h->id[2];
		h->l = h->id[3];
		h->crc = WORD2(trkd[i+6], trkd[i+7]);
		h->c1 = (wd1793_crc(trkd+i+1, 5) == h->crc);
		h->data = NULL;
		h->datlen = 0;

		if (h->l > 5) continue;

		unsigned end = min(trklen-8, i+8+43); // 43-DD, 30-SD

		for (unsigned j = i+8; j < end; j++)
		{
			if (trkd[j]!=0xA1 || !test_i(j) || test_i(j+1)) continue;

			if (trkd[j+1]==0xF8 || trkd[j+1]==0xFB)
			{
				h->datlen = (128 << h->l);
				h->data = trkd+j+2;
				h->c2 = (wd1793_crc(h->data-1, h->datlen+1) == WORD2(h->data[h->datlen], h->data[h->datlen + 1]));
			}

			break;
		}
	}
}

void C_TrkCache::format()
{
	memset(trkd, 0, trklen);
	memset(trki, 0, (trklen / 8) + ((trklen & 7) ? 1 : 0));

	uint8_t *dst = trkd;
	unsigned i;

	for (i = 0; i < 80; i++) *(dst++) = 0x4E; // 1st gap
	for (i = 0; i < 12; i++) *(dst++) = 0;
	for (i = 0; i < 3; i++) write((dst++) - trkd, 0xC2, 1);
	*(dst++) = 0xFC; // index

	for (unsigned is = 0; is < s; is++)
	{
		for (i = 0; i < 50; i++) *(dst++) = 0x4E;
		for (i = 0; i < 12; i++) *(dst++) = 0;
		for (i = 0; i < 3; i++) write((dst++) - trkd, 0xA1, 1);
		*(dst++) = 0xFE; // marker

		s_SecHdr *sechdr = hdr + is;
		*(dst++) = sechdr->c;
		*(dst++) = sechdr->s;
		*(dst++) = sechdr->n;
		*(dst++) = sechdr->l;

		unsigned crc = wd1793_crc(dst-5, 5);
		if (sechdr->c1 == 1) crc = sechdr->crc;
		if (sechdr->c1 == 2) crc ^= 0xFFFF;
		*(dst++) = (crc & 0xFF);
		*(dst++) = (crc >> 8);

		if (sechdr->data)
		{
			for (i = 0; i < 22; i++) *(dst++) = 0x4E;
			for (i = 0; i < 12; i++) *(dst++) = 0;
			for (i = 0; i < 3; i++) write((dst++) - trkd, 0xA1, 1);
			*(dst++) = 0xFB; // sector

			if (sechdr->l > 5)
			{
				printf("strange sector\n");
				return;
			}

			unsigned len = (128 << sechdr->l);

			if (sechdr->data && sechdr->data!=(uint8_t *)1) memcpy(dst, sechdr->data, len);
			else memset(dst, 0, len);

			crc = wd1793_crc(dst-1, len+1);
			if (sechdr->c2 == 1) crc = sechdr->crcd;
			if (sechdr->c2 == 2) crc ^= 0xFFFF;
			dst[len] = (crc & 0xFF);
			dst[len+1] = (crc >> 8);
			dst += len+2;
		}
	}

	if (dst > trklen+trkd)
	{
		printf("track too long\n");
		return;
	}

	while (dst < trkd+trklen) *(dst++) = 0x4E;
}

/*
void C_TrkCache::dump()
{
	printf("\n%d/%d:", cyl, side);
	if (!trkd) { printf("<e>"); return; }
	if (!sf) { printf("<n>"); return; }
	for (unsigned i = 0; i < s; i++) printf("%c%02X-%02X-%02X-%02X,%c%c%c", i?' ':'<', hdr[i].c,hdr[i].s,hdr[i].n,hdr[i].l, hdr[i].c1?'+':'-', hdr[i].c2?'+':'-', hdr[i].data?'d':'h');
	printf(">");
}
*/

int C_TrkCache::write_sector(unsigned sec, uint8_t *data)
{
	s_SecHdr *h = get_sector(sec);
	if (!h || !h->data) return 0;

	unsigned sz = h->datlen;
	memcpy(h->data, data, sz);

	uint16_t crc = wd1793_crc(h->data-1, sz+1);
	h->data[sz] = (crc & 0xFF);
	h->data[sz+1] = (crc >> 8);

	return sz;
}

s_SecHdr * C_TrkCache::get_sector(unsigned sec)
{
	unsigned i;

	for (i = 0; i < s; i++) {
		if (hdr[i].n == sec) break;
	}

	if (i == s) return NULL;
	if (hdr[i].l!=1 || hdr[i].c!=cyl) return NULL;

	return &hdr[i];
}
