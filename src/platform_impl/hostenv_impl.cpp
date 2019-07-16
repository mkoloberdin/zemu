// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "hostenv_impl.h"
#include "logger_impl.h"
#include "filesystem_impl.h"
#include "finder_impl.h"
#include "config_impl.h"

HostEnvImpl::HostEnvImpl(int argc, const char** argv, const std::string& applicationId) : applicationId(applicationId) {
    executableDirPath = (argc ? fileSystem()->path(argv[0])->parent() : fileSystem()->path(""));
}

LoggerPtr HostEnvImpl::logger() {
    if (!loggerPtr) {
        loggerPtr = LoggerPtr(new LoggerImpl());
    }

    return loggerPtr;
}

FileSystemPtr HostEnvImpl::fileSystem() {
    if (!fileSystemPtr) {
        fileSystemPtr = FileSystemPtr(new FileSystemImpl(applicationId));
    }

    return fileSystemPtr;
}

FinderPtr HostEnvImpl::finder() {
    if (!finderPtr) {
        finderPtr = FinderPtr(new FinderImpl(fileSystem(), executableDirPath));
    }

    return finderPtr;
}

ConfigPtr HostEnvImpl::config() {
    if (!configPtr) {
        configPtr = ConfigPtr(new ConfigImpl(applicationId, finder(), logger()));
    }

    return configPtr;
}

uintmax_t HostEnvImpl::loadDataFile(
    const char* directory,
    const std::string& fileName,
    uint8_t* buffer,
    uintmax_t size,
    uintmax_t offset
) {
    auto path = finder()->find(directory, fileName);

    if (path->isEmpty()) {
        return 0;
    }

    auto reader = path->fileReader();

    if (offset) {
        reader->setPosition(offset);
    }

    return reader->readBlock(buffer, size);
}
