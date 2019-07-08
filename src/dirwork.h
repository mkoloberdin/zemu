#ifndef _DIRWORK_H_INCLUDED_
#define _DIRWORK_H_INCLUDED_

#include "defines.h"
#include <string>

#ifdef _WIN32
	#include <io.h>
	#include <list>

	#define DW_ENUM_ROOT 1
	#define DW_ENUM_FILES 2
#endif

#ifndef _WIN32
	#include <sys/types.h>
	#include <dirent.h>
#endif

#define DW_NORMAL 0
#define DW_FOLDER 1

class C_DirWork
{
	public:

	C_DirWork();
	~C_DirWork();

	bool EnumFiles(const char *path);
	bool EnumNext(void);
	void EnumClose(void);

	static char* Normalize(const char *path, bool isFile=false);
	static char* LevelUp(const char *path);
	static char* ExtractPath(const char *path);
	static char* ExtractExt(const char *path);
	static char* ExtractFileName(const char *path);
	static char* LastDirName(const char *path);
	static std::string Append(const std::string &p1, const std::string &p2);

	char name[MAX_PATH];
	int attr;
	long size;

	private:

#ifdef _WIN32
	int enumMode;
	char pathNmask[MAX_PATH];

	struct _finddata_t cf;
	long hFile;

	std::list<char> availDrives;
#endif

#ifndef _WIN32
	DIR *hDir;
	char bpath[MAX_PATH];
#endif

	C_DirWork& operator=(const C_DirWork &dummy) { return *this; }
	C_DirWork(const C_DirWork &dummy) {}
};

#endif
