#ifndef PLATFORM__FINDER_H__INCLUDED
#define PLATFORM__FINDER_H__INCLUDED

#include <memory>
#include <string>
#include "filesystem.h"

class Finder {
    public:

    Finder() {}
    virtual ~Finder() {}

    virtual PathPtr find(const std::string& fileName) = 0;
    virtual PathPtr find(const std::string& directory, const std::string& fileName) = 0;

    private:

    Finder(const Finder&);
    Finder& operator=(const Finder&);
};

typedef std::shared_ptr<Finder> FinderPtr;

#endif
