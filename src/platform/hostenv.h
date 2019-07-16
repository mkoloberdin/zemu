#ifndef PLATFORM__HOSTENV_H__INCLUDED
#define PLATFORM__HOSTENV_H__INCLUDED

#include <cstdint>
#include <memory>
#include "logger.h"
#include "filesystem.h"
#include "finder.h"
#include "config.h"

class HostEnv {
    public:

    HostEnv() {}
    virtual ~HostEnv() {}

    virtual LoggerPtr logger() = 0;
    virtual FileSystemPtr fileSystem() = 0;
    virtual FinderPtr finder() = 0;
    virtual ConfigPtr config() = 0;

    virtual uintmax_t loadDataFile(
        const char* directory,
        const std::string& fileName,
        uint8_t* buffer,
        uintmax_t size,
        uintmax_t offset = 0
    ) = 0;

    private:

    HostEnv(const HostEnv&);
    HostEnv& operator=(const HostEnv&);
};

typedef std::shared_ptr<HostEnv> HostEnvPtr;

#endif
