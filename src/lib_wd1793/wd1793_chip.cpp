#include "wd1793.h"
#include "wd1793_chip.h"

static C_Wd1793 wd;

uint8_t wd1793_in(uint8_t port, uint64_t time, int *err)
{
	return wd.in(port, time, err);
}

void wd1793_out(uint8_t port, uint8_t val, uint64_t time, int *err)
{
	wd.out(port, val, time, err);
}

int wd1793_load_dimage(const char *filename, int drive)
{
	return wd.load_dimage(filename, drive);
}

void wd1793_eject_dimage(int drive)
{
	wd.eject_dimage(drive);
}

uint8_t wd1793_get_status_reg()
{
	return wd.get_status_reg();
}

void wd1793_set_appendboot(const char *boot_name)
{
	wd.set_appendboot(boot_name);
}

void wd1793_set_trd_interleave(int iv)
{
	wd.set_trd_interleave(iv);
}

void wd1793_set_nodelay(bool nodelay)
{
	wd.set_nodelay(nodelay);
}

bool wd1793_is_disk_changed(int drive)
{
	return wd.is_disk_changed(drive);
}

int wd1793_save_dimage(char *filename, int drive, enum DIMAGE_TYPE type)
{
	return wd.save_dimage(filename, drive, type);
}

bool wd1793_is_disk_loaded(int drive)
{
	return wd.is_disk_loaded(drive);
}

void wd1793_set_disk_wprotected(int drive, bool wp)
{
	wd.set_disk_wprotected(drive, wp);
}

bool wd1793_is_disk_wprotected(int drive)
{
	return wd.is_disk_wprotected(drive);
}

int wd1793_get_current_drive()
{
	return wd.get_current_drive();
}

enum DRIVE_STATE wd1793_get_drive_state(int drive, uint64_t time)
{
	enum DRIVE_STATE st;

	if (!wd.is_drive_spinning(drive, time))
	{
		st = DS_INACTIVE;		// [rst]
	}
	else
	{
		if (drive != wd.get_current_drive())
		{
			st = DS_IDLE;
		}
		else
		{
			switch (wd.get_state())
			{
				case C_Wd1793::S_WRSEC:
				case C_Wd1793::S_WRITE:
				case C_Wd1793::S_WRTRACK:
				case C_Wd1793::S_WR_TRACK_DATA:
					st = DS_WRITE;
					break;

				case C_Wd1793::S_READ:
					st = DS_READ;
					break;

				case C_Wd1793::S_SEEKSTART:
				case C_Wd1793::S_SEEK:
					st = DS_SEEK;
					break;

				default:
					st = DS_IDLE;
			}
		}
	}

	return st;
}

int wd1793_get_drive_head(int drive)
{
	return wd.get_drive_head(drive);
}
