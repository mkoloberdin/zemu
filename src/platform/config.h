#ifndef PLATFORM__CONFIG_H__INCLUDED
#define PLATFORM__CONFIG_H__INCLUDED

#include <memory>
#include <string>

class Config {
    public:

    Config() {}
    virtual ~Config() {}

    virtual std::string getString(const char* section, const char* key, const std::string& defaultValue) = 0;
    virtual void setString(const char* section, const char* key, const std::string& value) = 0;
    virtual int getInt(const char* section, const char* key, int defaultValue) = 0;
    virtual void setInt(const char* section, const char* key, int value) = 0;
    virtual bool getBool(const char* section, const char* key, bool defaultValue) = 0;
    virtual void setBool(const char* section, const char* key, bool value) = 0;

    private:

    Config(const Config&);
    Config& operator=(const Config&);
};

typedef std::shared_ptr<Config> ConfigPtr;

#endif
