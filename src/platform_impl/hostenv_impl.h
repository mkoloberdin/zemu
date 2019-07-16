#ifndef PLATFORM_IMPL__HOSTENV_IMPL_H__INCLUDED
#define PLATFORM_IMPL__HOSTENV_IMPL_H__INCLUDED

#include <string>
#include "platform/hostenv.h"

class HostEnvImpl : public HostEnv {
    public:

    HostEnvImpl(int argc, const char** argv, const std::string& applicationId);

    LoggerPtr logger();
    FileSystemPtr fileSystem();
    FinderPtr finder();
    ConfigPtr config();

    uintmax_t loadDataFile(
        const char* directory,
        const std::string& fileName,
        uint8_t* buffer,
        uintmax_t size,
        uintmax_t offset = 0
    );

    private:

    std::string applicationId;
    PathPtr executableDirPath;

    LoggerPtr loggerPtr;
    FileSystemPtr fileSystemPtr;
    FinderPtr finderPtr;
    ConfigPtr configPtr;
};

#endif
