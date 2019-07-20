#ifndef HOST_IMPL__HOST_IMPL_H__INCLUDED
#define HOST_IMPL__HOST_IMPL_H__INCLUDED

#include <string>
#include "host/host.h"

class HostImpl : public Host {
public:

    HostImpl(int argc, char** argv, const std::string& applicationId);
    ~HostImpl();

    void setStageConfig(const StageConfig& stageConfig);

    Logger* logger();
    Storage* storage();
    Config* config();
    Timer* timer();
    Stage* stage();

private:

    std::string applicationId;
    std::string executablePathStr;
    std::unique_ptr<StageConfig> stageConfigInstance;

    std::unique_ptr<Logger> loggerInstance;
    std::unique_ptr<Storage> storageInstance;
    std::unique_ptr<Config> configInstance;
    std::unique_ptr<Timer> timerInstance;
    std::unique_ptr<Stage> stageInstance;
};

#endif
