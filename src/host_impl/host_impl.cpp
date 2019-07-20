// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "host_impl.h"
#include "logger_impl.h"
#include "storage_impl.h"
#include "config_impl.h"

HostImpl::HostImpl(int argc, char** argv, const std::string& applicationId) : applicationId(applicationId) {
    executablePathStr = std::string(argc ? argv[0] : "");
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
    return nullptr;
}

Stage* HostImpl::stage() {
    return nullptr;
}
