// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <stdarg.h>
#include <boost/algorithm/string.hpp>
#include "storage_impl.h"

#ifdef _WIN32
    #ifndef _WIN32_IE
        #define _WIN32_IE 0x400
    #endif

    #include <fileapi.h>
    #include <shlobj.h>
#endif

//
// StorageImpl
//

StorageImpl::StorageImpl(const std::string& applicationId, const std::string& executablePathStr) : applicationId(applicationId) {
    extrasPaths.push_back(path(""));
    extrasPaths.push_back(path(executablePathStr)->parent());
    extrasPaths.push_back(appDataPath());

    #ifdef SHARE_PATH
        extrasPaths.push_back(path(SHARE_PATH));
    #endif
}

PathPtr StorageImpl::path(const std::string& path) {
    return PathPtr(new PathImpl(path));
}

PathPtr StorageImpl::appDataPath() {
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

PathPtr StorageImpl::findExtras(const std::string& fileName) {
    for (const auto& extrasPath : extrasPaths) {
        auto path = extrasPath->append(fileName);

        if (path->isFileExists()) {
            return path;
        }
    }

    return path("");
}

PathPtr StorageImpl::findExtras(const std::string& directory, const std::string& fileName) {
    for (const auto& extrasPath : extrasPaths) {
        auto path = extrasPath->append(directory)->append(fileName);

        if (path->isFileExists()) {
            return path;
        }
    }

    return path("");
}

uintmax_t StorageImpl::readExtras(
    const char* directory,
    const std::string& fileName,
    uint8_t* buffer,
    uintmax_t size,
    uintmax_t offset
) {
    auto path = findExtras(directory, fileName);

    if (path->isEmpty()) {
        return 0;
    }

    auto reader = path->dataReader();

    if (offset) {
        reader->setPosition(offset);
    }

    return reader->readBlock(buffer, size);
}

//
// PathImpl
//

PathImpl::PathImpl(const std::string& path) {
    #ifdef _WIN32
        // path -> host | platform
        std::string nativePathStr;
        std::string platformPathStr;

        if (path.length() < 2) {
            // "" -> "" | ""
            // "x" -> "x" | "x"
            // "/" -> "/" | ""

            nativePathStr = path;
            platformPathStr = ((path == "/") ? "" : path);
        } else if (path[1] == ':') {
            // "c:x..." -> "/c/x..." | "c:/x..."
            // "c:/x..." -> "/c/x..." | "c:/x..."

            nativePathStr = path;
            platformPathStr = path;

            if (path.length() > 2 && path[2] != '/') {
                nativePathStr.insert(2, 1, '/');
                platformPathStr.insert(2, 1, '/');
            }

            nativePathStr[1] = nativePathStr[0];
            nativePathStr[0] = '/';
        } else if (path[0] != '/') {
            // relative path, nothing to convert

            nativePathStr = path;
            platformPathStr = path;
        } else if (path.length() == 2 || path[2] == '/') {
            // "/c" -> "/c" | "c:"
            // "/c/..." -> "/c/..." | "c:/..."

            nativePathStr = path;
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
                nativePathStr = platformPathStr;

                nativePathStr[1] = nativePathStr[0];
                nativePathStr[0] = '/';
            }
        }

        boost::algorithm::replace_all(nativePathStr, "\\", "/");

        nativePath = nativePathStr;
        platformPath = platformPathStr;
    #else
        nativePath = path;
    #endif

    isAbsolute = nativePath.is_absolute();
}

bool PathImpl::isWriteSupported() {
    return true;
}

bool PathImpl::isEmpty() {
    return nativePath.empty();
}

bool PathImpl::isRoot() {
    boost::filesystem::path path;

    if (!isAbsolute) {
        boost::system::error_code ec;
        auto currentPath = boost::filesystem::current_path(ec);

        if (ec) {
            return false;
        }

        path = boost::filesystem::absolute(nativePath, currentPath);
    } else {
        path = nativePath;
    }

    // boost::filesystem::equivalent() is not used on purpose:
    // - it can fail;
    // - we just don't need it here, because our paths are carefuly crafted.

    return path.has_root_path() && path.root_path() == path;
}

std::string PathImpl::string() {
    return nativePath.string();
}

std::string PathImpl::fileName() {
    return nativePath.filename().string();
}

std::string PathImpl::extension() {
    return nativePath.extension().string();
}

PathPtr PathImpl::parent() {
    #ifdef _WIN32
        auto parentNativePath = nativePath.parent_path();

        if (isAbsolute && parentNativePath.has_root_path() && parentNativePath.root_path() == parentNativePath) {
            return PathPtr(new PathImpl(boost::filesystem::path(), parentNativePath));
        }

        return PathPtr(new PathImpl(platformPath.parent_path(), parentNativePath));
    #else
        return PathPtr(new PathImpl(nativePath.parent_path()));
    #endif
}

PathPtr PathImpl::concat(const std::string& value) {
    #ifdef _WIN32
        boost::filesystem::path newPlatformPath(platformPath);
        newPlatformPath += value;
    #endif

    boost::filesystem::path newNativePath(nativePath);
    newNativePath += value;

    #ifdef _WIN32
        return PathPtr(new PathImpl(newPlatformPath, newNativePath));
    #else
        return PathPtr(new PathImpl(newNativePath));
    #endif
}

PathPtr PathImpl::append(const std::string& value) {
    #ifdef _WIN32
        boost::filesystem::path newPlatformPath(platformPath);
        newPlatformPath /= value;
    #endif

    boost::filesystem::path newNativePath(nativePath);
    newNativePath /= value;

    #ifdef _WIN32
        return PathPtr(new PathImpl(newPlatformPath, newNativePath));
    #else
        return PathPtr(new PathImpl(newNativePath));
    #endif
}

PathPtr PathImpl::canonical() {
    boost::system::error_code ec;

    #ifdef _WIN32
        auto canonicalPath = boost::filesystem::canonical(platformPath, ec);
        return PathPtr(new PathImpl(ec ? "" : canonicalPath.string()));
    #else
        auto canonicalPath = boost::filesystem::canonical(nativePath, ec);
        return PathPtr(ec ? new PathImpl(std::string("")) : new PathImpl(canonicalPath));
    #endif
}

bool PathImpl::isExists() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::exists(platformPath, ec);
    #else
        bool result = boost::filesystem::exists(nativePath, ec);
    #endif

    return result && !ec;
}

bool PathImpl::isFileExists() {
    if (!isExists()) {
        return false;
    }

    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::is_regular_file(platformPath, ec);
    #else
        bool result = boost::filesystem::is_regular_file(nativePath, ec);
    #endif

    return result && !ec;
}

bool PathImpl::isDirectory() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::is_directory(platformPath, ec);
    #else
        bool result = boost::filesystem::is_directory(nativePath, ec);
    #endif

    return result && !ec;
}

uintmax_t PathImpl::fileSize() {
    boost::system::error_code ec;

    #ifdef _WIN32
        uintmax_t result = boost::filesystem::file_size(platformPath, ec);
    #else
        uintmax_t result = boost::filesystem::file_size(nativePath, ec);
    #endif

    return (ec ? 0 : result);
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

    if (!isExists()) {
        return;
    }

    boost::system::error_code ec;

    #ifdef _WIN32
        auto iterator = boost::filesystem::directory_iterator(platformPath);
    #else
        auto iterator = boost::filesystem::directory_iterator(nativePath);
    #endif

    if (ec) {
        return;
    }

    for (const auto& entry : iterator) {
        into.push_back(PathPtr(new PathImpl(entry.path())));
    }
}

DataReaderPtr PathImpl::dataReader() {
    #ifdef _WIN32
        return DataReaderPtr(new DataReaderImpl(platformPath));
    #else
        return DataReaderPtr(new DataReaderImpl(nativePath));
    #endif
}

bool PathImpl::remove() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::remove(platformPath, ec);
    #else
        bool result = boost::filesystem::remove(nativePath, ec);
    #endif

    return result && !ec;
}

bool PathImpl::createDirectory() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::create_directory(platformPath, ec);
    #else
        bool result = boost::filesystem::create_directory(nativePath, ec);
    #endif

    return result && !ec;
}

DataWriterPtr PathImpl::dataWriter() {
    #ifdef _WIN32
        return DataWriterPtr(new DataWriterImpl(platformPath));
    #else
        return DataWriterPtr(new DataWriterImpl(nativePath));
    #endif
}

//
// DataReaderImpl
//

DataReaderImpl::DataReaderImpl(const boost::filesystem::path& path) {
    ifs.open(path.string(), std::ifstream::in | std::ifstream::binary);
}

DataReaderImpl::~DataReaderImpl() {
    ifs.close();
}

bool DataReaderImpl::isEof() {
    return ifs.eof();
}

char DataReaderImpl::readChar() {
    return ifs.get();
}

uint8_t DataReaderImpl::readByte() {
    return ifs.get();
}

uint16_t DataReaderImpl::readWord() {
    int l = ifs.get();
    int h = ifs.get();
    return (uint16_t)(l + 0x100 * h);
}

uint32_t DataReaderImpl::readDword() {
    uint32_t l = readWord();
    uint32_t h = readWord();
    return (uint32_t)(l + (uint32_t)0x10000 * h);
}

uintmax_t DataReaderImpl::readBlock(void* buffer, uintmax_t size) {
    ifs.read((char*)buffer, size);
    return ifs.gcount();
}

std::string DataReaderImpl::readLine() {
    std::string result;
    std::getline(ifs, result);
    return result;
}

uintmax_t DataReaderImpl::getPosition() {
    return ifs.tellg();
}

void DataReaderImpl::setPosition(uintmax_t position) {
    ifs.seekg(position);
}

//
// DataWriterImpl
//

DataWriterImpl::DataWriterImpl(const boost::filesystem::path& path) {
    ofs.open(path.string(), std::ifstream::out | std::ifstream::binary | std::ifstream::trunc);
    fmtBuffer[0] = '\0';
}

DataWriterImpl::~DataWriterImpl() {
    ofs.close();
}

void DataWriterImpl::writeFmt(const char* fmt, ...) {
    va_list argPtr;

    va_start(argPtr, fmt);
    int size = vsnprintf(fmtBuffer, sizeof(fmtBuffer), fmt, argPtr);
    va_end(argPtr);

    ofs.write(fmtBuffer, size);
}

void DataWriterImpl::writeChar(char value) {
    ofs.put(value);
}

void DataWriterImpl::writeByte(uint8_t value) {
    ofs.put(value);
}

void DataWriterImpl::writeWord(uint16_t value) {
    ofs.put(value & 0xFF);
    ofs.put(value >> 8);
}

void DataWriterImpl::writeDword(uint32_t value) {
    writeWord((uint16_t)(value & (uint32_t)0xFFFF));
    writeWord((uint16_t)(value >> 0x10));
}

void DataWriterImpl::writeBlock(void* buffer, uintmax_t size) {
    ofs.write((char*)buffer, size);
}
