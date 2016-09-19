#ifndef ZEMU_APPENV_H_INCLUDED
#define ZEMU_APPENV_H_INCLUDED

#include "SimpleIni.h"
#include <cstdint>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <string>
#include <vector>

namespace fs = boost::filesystem;

class AppEnv {
public:
  AppEnv();
  ~AppEnv();

  void initialize(const char *app_name);
  int getInt(const char *section, const char *key, int default_value);
  void setInt(const char *section, const char *key, int value);
  std::string getString(const std::string& section, const std::string& key,
                        const std::string& default_value);
  void setString(const std::string& section, const std::string& key, const std::string& value);
  bool getBool(const char *section, const char *key, bool default_value);
  void setBool(const char *section, const char *key, bool value);

  fs::path findConfigFile(const fs::path& filename);
  fs::path findConfigFile(const fs::path& prefix, const fs::path& filename);
  
  fs::path getDataHome();
  fs::path newDataFilePath(const fs::path& filename);

  size_t loadDataFile(const fs::path& filename,
                      uint8_t *buffer, size_t size, size_t offset = 0);
  size_t loadDataFile(const fs::path& prefix, const fs::path& filename,
                      uint8_t *buffer, size_t size, size_t offset = 0);

  // [not used] bool SaveDataFile(const char *prefix, const char *filename, const uint8_t *buffer, size_t size);
  fs::path findDataFile(const fs::path& filename);
  fs::path findDataFile(const fs::path& prefix, const fs::path& filename);

private:
  fs::path DataHome;                // $XDG_DATA_HOME
  fs::path ConfigHome;              // $XDG_CONFIG_HOME
  std::vector<fs::path> DataDirs;   // $XDG_DATA_DIRS
  std::vector<fs::path> ConfigDirs; // $XDG_CONFIG_DIRS
  fs::path CacheHome;               // $XDG_CACHE_HOME
  fs::path RuntimeDir;              // $XDG_RUNTIME_DIR
  
  bool Changed;
  std::string IniName;
  CSimpleIni Ini;

  AppEnv &operator= (const AppEnv &dummy) { return *this; }
  AppEnv(const AppEnv &dummy) {}
};

#endif // ZEMU_APPENV_H_INCLUDED
