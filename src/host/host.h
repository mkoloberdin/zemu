#ifndef HOST__HOST_H__INCLUDED
#define HOST__HOST_H__INCLUDED

#include <cstdint>
#include <memory>
#include "logger.h"
#include "storage.h"
#include "config.h"
#include "timer.h"
#include "stage.h"

class Host {
public:

    Host() {}
    virtual ~Host() {}

    virtual void setStageConfig(const StageConfig& stageConfig) = 0;

    virtual Logger* logger() = 0;
    virtual Storage* storage() = 0;
    virtual Config* config() = 0;
    virtual Timer* timer() = 0;
    virtual Stage* stage() = 0;

private:

    Host(const Host&);
    Host& operator=(const Host&);
};

#endif
