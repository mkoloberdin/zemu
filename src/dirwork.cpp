#include <string.h>
#include "exceptions.h"
#include "dirwork.h"

using namespace std;

#ifdef _WIN32

#include <shlwapi.h>

char *realpath(const char *path, char *resolved_path)
{
	char *c;
	char *r = _fullpath(resolved_path, path, _MAX_PATH);

	if (!r) return NULL;
	if (_access(r, 0) != 0) return NULL; // path does not exist

	// TODO: while * strchr == slow. optimize it
	while ((c = strchr(r, '\\')) != NULL) {
		*c = '/';
	}

	return r;
}

C_DirWork::C_DirWork()
{
	hFile = -1;
	enumMode = 0;
}

C_DirWork::~C_DirWork()
{
	if (hFile != -1) _findclose(hFile);
}

bool C_DirWork::EnumFiles(const char *path)
{
	if (enumMode) throw C_E(E_DirWork);

	if (!strlen(path) || !strcmp(path, "/"))
	{
		char root_path[] = "X:";

		availDrives.clear();

		for (char drv_lett='A'; drv_lett<='Z'; drv_lett++)
		{
			root_path[0] = drv_lett;
			unsigned drv_type = GetDriveType(root_path);

			if (drv_type == DRIVE_FIXED ||
				drv_type == DRIVE_REMOVABLE ||
				drv_type == DRIVE_REMOTE ||
				drv_type == DRIVE_CDROM ||
				drv_type == DRIVE_RAMDISK) {
				availDrives.push_back(drv_lett);
			}
		}

		if (!availDrives.empty())
		{
			name[0] = availDrives.front();
			name[1] = 0;

			attr = DW_FOLDER;
			size = 0;

			availDrives.pop_front();
			enumMode = DW_ENUM_ROOT;
			return true;
		}
	}
	else
	{
		strcpy(pathNmask, path);

		pathNmask[0] = pathNmask[1];
		pathNmask[1] = ':';

		unsigned sz = strlen(pathNmask);
		if (pathNmask[sz-1] != '/') strcat(pathNmask, "/");

		strcat(pathNmask, "*.*");

		if (strlen(pathNmask) == (5+1))	// "X:/*.*"
		{
			enumMode = DW_ENUM_FILES;

			strcpy(name, "..");
			attr = DW_FOLDER;
			size = 0;

			if (hFile != -1) _findclose(hFile);
			hFile = -1;

			return true;
		}
		else
		{
			hFile = _findfirst(pathNmask, &cf);

			if (hFile != -1)
			{
				strcpy(name, cf.name);
				attr = (cf.attrib & _A_SUBDIR) ? DW_FOLDER : DW_NORMAL;
				size = cf.size;

				enumMode = DW_ENUM_FILES;
				return true;
			}
		}
	}

	return false;
}

bool C_DirWork::EnumNext(void)
{
	if (!enumMode) throw C_E(E_DirWork);

	if (enumMode == DW_ENUM_ROOT)
	{
		if (!availDrives.empty())
		{
			name[0] = availDrives.front();
			name[1] = 0;

			attr = DW_FOLDER;
			size = 0;

			availDrives.pop_front();
			return true;
		}
	}
	else
	{
		if (hFile == -1)
		{
			hFile = _findfirst(pathNmask, &cf);

			if (hFile != -1)
			{
				strcpy(name, cf.name);
				attr = (cf.attrib & _A_SUBDIR) ? DW_FOLDER : DW_NORMAL;
				size = cf.size;
				return true;
			}
		}
		else
		{
			if (_findnext(hFile, &cf) == 0)
			{
				strcpy(name, cf.name);
				attr = (cf.attrib & _A_SUBDIR) ? DW_FOLDER : DW_NORMAL;
				size = cf.size;
				return true;
			}
		}
	}

	return false;
}

void C_DirWork::EnumClose(void)
{
	if (!enumMode) throw C_E(E_DirWork);

	if (enumMode == DW_ENUM_FILES)
	{
		_findclose(hFile);
		hFile = -1;
	}
	else
	{
		availDrives.clear();
	}

	enumMode = 0;
}

#endif	// _WIN32

#ifdef __unix__

#include <sys/stat.h>

// TODO: at this time all functions ignore file mask. do it ^_~

C_DirWork::C_DirWork()
{
	hDir = NULL;
}

C_DirWork::~C_DirWork()
{
	if (hDir != NULL) closedir(hDir);
}

bool C_DirWork::EnumFiles(const char *path)
{
	struct dirent *di;
	struct stat st;
	char tpath[MAX_PATH];

	if (hDir != NULL) throw C_E(E_DirWork);

	strcpy(bpath, path);
	hDir = opendir(bpath);
	if (hDir == NULL) return false;

	di = readdir(hDir);

	if (di == NULL)
	{
		closedir(hDir);
		hDir = NULL;
		return false;
	}

	strcpy(name, di->d_name);
	strcpy(tpath, bpath);
	strcat(tpath, name);
	stat(tpath, &st);

	attr = (S_ISDIR(st.st_mode)) ? DW_FOLDER : DW_NORMAL;
	size = st.st_size;
	return true;
}

bool C_DirWork::EnumNext(void)
{
	struct dirent *di;
	struct stat st;
	char path[MAX_PATH];

	if (hDir == NULL) throw C_E(E_DirWork);

	di = readdir(hDir);
	if (di == NULL) return false;

	strcpy(name, di->d_name);
	strcpy(path, bpath);
	strcat(path, name);
	stat(path, &st);

	attr = (S_ISDIR(st.st_mode)) ? DW_FOLDER : DW_NORMAL;
	size = st.st_size;
	return true;
}

void C_DirWork::EnumClose(void)
{
	if (hDir == NULL) throw C_E(E_DirWork);

	closedir(hDir);
	hDir = NULL;
}

#endif	// __unix__

char* C_DirWork::Normalize(const char *path, bool isFile)
{
	static char result[MAX_PATH];

	if (realpath(path, result) == NULL)
	{
		strcpy(result, "/");
		return result;
	}

	size_t rlen = strlen(result);

	if (!isFile && rlen > 0 && result[rlen-1] != '/') {
		strcat(result, "/");
	}

	if (rlen >= 3 && result[2] == '/' && result[1] == ':') {
		result[1] = result[0];
		result[0] = result[2] = '/';
	}

	return result;
}

char* C_DirWork::LevelUp(const char *path)
{
	int i, l;
	static char result[MAX_PATH];

	l = strlen(path)-1;

	while (l >= 0)
	{
		if (path[l] == '/') break;
		l--;
	}

	if (l < 0)
	{
		strcpy(result, "../");
		return result;
	}

	if (l == 0)
	{
		strcpy(result, "/");
		return result;
	}

	l--;

	if (l > 0)
	{
		if (path[l]=='.' && path[l-1]=='.')
		{
			if (l == 1)
			{
				strcpy(result, "../../");
				return result;
			}

			if (path[l-2] == '/')
			{
				for (i = 0; i <= l; i++) result[i] = path[i];
				result[i] = 0;

				strcat(result, "/../");
				return result;
			}
		}
	}

	while (l >= 0)
	{
		if (path[l] == '/') break;
		l--;
	}

	if (l < 0)
	{
		strcpy(result, "");
		return result;
	}

	for (i = 0; i <= l; i++) result[i] = path[i];
	result[i] = 0;

	return result;
}

char* C_DirWork::ExtractPath(const char *path)
{
	int i, l;
	static char result[MAX_PATH];

	l = strlen(path)-1;

	while (l >= 0)
	{
		#ifdef _WIN32
			// TODO: PathRemoveFileSpec from shlwapi.h
			if (path[l] == '\\' || path[l] == '/') break;
		#else
			if (path[l] == '/') break;
		#endif

		l--;
	}

	if (l < 0)
	{
		strcpy(result, "");
		return result;
	}

	for (i = 0; i <= l; i++) result[i] = path[i];
	result[i] = 0;

	return result;
}

char* C_DirWork::ExtractExt(const char *path)
{
	int i, l, le;
	static char result[MAX_PATH];

	le = strlen(path);
	l = le - 1;

	while (l >= 0)
	{
		if (path[l] == '.') break;
		l--;
	}

	if (l < 0)
	{
		strcpy(result, "");
		return result;
	}

	l++;
	for (i = 0; i < le-l; i++) result[i] = path[i+l];
	result[i] = 0;

	return result;
}

char* C_DirWork::ExtractFileName(const char *path)
{
	int i, l;
	static char result[MAX_PATH];

	l = strlen(path) - 1;

	while (l >= 0)
	{
		#ifdef _WIN32
			if (path[l] == '\\' || path[l] == '/') break;
		#else
			if (path[l] == '/') break;
		#endif

		l--;
	}

	if (l < 0)
	{
		strcpy(result, path);
		return result;
	}

	l++;
	for (i = l; i < (int)strlen(path); i++) result[i-l] = path[i];
	result[i-l] = 0;

	return result;
}

char* C_DirWork::LastDirName(const char *path)
{
	static char result[MAX_PATH];

#ifdef __unix__
	if (realpath(C_DirWork::ExtractPath(path), result) == NULL)
	{
		strcpy(result, "");
		return result;
	}
#else
	// TODO: use realpath for win32 version
	strcpy(result, path);

	unsigned sz = strlen(result);
	if (sz && result[sz-1]=='/') result[sz-1] = 0;
#endif

	return C_DirWork::ExtractFileName(result);
}

string C_DirWork::Append(const string &p1, const string &p2) {
	if (p1.empty()) {
		return p2;
	} else if (p2.empty()) {
		return p1;
	}

#ifdef _WIN32
	TCHAR buffer[MAX_PATH];
	strcpy(buffer, p1.c_str());
	PathAppend(buffer, p2.c_str());
	return string(buffer);
#else
	size_t offset = 0;

	while (!p2.empty() > 0 && offset < p2.size() && p2[offset] == '/') {
		offset++;
	}

	if (p1[p1.size() - 1] == '/') {
		return p1 + p2.substr(offset);
	} else {
		return p1 + '/' + p2.substr(offset);
	}
#endif
}
