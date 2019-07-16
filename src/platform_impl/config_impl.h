#ifndef PLATFORM_IMPL__CONFIG_IMPL_H__INCLUDED
#define PLATFORM_IMPL__CONFIG_IMPL_H__INCLUDED

#include "platform/config.h"
#include "platform/finder.h"
#include "platform/logger.h"
#include "lib_simpleini/SimpleIni.h"

class ConfigImpl : public Config {
public:

    ConfigImpl(const std::string& applicationId, const FinderPtr& finder, const LoggerPtr& logger);
    ~ConfigImpl();

    std::string getString(const char* section, const char* key, const std::string& defaultValue);
    void setString(const char* section, const char* key, const std::string& value);
    int getInt(const char* section, const char* key, int defaultValue);
    void setInt(const char* section, const char* key, int value);
    bool getBool(const char* section, const char* key, bool defaultValue);
    void setBool(const char* section, const char* key, bool value);

private:

    std::string applicationId;
    FinderPtr finder;
    LoggerPtr logger;

    bool isChanged = false;
    PathPtr configPath;
    CSimpleIni ini;
};

#endif
