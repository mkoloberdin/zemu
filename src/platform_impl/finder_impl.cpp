// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "finder_impl.h"

FinderImpl::FinderImpl(const FileSystemPtr& fileSystem, const PathPtr& executableDirPath) : fileSystem(fileSystem),
    executableDirPath(executableDirPath)
{
    // 1. try in current folder (allow use custom-configs for folder)
    searchPaths.push_back(fileSystem->path(""));

    // 2. try in executable dir (useful for debug version && file associations)
    searchPaths.push_back(executableDirPath);

    // 3. try in home folder
    searchPaths.push_back(fileSystem->appDataPath());

    #ifdef SHARE_PATH
        // 4. try in shared folder
        searchPaths.push_back(fileSystem->path(SHARE_PATH));
    #endif
}

PathPtr FinderImpl::find(const std::string& fileName) {
    for (const auto& searchPath : searchPaths) {
        auto path = searchPath->append(fileName);

        if (path->fileExists()) {
            return path;
        }
    }

    return fileSystem->path("");
}

PathPtr FinderImpl::find(const std::string& directory, const std::string& fileName) {
    for (const auto& searchPath : searchPaths) {
        auto path = searchPath->append(directory)->append(fileName);

        if (path->fileExists()) {
            return path;
        }
    }

    return fileSystem->path("");
}
