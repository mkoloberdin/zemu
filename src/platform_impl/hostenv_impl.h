#ifndef PLATFORM_IMPL__HOSTENV_IMPL_H__INCLUDED
#define PLATFORM_IMPL__HOSTENV_IMPL_H__INCLUDED

#include <string>
#include "platform/hostenv.h"

class HostEnvImpl : public HostEnv {
public:

    HostEnvImpl(int argc, const char** argv, const std::string& applicationId);

    Logger* logger();
    Storage* storage();
    Config* config();

private:

    std::string applicationId;
    std::string executablePathStr;

    std::unique_ptr<Logger> loggerInstance;
    std::unique_ptr<Storage> storageInstance;
    std::unique_ptr<Config> configInstance;
};

#endif
