#ifndef ZEMU_CONFIG_H
#define ZEMU_CONFIG_H

#include "SimpleIni.h"
#include <stdint.h>
#include <string>

using namespace std;

class CConfig {
public:
	CConfig();
	~CConfig();

	void Initialize(const char *app_name);
	int GetInt(const char *section, const char *key, int default_value);
	void SetInt(const char *section, const char *key, int value);
	string GetString(const char *section, const char *key, const string &default_value);
	void SetString(const char *section, const char *key, const string &value);
	bool GetBool(const char *section, const char *key, bool default_value);
	void SetBool(const char *section, const char *key, bool value);

	size_t LoadDataFile(const char *prefix, const char *filename, uint8_t *buffer, size_t size, size_t offset = 0);
	// [not used] bool SaveDataFile(const char *prefix, const char *filename, const uint8_t *buffer, size_t size);
	string FindDataFile(const char *prefix, const char *filename);

	static char *executableDir;

private:
	bool changed;
	string ini_name;
	string user_path;
	string home_path;
	CSimpleIni ini;

	void EnsurePaths(const char *app_name);

	CConfig& operator=(const CConfig &dummy) {}
	CConfig(const CConfig &dummy) {}
};

#endif // ZEMU_CONFIG_H
