// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "config_impl.h"

ConfigImpl::ConfigImpl(const std::string& applicationId, Storage* storage, Logger* logger) {
    auto path = storage->findExtras(applicationId + ".ini");

    if (!ini.LoadFile(path->string().c_str())) {
        logger->log("Config loaded successfully from \"%s\"", path->string().c_str());
        configPath = path;
    } else {
        logger->log("Failed to find config");
    }
}

ConfigImpl::~ConfigImpl() {
    if (isChanged && !configPath->isEmpty()) {
        ini.SaveFile(configPath->string().c_str());
    }
}

std::string ConfigImpl::getString(const char* section, const char* key, const std::string& defaultValue) {
    const char* value = ini.GetValue(section, key, nullptr);

    if (value) {
        return std::string(value);
    }

    setString(section, key, defaultValue);
    return defaultValue;
}

void ConfigImpl::setString(const char* section, const char* key, const std::string& value) {
    ini.SetValue(section, key, value.c_str());
    isChanged = true;
}

int ConfigImpl::getInt(const char* section, const char* key, int defaultValue) {
    const char* value = ini.GetValue(section, key, nullptr);

    if (value) {
        return atoi(value);
    }

    setInt(section, key, defaultValue);
    return defaultValue;
}

void ConfigImpl::setInt(const char* section, const char* key, int value) {
    ini.SetValue(section, key, std::to_string(value).c_str());
    isChanged = true;
}

bool ConfigImpl::getBool(const char* section, const char* key, bool defaultValue) {
    const char* value = ini.GetValue(section, key, nullptr);

    if (value) {
        if (!strcasecmp(value, "true") || !strcasecmp(value, "yes")) {
            return true;
        }

        return atoi(value);
    }

    setBool(section, key, defaultValue);
    return defaultValue;
}

void ConfigImpl::setBool(const char* section, const char* key, bool value) {
    ini.SetValue(section, key, value ? "yes" : "no");
    isChanged = true;
}
