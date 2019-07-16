#ifndef PLATFORM_IMPL__FINDER_IMPL_H__INCLUDED
#define PLATFORM_IMPL__FINDER_IMPL_H__INCLUDED

#include <vector>
#include "platform/finder.h"

class FinderImpl : public Finder {
public:

    FinderImpl(const FileSystemPtr& fileSystem, const PathPtr& executableDirPath);

    PathPtr find(const std::string& fileName);
    PathPtr find(const std::string& directory, const std::string& fileName);

private:

    FileSystemPtr fileSystem;
    PathPtr executableDirPath;

    std::vector<PathPtr> searchPaths;
};

#endif
