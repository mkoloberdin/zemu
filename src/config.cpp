#include "platform.h"
#include "config.h"
#include "path.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <io.h> // for _access()
#else
#include <unistd.h> // for access()
#endif // _WIN32

#ifdef FOR_INSTALL
#ifdef _WIN32
#ifndef _WIN32_IE
#define _WIN32_IE 0x400
#endif
#include <shlobj.h>
#include <io.h>

#else
#include <sys/stat.h>
#include <sys/types.h>

#endif // _WIN32
#endif // FOR_INSTALL

#ifdef _WIN32
#include <shlwapi.h>
#endif

#include "file.h"

CConfig::CConfig(const char *app_name) {
	changed = false;
	this->app_name = app_name;

	#ifdef _WIN32
		TCHAR buffer[MAX_PATH];
		GetModuleFileName(0, buffer, MAX_PATH);
		PathRemoveFileSpec(buffer);
		share_path = string(buffer);
	#else
		#ifdef SHARE_PATH
			share_path = SHARE_PATH; // e.g. /usr/share/app_name or /opt/app_name/share
		#else
			share_path = '.'; // used only for debugging, FIXME: perhaps use argv[0] ?
		#endif // SHARE_PATH
	#endif // _WIN32

	#ifdef FOR_INSTALL
		#ifdef _WIN32
			if (SHGetSpecialFolderPath(0, buffer, CSIDL_APPDATA, true)) {
				user_path = path_append(string(buffer), app_name);
			}
		#else
			if (getenv("XDG_CONFIG_HOME")) {
				user_path = path_append(getenv("XDG_CONFIG_HOME"), this->app_name);
			} else if (C_File::FileExists(path_append(getenv("HOME"), ".config").c_str())) {
				user_path = path_append(getenv("HOME"), ".config/zemu");
			} else {
				user_path = path_append(getenv("HOME"), string(".") + this->app_name);
			}
		#endif // _WIN32

		if(!user_path.empty()) {
			#ifdef _WIN32
				mkdir(user_path.c_str());
			#else
				mkdir(user_path.c_str(), 0755);
			#endif
		}
	#else
		user_path = share_path;
	#endif // FOR_INSTALL

	string ini_user = path_append(user_path, this->app_name + ".ini");

	if(ini.LoadFile(ini_user.c_str())) {
		string ini_global = path_append(share_path, this->app_name + ".ini");
		ini.LoadFile(ini_global.c_str());
	}
}

CConfig::~CConfig() {
	if (changed) {
		string ini_user = path_append(user_path, app_name + ".ini");
		ini.SaveFile(ini_user.c_str());
	}
}

int CConfig::GetInt(const char *section, const char *key, int default_value) {
	const char *cval = ini.GetValue(section, key, NULL);
	if (cval) {
		return atoi(cval);
	} else {
		SetInt(section, key, default_value);
		return default_value;
	}
}

void CConfig::SetInt(const char *section, const char *key, int value) {
	char buffer[0x100];
	sprintf(buffer, "%d", value);
	ini.SetValue(section, key, buffer);
	changed = true;
}

string CConfig::GetString(const char *section, const char *key, const string &default_value) {
	const char *cval = ini.GetValue(section, key, NULL);
	if (cval) {
		return string(cval);
	} else {
		SetString(section, key, default_value);
		return default_value;
	}
}

void CConfig::SetString(const char *section, const char *key, const string &value) {
	ini.SetValue(section, key, value.c_str());
	changed = true;
}

bool CConfig::GetBool(const char *section, const char *key, bool default_value) {
	const char *cval = ini.GetValue(section, key, NULL);
	if (cval) {
		if(!strcasecmp(cval, "true") || !strcasecmp(cval, "yes")) {
			return true;
		} else {
			return atoi(cval) ? true : false;
		}
	} else {
		SetBool(section, key, default_value);
		return default_value;
	}
}

void CConfig::SetBool(const char *section, const char *key, bool value) {
	ini.SetValue(section, key, value ? "yes" : "no");
	changed = true;
}

// try user's home directory first, fall back to shared location
size_t CConfig::LoadDataFile(const char *prefix, const char *filename,
		uint8_t *buffer, size_t size, size_t offset) {
	string fn = path_append(user_path, path_append(prefix, filename));
	FILE *f = fopen(fn.c_str(), "rb");
	if (f == NULL) {
		fn = path_append(share_path, path_append(prefix, filename));
		f = fopen(fn.c_str(), "rb");
		if (f == NULL) return 0;
	}
	if (offset) {
		fseek(f, offset, SEEK_SET);
	}
	size_t r = fread(buffer, 1, size, f);
	fclose(f);
	return r;
}

// save to user's home directory only
bool CConfig::SaveDataFile(const char *prefix, const char *filename,
		const uint8_t *buffer, size_t size) {
	string fn = path_append(user_path, path_append(prefix, filename));
	FILE *f = fopen(fn.c_str(), "wb");
	if (f == NULL) return false;
	size_t r = fwrite(buffer, 1, size, f);
	fclose(f);
	if (r == size) return true;
	else return false;
}

string CConfig::FindDataFile(const char *prefix, const char *filename) {
	string fn = path_append(user_path, path_append(prefix, filename));
#ifdef _WIN32
	if (_access(fn.c_str(), 04) != -1) {
#else
	if (access(fn.c_str(), F_OK | R_OK) == 0) {
#endif // _WIN32
		return fn;
	} else {
		fn = path_append(share_path, path_append(prefix, filename));
#ifdef _WIN32
		if (_access(fn.c_str(), 04) != -1) {
#else
		if (access(fn.c_str(), F_OK | R_OK) == 0) {
#endif // _WIN32
			return fn;
		} else {
			return string("");
		}
	}
}
