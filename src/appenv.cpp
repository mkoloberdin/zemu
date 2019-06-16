#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "appenv.h"

#ifdef _WIN32
  #ifndef _WIN32_IE
    #define _WIN32_IE 0x400
  #endif

  #include <shlobj.h>
  #include <io.h>
  #include <direct.h> // for _mkdir()
#else
  #include <sys/stat.h>
  #include <sys/types.h>
#endif // _WIN32

#ifdef _WIN32
  #include <shlwapi.h>
#endif

namespace {

fs::path throw_if_not_absolute(const std::string& desc, const fs::path& p) {
  if (p.has_root_directory())
    return p;

  throw std::runtime_error { desc + " must be an absolute path." };
}

namespace env {

std::string get(const std::string& key, const std::string& default_value) {
  if (auto value = std::getenv(key.c_str()))
    return value;

  return default_value;
}

std::string get_or_throw(const std::string& key) {
  if (auto value = std::getenv(key.c_str()))
    return value;

  throw std::runtime_error { key + " is not set in environment" };
}

constexpr const char* xdg_data_home { "XDG_DATA_HOME" };
constexpr const char* xdg_data_dirs { "XDG_DATA_DIRS" };
constexpr const char* xdg_config_home { "XDG_CONFIG_HOME" };
constexpr const char* xdg_config_dirs { "XDG_CONFIG_DIRS" };
constexpr const char* xdg_cache_home { "XDG_CACHE_HOME" };
constexpr const char* xdg_runtime_dir { "XDG_RUNTIME_DIR" };
} // namespace env

namespace basedir {

fs::path data_home() {
  auto v = env::get(env::xdg_data_home, "");
  if (v.empty())
    return throw_if_not_absolute("HOME",
        fs::path { env::get_or_throw("HOME") } / ".local" / "share");

  return throw_if_not_absolute(env::xdg_data_home, fs::path(v));
}

fs::path config_home() {
  auto v = env::get(env::xdg_config_home, "");
  if (v.empty())
    return throw_if_not_absolute("HOME", fs::path { env::get_or_throw("HOME") } / ".config");

  return throw_if_not_absolute(env::xdg_config_home, fs::path(v));
}

std::vector<fs::path> data_dirs() {
  auto v = env::get(env::xdg_data_dirs, "");
  if (v.empty())
    return {fs::path {"/usr/local/share"}, fs::path {"/usr/share"}};

  std::vector<std::string> tokens;
  tokens = boost::split(tokens, v, boost::is_any_of(":"));
  std::vector<fs::path> result;
  for (const auto& token : tokens) {
    result.push_back(
        throw_if_not_absolute(token + " (from " + env::xdg_data_dirs + ")", fs::path(token)));
  }
  return result;
}

std::vector<fs::path> config_dirs() {
  auto v = env::get(env::xdg_config_dirs, "");
  if (v.empty())
    return {fs::path {"/etc/xdg"}};

  std::vector<std::string> tokens;
  tokens = boost::split(tokens, v, boost::is_any_of(":"));
  std::vector<fs::path> result;
  for (const auto& token : tokens) {
    fs::path p(token);
    result.push_back(throw_if_not_absolute(token + " (from " + env::xdg_config_dirs + ")", p));
  }
  return result;
}

} // namespace basedir
} // empty namespace


AppEnv::AppEnv() {
  Changed = false;
}

AppEnv::~AppEnv() {
  if (Changed && !ConfigHome.empty() && !IniName.empty()) {
    Ini.SaveFile((ConfigHome / IniName).c_str());
  }
}

void AppEnv::initialize(const char *app_name) {
  Changed = false;
  std::string AppName = std::string(app_name);
  IniName = AppName + ".ini";

  DataHome = basedir::data_home() / fs::path{AppName};
  if(!fs::exists(DataHome))
    fs::create_directories(DataHome);
  
  ConfigHome = basedir::config_home() / fs::path{AppName};
  if(!fs::exists(ConfigHome))
    fs::create_directories(ConfigHome);
  
  DataDirs = basedir::data_dirs();
  for(auto& dir : DataDirs) {
    dir = dir / AppName;
  }
  DataDirs.insert(DataDirs.begin(), DataHome);

  ConfigDirs = basedir::config_dirs();
  for(auto& dir : ConfigDirs) {
    dir = dir / AppName;
  }
  ConfigDirs.insert(ConfigDirs.begin(), ConfigHome);
  
  if (!Ini.LoadFile((findConfigFile(IniName)).c_str())) {
    printf("Config loaded successfully\n");
    return;
  }
}

// FIXME: WIN32 support
/*
void AppEnv::EnsurePaths(const char *app_name) {
  if (!home_path.empty()) {
    return;
  }

#ifdef _WIN32
  TCHAR buffer[MAX_PATH];

  if (SHGetSpecialFolderPath(0, buffer, CSIDL_APPDATA, true)) {
    home_path = string(buffer);
  }
#else
  if (getenv("XDG_CONFIG_HOME")) {
    home_path = getenv("XDG_CONFIG_HOME");
  } else if (getenv("HOME")) {
    if (C_File::FileExists(C_DirWork::Append(getenv("HOME"), ".config").c_str())) {
      home_path = C_DirWork::Append(getenv("HOME"), ".config");
    } else {
      home_path = string(getenv("HOME"));
    }
  }
#endif

  if (home_path.empty()) {
    home_path = ".";
  } else {
    home_path = C_DirWork::Append(home_path, app_name);

#ifdef _WIN32
    _mkdir(home_path.c_str());
#else
    mkdir(home_path.c_str(), 0755);
#endif
  }
}
*/

int AppEnv::getInt(const char *section, const char *key, int default_value) {
  const char *cval = Ini.GetValue(section, key, nullptr);
  if (cval) {
    return atoi(cval);
  } else {
    setInt(section, key, default_value);
    return default_value;
  }
}

void AppEnv::setInt(const char *section, const char *key, int value) {
  char buffer[0x100];
  sprintf(buffer, "%d", value);
  Ini.SetValue(section, key, buffer);
  Changed = true;
}

std::string AppEnv::getString(const std::string& section, const std::string& key,
                              const std::string& default_value) {
  const char *cval = Ini.GetValue(section.c_str(), key.c_str(), nullptr);
  if (cval) {
    return std::string(cval);
  } else {
    setString(section, key, default_value);
    return default_value;
  }
}

void AppEnv::setString(const std::string& section, const std::string& key,
                       const std::string& value) {
  Ini.SetValue(section.c_str(), key.c_str(), value.c_str());
  Changed = true;
}

bool AppEnv::getBool(const char *section, const char *key, bool default_value) {
  const char *cval = Ini.GetValue(section, key, nullptr);
  if (cval) {
    if (!strcasecmp(cval, "true") || !strcasecmp(cval, "yes")) {
      return true;
    } else {
      return atoi(cval) ? true : false;
    }
  } else {
    setBool(section, key, default_value);
    return default_value;
  }
}

void AppEnv::setBool(const char *section, const char *key, bool value) {
  Ini.SetValue(section, key, value ? "yes" : "no");
  Changed = true;
}

fs::path AppEnv::findConfigFile(const fs::path& filename) {
  return findConfigFile("", filename);
}

fs::path AppEnv::findConfigFile(const fs::path& prefix, const fs::path& filename) {
  for (auto& p : ConfigDirs) {
    fs::path f = p / prefix / filename;
    if (fs::exists(f))
      return f;
  }
  return fs::path{""};
}

fs::path AppEnv::getDataHome() {
  return DataHome;
}

fs::path AppEnv::newDataFilePath(const fs::path& filename){
  return DataHome / filename;
}

size_t AppEnv::loadDataFile(const fs::path& filename,
                            uint8_t *buffer, size_t size, size_t offset) {
  fs::path path = AppEnv::findDataFile(filename);
  std::cout << "findDataFile() returned " << path << std::endl;

  if (path.empty()) {
    return 0;
  }

  fs::ifstream IFS(fs::path(path), std::ios_base::binary);
  
  if(offset)
    IFS.seekg(offset, std::ios_base::beg);
  
  IFS.read((char *)buffer, size);
  return IFS.gcount();
}

size_t AppEnv::loadDataFile(const fs::path& prefix, const fs::path& filename,
                            uint8_t *buffer, size_t size, size_t offset) {
  return loadDataFile(prefix / filename, buffer, size, offset);
}

/*
 * Not used
 *
bool CConfig::SaveDataFile(const char *prefix, const char *filename, const uint8_t *buffer, size_t size) {
	string folder = C_DirWork::Append(user_path, prefix);

	#ifdef _WIN32
		mkdir(folder.c_str());
	#else
		mkdir(folder.c_str(), 0755);
	#endif

	C_File fl;
	fl.Write(C_DirWork::Append(folder, filename).c_str());
	fl.WriteBlock(buffer, size);
	fl.Close();

	return true;
}
*/

fs::path AppEnv::findDataFile(const fs::path& filename) {
  // If filename's path is absolute just return it
  if(filename.has_root_directory()) {
    return filename;
  } else {
    // otherwise try to find the file in data directories
    for (auto p : DataDirs) {
      fs::path f = p / filename;
      std::cout << "findDataFile: checking for " << f << std::endl;
      if (fs::exists(f))
        return f;
    }
    return fs::path{""};
  }
}

fs::path AppEnv::findDataFile(const fs::path& prefix, const fs::path& filename) {
  std::cout << "findDataFile: filename=" << filename << std::endl;

  return findDataFile(prefix / filename);
}
