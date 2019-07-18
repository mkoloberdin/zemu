#ifndef PLATFORM__HOSTENV_H__INCLUDED
#define PLATFORM__HOSTENV_H__INCLUDED

#include <cstdint>
#include <memory>
#include "logger.h"
#include "storage.h"
#include "config.h"
#include "hardware.h"

class HostEnv {
public:

    HostEnv() {}
    virtual ~HostEnv() {}

    virtual Logger* logger() = 0;
    virtual Storage* storage() = 0;
    virtual Config* config() = 0;
    virtual Hardware* hardware() = 0;

private:

    HostEnv(const HostEnv&);
    HostEnv& operator=(const HostEnv&);
};

#endif
