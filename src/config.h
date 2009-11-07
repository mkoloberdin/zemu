#ifndef ZEMU_CONFIG_H
#define ZEMU_CONFIG_H

#include "SimpleIni.h"
#include <stdint.h>
#include <string>

using namespace std;

class CConfig {
public:
	CConfig(const char *app_name);
	~CConfig();

	int GetInt(const char *section, const char *key, int default_value);
	void SetInt(const char *section, const char *key, int value);
	string GetString(const char *section, const char *key, const string &default_value);
	void SetString(const char *section, const char *key, const string &value);
	bool GetBool(const char *section, const char *key, bool default_value);
	void SetBool(const char *section, const char *key, bool value);

	size_t LoadDataFile(const char *prefix, const char *filename,
			uint8_t *buffer, size_t size, size_t offset = 0);
	bool SaveDataFile(const char *prefix, const char *filename,
			const uint8_t *buffer, size_t size);
	string FindDataFile(const char *prefix, const char *filename);

private:
	bool changed;
	string app_name;
	string user_path;
	string share_path;
	CSimpleIni ini;
};

#endif // ZEMU_CONFIG_H
