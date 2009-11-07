#ifndef _WD1793_H_INCLUDED_
#define _WD1793_H_INCLUDED_

#include <string.h>
#include "wd1793_defs.h"
#include "wd1793_fdd.h"
#include "wd1793_chip.h"

class C_Wd1793
{
	private:

	int64_t next;
	int64_t time;
	C_Fdd *seldrive;
	unsigned tshift;

	uint8_t state, state2, cmd;
	uint8_t data, track, sector;
	uint8_t rqs, status;

	unsigned drive, side;                // update this with changing 'system'

	int8_t stepdirection;
	uint8_t system;                // beta128 system register

	// read/write sector(s) data
	int64_t end_waiting_am;
	unsigned foundid;                    // index in trkcache.hdr for next encountered ID and bytes before this ID
	unsigned rwptr, rwlen;

	// format track data
	unsigned start_crc;

	int wd93_nodelay;

	enum CMDBITS
	{
		CMD_SEEK_RATE     = 0x03,
		CMD_SEEK_VERIFY   = 0x04,
		CMD_SEEK_HEADLOAD = 0x08,
		CMD_SEEK_TRKUPD   = 0x10,
		CMD_SEEK_DIR      = 0x20,

		CMD_WRITE_DEL     = 0x01,
		CMD_SIDE_CMP_FLAG = 0x02,
		CMD_DELAY         = 0x04,
		CMD_SIDE          = 0x08,
		CMD_SIDE_SHIFT    = 3,
		CMD_MULTIPLE      = 0x10,
	};

	enum BETA_STATUS
	{
		DRQ   = 0x40,
		INTRQ = 0x80,
	};

	enum WD_STATUS
	{
		WDS_BUSY      = 0x01,
		WDS_INDEX     = 0x02,
		WDS_DRQ       = 0x02,
		WDS_TRK00     = 0x04,
		WDS_LOST      = 0x04,
		WDS_CRCERR    = 0x08,
		WDS_NOTFOUND  = 0x10,
		WDS_SEEKERR   = 0x10,
		WDS_RECORDT   = 0x20,
		WDS_HEADL     = 0x20,
		WDS_WRFAULT   = 0x20,
		WDS_WRITEP    = 0x40,
		WDS_NOTRDY    = 0x80
	};

	int process();
	void find_marker();
	char notready();
	void load();
	void getindex();

	C_TrkCache trkcache;
	C_Fdd fdd[4];

	public:

	enum WDSTATE
	{
		S_IDLE = 0,
		S_WAIT,

		S_DELAY_BEFORE_CMD,
		S_CMD_RW,
		S_FOUND_NEXT_ID,
		S_READ,
		S_WRSEC,
		S_WRITE,
		S_WRTRACK,
		S_WR_TRACK_DATA,

		S_TYPE1_CMD,
		S_STEP,
		S_SEEKSTART,
		S_SEEK,
		S_VERIFY,

		S_RESET
	};

	// read from wd port
	// ttime - number of cpu tacts since computer start
	uint8_t in(uint8_t port, int64_t ttime, int *err);

	// write to wd port
	// ttime - number of cpu tacts since computer start
	void out(uint8_t port, uint8_t val, int64_t ttime, int *err);

	// load disk image to drive
	// drive - from 0 to 3
	int load_dimage(const char *filename, int drive);

	// eject disk image from drive
	void eject_dimage(int drive);

	// test whether disk in drive have been changed
	int is_disk_changed(int drive);

	// emulate wd93 delays or not?
	void set_nodelay(int nodelay);

	// get wd1793 status register
	uint8_t get_status_reg();

	void set_appendboot(const char *boot_name);
	void set_trd_interleave(int iv);
	int save_dimage(char *filename, int drive, enum DIMAGE_TYPE type);
	int is_disk_loaded(int drive);
	void set_disk_wprotected(int drive, bool wp);
	bool is_disk_wprotected(int drive);
	int get_current_drive();
	int is_drive_spinning(int drive, int64_t tm);
	unsigned get_state();

	// get current track for a drive
	int get_drive_head(int drive);

	C_Wd1793();
};

#endif
