// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdexcept>
#include "host_impl.h"
#include "logger_impl.h"
#include "storage_impl.h"
#include "config_impl.h"
#include "timer_impl.h"
#include "stage_impl.h"

HostImpl::HostImpl(int argc, char** argv, const std::string& applicationId) : applicationId(applicationId) {
    executablePathStr = std::string(argc ? argv[0] : "");

    if (SDL_Init(SDL_INIT_TIMER) < 0) {
        throw std::runtime_error(std::string("SDL_Init() failed: ") + SDL_GetError());
    }
}

HostImpl::~HostImpl() {
    stageInstance.reset(); // destruct stage before calling SDL_Quit()
    SDL_Quit();
}

void HostImpl::setStageConfig(const StageConfig& stageConfig) {
    if (stageInstance) {
        throw std::logic_error("setStageConfig() should not be called after stage() is used");
    }

    stageConfigInstance.reset(new StageConfig(stageConfig));
}

Logger* HostImpl::logger() {
    if (!loggerInstance) {
        loggerInstance.reset(new LoggerImpl());
    }

    return loggerInstance.get();
}

Storage* HostImpl::storage() {
    if (!storageInstance) {
        storageInstance.reset(new StorageImpl(applicationId, executablePathStr));
    }

    return storageInstance.get();
}

Config* HostImpl::config() {
    if (!configInstance) {
        configInstance.reset(new ConfigImpl(applicationId, storage(), logger()));
    }

    return configInstance.get();
}

Timer* HostImpl::timer() {
    if (!timerInstance) {
        timerInstance.reset(new TimerImpl());
    }

    return timerInstance.get();
}

Stage* HostImpl::stage() {
    if (!stageInstance) {
        if (!stageConfigInstance) {
            throw std::logic_error("setStageConfig() must be called before using stage()");
        }

        stageInstance.reset(new StageImpl(*stageConfigInstance, logger()));
    }

    return stageInstance.get();
}
