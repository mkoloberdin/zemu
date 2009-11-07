#ifndef _WD1793_FDD_H_INCLUDED_
#define _WD1793_FDD_H_INCLUDED_

#include <stdio.h>
#include "wd1793_defs.h"
#include "wd1793_trkcache.h"
#include "wd1793_crc_utils.h"
#include "wd1793_chip.h"

enum DISKSNAP
{
	snHOB,
	snSCL,
	snTRD,
	snFDI,
	snTD0,
	snUDI,
	snUNKNOWN,
	snTOOLARGE,
	snNOFILE
};

extern uint8_t snbuf[SNBUF_LEN]; // large temporary buffer

class C_Fdd
{
	public:

	// drive data

	int64_t motor;       // 0 - not spinning, >0 - time when it'll stop
	uint8_t track; // head position

	// disk data

	uint8_t *rawdata;
	size_t rawsize;
	unsigned cyls, sides;
	unsigned trklen[MAX_CYLS][2];
	uint8_t *trkd[MAX_CYLS][2];
	uint8_t *trki[MAX_CYLS][2];
	uint8_t optype; // bits: 0-not modified, 1-write sector, 2-format track
	uint8_t snaptype;
	size_t snapsize;
	bool is_wp;
	int interleave;

	C_TrkCache t; // used in read/write image
	char name[0x200];
	char dsc[0x200];
	C_TrkCache *trkcache;
	char appendboot[1024];

	void format_trd();
	void emptydisk();
	void newdisk(unsigned cyls, unsigned sides);
	int addfile(uint8_t *hdr, uint8_t *data);
	void addboot();
	uint8_t what_is(const char *filename);

	int read(uint8_t snType);

	int read_scl();
	int read_hob();
	int read_trd();
	int write_trd(FILE *ff);
	int read_fdi();
	int write_fdi(FILE *ff);
	int read_td0();
	int write_td0(FILE *ff);
	int read_udi();
	int write_udi(FILE *ff);

	void set_trkcache(C_TrkCache *tc);
	void free();

	//

	bool is_wprotected();
	void set_wprotected(bool wp);
	int load_dimage(const char *filename); /*load disk image*/
	int save_dimage(char *filename, enum DIMAGE_TYPE type);
	char is_changed(); /*whether disk in drive has been changed*/
	void set_appendboot(const char *boot_name); /*name for file with boot, or NULL*/
	char *get_appendboot(void);
	void set_trd_interleave(int iv);
	int is_disk_loaded();
	void eject();

	C_Fdd();
	~C_Fdd();
};

#endif
