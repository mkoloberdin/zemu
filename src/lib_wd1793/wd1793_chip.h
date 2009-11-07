/*
 * Unreal WD1793 Emulation library
 * emulation core written by SMT (http://sourceforge.net/projects/unrealspeccy)
 * ported to GNU C++ and modularized by boo_boo
 *
 * mail me: boo_boo(^at^)inbox.ru
 *
 */

#ifndef _WD1793_CHIP_INCLUDED_
#define _WD1793_CHIP_INCLUDED_

enum DIMAGE_TYPE
{
	imgTRD,
	imgFDI,
	imgTD0,
	imgUDI,
	imgUnknown
};

enum DRIVE_STATE
{
	DS_INACTIVE = 0,
	DS_READ,
	DS_WRITE,
	DS_IDLE,
	DS_SEEK,
};

/*
wd1793_in: read from wd1793 port
port -- port to read from
time -- time in tacts of Z80 since start
err -- pointer to error flag (0=no errors)
*/
uint8_t wd1793_in(uint8_t port, uint64_t time, int *err);

/*
wd1793_out: write to wd1793 port
port -- port to write to
val -- value to write
time -- time in tacts of Z80 since start
err -- pointer to error flag (0=no errors)
*/
void wd1793_out(uint8_t port, uint8_t val, uint64_t time, int *err);

/*
wd1793_load_dimage: load disk image <filename> to <drive=0,1,2 or 3>
*/
int wd1793_load_dimage(const char *filename, int drive);

/*
wd1793_eject_dimage: eject disk image from <drive>
*/
void wd1793_eject_dimage(int drive);

/*
wd1793_get_status_reg: get wd1793 status register
*/
uint8_t wd1793_get_status_reg();

/*
wd1793_set_appendboot: automatically append boot from file <boot_name> to TRDOS-disks, if there's no boot.B
*/
void wd1793_set_appendboot(const char *boot_name);

/*
wd1793_set_trd_interleave: set interleave for trd disks
*/
void wd1793_set_trd_interleave(int iv);

/*
wd1793_set_nodelay: set emulation of delays on or off
*/
void wd1793_set_nodelay(bool nodelay);

/*
wd1793_is_disk_changed: returns 1 if disk in <drive> was modifyed
*/
bool wd1793_is_disk_changed(int drive);

/*
wd1793_save_dimage: save disk in <drive> to <filename>, in format <type>
*/
int wd1793_save_dimage(char *filename, int drive, enum DIMAGE_TYPE type);

/*
wd1793_is_disk_loaded: returns 1 if there's disk in <drive>
*/
bool wd1793_is_disk_loaded(int drive);

/*
wd1793_set_disk_wprotected: set write protection for disk in <drive>, 1=protected, 0=not
*/
void wd1793_set_disk_wprotected(int drive, bool wp);

/*
wd1793_is_disk_wprotected: returns 1 if disk in <drive> is write-protected
*/
bool wd1793_is_disk_wprotected(int drive);

/*
wd1793_get_current_drive: get current drive number (0 - 3)
*/
int wd1793_get_current_drive();

/*
wd1793_drive_status: get drive status
*/
enum DRIVE_STATE wd1793_get_drive_state(int drive, uint64_t time);

/*
wd1793_get_drive_head: get current track for drive
*/
int wd1793_get_drive_head(int drive);

#endif
