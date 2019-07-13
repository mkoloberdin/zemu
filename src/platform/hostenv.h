#ifndef PLATFORM__HOSTENV_H__INCLUDED
#define PLATFORM__HOSTENV_H__INCLUDED

#include "filesystem.h"

class HostEnv {
    public:

    HostEnv() {}
    virtual ~HostEnv() {}

    virtual FileSystem* fileSystem() = 0;

    private:

    HostEnv(const HostEnv&);
    HostEnv& operator=(const HostEnv&);
};

class HostEnvImpl : public HostEnv {
    public:

    FileSystem* fileSystem();

    private:

    FileSystemImpl fileSystemInstance;
};

#endif
