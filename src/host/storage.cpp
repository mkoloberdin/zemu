// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <boost/algorithm/string.hpp>
#include "storage.h"

std::string Path::extensionLc() {
    std::string ext = extension();
    boost::algorithm::to_lower(ext);
    return ext;
}
