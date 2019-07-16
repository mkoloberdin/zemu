// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <stdarg.h>
#include <boost/algorithm/string.hpp>
#include "filesystem_impl.h"

#ifdef _WIN32
    #ifndef _WIN32_IE
        #define _WIN32_IE 0x400
    #endif

    #include <fileapi.h>
    #include <shlobj.h>
#endif

//
// FileSystemImpl
//

PathPtr FileSystemImpl::path(const std::string& path) {
    return PathPtr(new PathImpl(path));
}

PathPtr FileSystemImpl::appDataPath() {
    if (!appDataPathPtr) {
        #ifdef _WIN32
            const TCHAR path[MAX_PATH];

            if (SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_APPDATA | CSIDL_FLAG_CREATE, nullptr, 0, path))) {
                appDataPathPtr.reset(new PathImpl(std::string(path)));
            } else {
                appDataPathPtr.reset(new PathImpl(std::string("")));
            }
        #else
            const char* configHomePath = std::getenv("XDG_CONFIG_HOME");

            if (configHomePath) {
                appDataPathPtr.reset(new PathImpl(std::string(configHomePath)));
            } else {
                const char* homePath = std::getenv("HOME");

                if (homePath) {
                    appDataPathPtr = PathPtr(new PathImpl(std::string(homePath)))->append(".config");
                    appDataPathPtr->createDirectory();
                } else {
                    appDataPathPtr.reset(new PathImpl(std::string("")));
                }
            }
        #endif

        if (!appDataPathPtr->isEmpty()) {
            appDataPathPtr = appDataPathPtr->append(applicationId);
            appDataPathPtr->createDirectory();
        }
    }

    return appDataPathPtr;
}

//
// PathImpl
//

PathImpl::PathImpl(const std::string& path) {
    #ifdef _WIN32
        // path -> host | platform
        std::string hostPathStr;
        std::string platformPathStr;

        if (path.length() < 2) {
            // "" -> "" | ""
            // "x" -> "x" | "x"
            // "/" -> "/" | ""

            hostPathStr = path;
            platformPathStr = ((path == "/") ? "" : path);
        } else if (path[1] == ':') {
            // "c:x..." -> "/c/x..." | "c:/x..."
            // "c:/x..." -> "/c/x..." | "c:/x..."

            hostPathStr = path;
            platformPathStr = path;

            if (path.length() > 2 && path[2] != '/') {
                hostPathStr.insert(2, 1, '/');
                platformPathStr.insert(2, 1, '/');
            }

            hostPathStr[1] = hostPathStr[0];
            hostPathStr[0] = '/';
        } else if (path[0] != '/') {
            // relative path, nothing to convert

            hostPathStr = path;
            platformPathStr = path;
        } else if (path.length() == 2 || path[2] == '/') {
            // "/c" -> "/c" | "c:"
            // "/c/..." -> "/c/..." | "c:/..."

            hostPathStr = path;
            platformPathStr = path;

            platformPathStr[0] = platformPathStr[1];
            platformPathStr[1] = ':';
        } else {
            // "/abc..." -> "/Z/abc..." | "Z:/abc" where "Z" is current drive

            boost::system::error_code ec;
            auto currentPath = boost::filesystem::current_path(ec);

            if (ec) {
                // an error occurred, so just ignore such path
            } else {
                platformPathStr = currentPath.string() + path;
                hostPathStr = platformPathStr;

                hostPathStr[1] = hostPathStr[0];
                hostPathStr[0] = '/';
            }
        }

        boost::algorithm::replace_all(hostPathStr, "\\", "/");

        hostPath = hostPathStr;
        platformPath = platformPathStr;
    #else
        hostPath = path;
    #endif

    isAbsolute = hostPath.is_absolute();
}

bool PathImpl::isEmpty() {
    return hostPath.empty();
}

bool PathImpl::isRoot() {
    boost::filesystem::path path;

    if (!isAbsolute) {
        boost::system::error_code ec;
        auto currentPath = boost::filesystem::current_path(ec);

        if (ec) {
            return false;
        }

        path = boost::filesystem::absolute(hostPath, currentPath);
    } else {
        path = hostPath;
    }

    // boost::filesystem::equivalent() is not used on purpose:
    // - it can fail;
    // - we just don't need it here, because our paths are carefuly crafted.

    return path.has_root_path() && path.root_path() == path;
}

std::string PathImpl::string() {
    return hostPath.string();
}

std::string PathImpl::fileName() {
    return hostPath.filename().string();
}

std::string PathImpl::extension() {
    return hostPath.extension().string();
}

PathPtr PathImpl::parent() {
    #ifdef _WIN32
        auto parentHostPath = hostPath.parent_path();

        if (isAbsolute && parentHostPath.has_root_path() && parentHostPath.root_path() == parentHostPath) {
            return PathPtr(new PathImpl(boost::filesystem::path(), parentHostPath));
        }

        return PathPtr(new PathImpl(platformPath.parent_path(), parentHostPath));
    #else
        return PathPtr(new PathImpl(hostPath.parent_path()));
    #endif
}

PathPtr PathImpl::concat(const std::string& value) {
    #ifdef _WIN32
        boost::filesystem::path newPlatformPath(hostPath);
        newPlatformPath += value;
    #endif

    boost::filesystem::path newHostPath(hostPath);
    newHostPath += value;

    #ifdef _WIN32
        return PathPtr(new PathImpl(newPlatformPath, newHostPath));
    #else
        return PathPtr(new PathImpl(newHostPath));
    #endif
}

PathPtr PathImpl::append(const std::string& value) {
    #ifdef _WIN32
        boost::filesystem::path newPlatformPath(hostPath);
        newPlatformPath /= value;
    #endif

    boost::filesystem::path newHostPath(hostPath);
    newHostPath /= value;

    #ifdef _WIN32
        return PathPtr(new PathImpl(newPlatformPath, newHostPath));
    #else
        return PathPtr(new PathImpl(newHostPath));
    #endif
}

PathPtr PathImpl::canonical() {
    boost::system::error_code ec;

    #ifdef _WIN32
        auto canonicalPath = boost::filesystem::canonical(platformPath, ec);
        return PathPtr(new PathImpl(ec ? "" : canonicalPath.string()));
    #else
        auto canonicalPath = boost::filesystem::canonical(hostPath, ec);
        return PathPtr(ec ? new PathImpl(std::string("")) : new PathImpl(canonicalPath));
    #endif
}

bool PathImpl::exists() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::exists(platformPath, ec);
    #else
        bool result = boost::filesystem::exists(hostPath, ec);
    #endif

    return result && !ec;
}

bool PathImpl::fileExists() {
    if (!exists()) {
        return false;
    }

    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::is_regular_file(platformPath, ec);
    #else
        bool result = boost::filesystem::is_regular_file(hostPath, ec);
    #endif

    return result && !ec;
}

bool PathImpl::isDirectory() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::is_directory(platformPath, ec);
    #else
        bool result = boost::filesystem::is_directory(hostPath, ec);
    #endif

    return result && !ec;
}

uintmax_t PathImpl::fileSize() {
    boost::system::error_code ec;

    #ifdef _WIN32
        uintmax_t result = boost::filesystem::file_size(platformPath, ec);
    #else
        uintmax_t result = boost::filesystem::file_size(hostPath, ec);
    #endif

    return (ec ? 0 : result);
}

bool PathImpl::remove() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::remove(platformPath, ec);
    #else
        bool result = boost::filesystem::remove(hostPath, ec);
    #endif

    return result && !ec;
}

bool PathImpl::createDirectory() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::create_directory(platformPath, ec);
    #else
        bool result = boost::filesystem::create_directory(hostPath, ec);
    #endif

    return result && !ec;
}

FileReaderPtr PathImpl::fileReader() {
    #ifdef _WIN32
        return FileReaderPtr(new FileReaderImpl(platformPath));
    #else
        return FileReaderPtr(new FileReaderImpl(hostPath));
    #endif
}

FileWriterPtr PathImpl::fileWriter() {
    #ifdef _WIN32
        return FileWriterPtr(new FileWriterImpl(platformPath));
    #else
        return FileWriterPtr(new FileWriterImpl(hostPath));
    #endif
}

void PathImpl::listEntries(std::vector<PathPtr>& into) {
    into.clear();

    #ifdef _WIN32
        if (isRoot()) {
            char* drivePath = "X:";

            for (char driveLetter = 'A'; driveLetter <= 'Z'; driveLetter++) {
                drivePath[0] = driveLetter;
                unsigned driveType = GetDriveType(drivePath);

                if (driveType == DRIVE_REMOVABLE
                    || driveType == DRIVE_FIXED
                    || driveType == DRIVE_REMOTE
                    || driveType == DRIVE_CDROM
                    || driveType == DRIVE_RAMDISK
                ) {
                    into.push_back(PathPtr(new PathImpl(drivePath)));
                }
            }
        }
    #endif

    if (!isRoot()) {
        into.push_back(append(".."));
    }

    if (!exists()) {
        return;
    }

    boost::system::error_code ec;

    #ifdef _WIN32
        auto iterator = boost::filesystem::directory_iterator(platformPath);
    #else
        auto iterator = boost::filesystem::directory_iterator(hostPath);
    #endif

    if (ec) {
        return;
    }

    for (const auto& entry : iterator) {
        into.push_back(PathPtr(new PathImpl(entry.path())));
    }
}

//
// FileReaderImpl
//

FileReaderImpl::FileReaderImpl(const boost::filesystem::path& path) {
    ifs.open(path.string(), std::ifstream::in | std::ifstream::binary);
}

FileReaderImpl::~FileReaderImpl() {
    ifs.close();
}

bool FileReaderImpl::isEof() {
    return ifs.eof();
}

char FileReaderImpl::readChar() {
    return ifs.get();
}

uint8_t FileReaderImpl::readByte() {
    return ifs.get();
}

uint16_t FileReaderImpl::readWord() {
    int l = ifs.get();
    int h = ifs.get();
    return (uint16_t)(l + 0x100 * h);
}

uint32_t FileReaderImpl::readDword() {
    uint32_t l = readWord();
    uint32_t h = readWord();
    return (uint32_t)(l + (uint32_t)0x10000 * h);
}

uintmax_t FileReaderImpl::readBlock(void* buffer, uintmax_t size) {
    ifs.read((char*)buffer, size);
    return ifs.gcount();
}

std::string FileReaderImpl::readLine() {
    std::string result;
    std::getline(ifs, result);
    return result;
}

uintmax_t FileReaderImpl::getPosition() {
    return ifs.tellg();
}

void FileReaderImpl::setPosition(uintmax_t position) {
    ifs.seekg(position);
}

//
// FileWriterImpl
//

FileWriterImpl::FileWriterImpl(const boost::filesystem::path& path) {
    ofs.open(path.string(), std::ifstream::out | std::ifstream::binary | std::ifstream::trunc);
    fmtBuffer[0] = '\0';
}

FileWriterImpl::~FileWriterImpl() {
    ofs.close();
}

void FileWriterImpl::writeFmt(const char* fmt, ...) {
    va_list argPtr;

    va_start(argPtr, fmt);
    int size = vsnprintf(fmtBuffer, sizeof(fmtBuffer), fmt, argPtr);
    va_end(argPtr);

    ofs.write(fmtBuffer, size);
}

void FileWriterImpl::writeChar(char value) {
    ofs.put(value);
}

void FileWriterImpl::writeByte(uint8_t value) {
    ofs.put(value);
}

void FileWriterImpl::writeWord(uint16_t value) {
    ofs.put(value & 0xFF);
    ofs.put(value >> 8);
}

void FileWriterImpl::writeDword(uint32_t value) {
    writeWord((uint16_t)(value & (uint32_t)0xFFFF));
    writeWord((uint16_t)(value >> 0x10));
}

void FileWriterImpl::writeBlock(void* buffer, uintmax_t size) {
    ofs.write((char*)buffer, size);
}
