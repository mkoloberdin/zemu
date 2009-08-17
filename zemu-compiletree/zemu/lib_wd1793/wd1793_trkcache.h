#ifndef _WD1793_TRKCACHE_H_INCLUDED_
#define _WD1793_TRKCACHE_H_INCLUDED_

#include "wd1793_const.h"
#include "wd1793_defs.h"

class C_Fdd;

class C_TrkCache
{
	public:

	// cached track position
	C_Fdd *drive;
	unsigned cyl, side;

	// generic track data
	unsigned trklen;
	unsigned char *trkd, *trki;       // pointer to data inside UDI
	unsigned ts_byte;                 // cpu.t per byte
	SEEK_MODE sf;                     // flag: is sectors filled
	unsigned s;                       // no. of sectors

	// sectors on track
	s_SecHdr hdr[MAX_SEC];

	void set_i(unsigned pos);
	void clr_i(unsigned pos);
	unsigned char test_i(unsigned pos);
	void write(unsigned pos, unsigned char byte, char index);

	void seek(C_Fdd *d, unsigned cyl, unsigned side, SEEK_MODE fs);
	void format(); // before use, call seek(d,c,s,JUST_SEEK), set s and hdr[]
	int write_sector(unsigned sec, unsigned char *data); // call seek(d,c,s,LOAD_SECTORS)
	s_SecHdr * get_sector(unsigned sec); // before use, call fill(d,c,s,LOAD_SECTORS)

	// void dump();
	void clear();

	C_TrkCache();
};

#endif
