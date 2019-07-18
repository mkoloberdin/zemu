// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifdef __APPLE__
    // Fix compilation error due to bug in the boost.process 1.69.x - https://github.com/boostorg/process/issues/55
    // This "fix" will broke timed functions (eg. wait_for / wait_until), but we don't use them, so it's ok for us.
    #include <signal.h>

    #ifdef sigemptyset
        #undef sigemptyset
    #endif

    #ifdef sigaddset
        #undef sigaddset
    #endif

    #ifdef sigtimedwait
        #undef sigtimedwait
    #endif

    int sigemptyset(sigset_t *);
    int sigaddset(sigset_t *, int);
    int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout);
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/process.hpp>
#include "storage_impl.h"

#ifdef _WIN32
    #ifndef _WIN32_IE
        #define _WIN32_IE 0x400
    #endif

    #include <fileapi.h>
    #include <shlobj.h>
#endif

namespace {
    bool shellExecute(std::string command, std::initializer_list<std::string> arguments, std::vector<std::string>* into) {
        boost::process::ipstream ips;

        if (boost::process::system(command, boost::process::args(arguments), boost::process::std_out > ips) != 0) {
            return false;
        }

        if (into == nullptr) {
            return true;
        }

        std::string line;
        into->clear();

        while (ips && std::getline(ips, line)) {
            into->push_back(line);
        }

        return true;
    }
}

//
// StorageImpl
//

StorageImpl::StorageImpl(const std::string& applicationId, const std::string& executablePathStr) : applicationId(applicationId) {
    extrasPaths.push_back(path("")); // This is not "empty" path, but path pointing to the current directory
    extrasPaths.push_back(path(executablePathStr)->parent());
    extrasPaths.push_back(appDataPath());

    #ifdef SHARE_PATH
        extrasPaths.push_back(path(SHARE_PATH));
    #endif

    boost::system::error_code ec;
    auto nativeTempDirPath = boost::filesystem::temp_directory_path(ec);

    if (ec) {
        tempDirPath = PathPtr(new EmptyPathImpl(this));
        return;
    }

    nativeTempDirPath /= boost::filesystem::unique_path(applicationId + "-%%%%-%%%%-%%%%-%%%%", ec);

    if (ec) {
        tempDirPath = PathPtr(new EmptyPathImpl(this));
        return;
    }

    tempDirPath = path(nativeTempDirPath.string());
    tempDirPath->createDirectories();
}

StorageImpl::~StorageImpl() {
    if (!tempDirPath->isEmpty()) {
        tempDirPath->removeAll();
    }
}

PathPtr StorageImpl::path(const std::string& path) {
    return PathPtr(createPath(path));
}

PathPtr StorageImpl::appDataPath() {
    if (!appDataPathInstance) {
        #ifdef _WIN32
            const TCHAR path[MAX_PATH];

            if (SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_APPDATA | CSIDL_FLAG_CREATE, nullptr, 0, path))) {
                appDataPathInstance.reset(createPath(path));
            } else {
                appDataPathInstance.reset(new EmptyPathImpl(this));
            }
        #else
            const char* configHomePath = std::getenv("XDG_CONFIG_HOME");

            if (configHomePath) {
                appDataPathInstance.reset(createPath(configHomePath));
            } else {
                const char* homePath = std::getenv("HOME");

                if (homePath) {
                    appDataPathInstance = path(homePath)->append(".config");
                } else {
                    appDataPathInstance.reset(new EmptyPathImpl(this));
                }
            }
        #endif

        if (!appDataPathInstance->isEmpty()) {
            appDataPathInstance = appDataPathInstance->append(applicationId);
            appDataPathInstance->createDirectories();
        }
    }

    return appDataPathInstance;
}

PathPtr StorageImpl::findExtras(const std::string& fileName) {
    for (const auto& extrasPath : extrasPaths) {
        auto path = extrasPath->append(fileName);

        if (path->isFile()) {
            return path;
        }
    }

    return PathPtr(new EmptyPathImpl(this));
}

PathPtr StorageImpl::findExtras(const std::string& directory, const std::string& fileName) {
    for (const auto& extrasPath : extrasPaths) {
        auto path = extrasPath->append(directory)->append(fileName);

        if (path->isFile()) {
            return path;
        }
    }

    return PathPtr(new EmptyPathImpl(this));
}

uintmax_t StorageImpl::readExtras(
    const char* directory,
    const std::string& fileName,
    uint8_t* buffer,
    uintmax_t maxSize,
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

    return reader->readBlock(buffer, maxSize);
}

Path* StorageImpl::createPath(const std::string& srcPathStr) {
    boost::filesystem::path nativePath(srcPathStr);
    boost::filesystem::path archivePath;
    boost::filesystem::path archiveInnerPath;
    PathPtr archivePluginPath = PathPtr(new EmptyPathImpl(this));

    for (auto it = nativePath.begin(); it != nativePath.end(); it++) {
        archivePath /= *it;
        archivePluginPath = detectArchivePlugin(archivePath);

        if (!archivePluginPath->isEmpty()) {
            for (it++; it != nativePath.end(); it++) {
                archiveInnerPath /= *it;
            }

            break;
        }
    }

    std::string pathStr = archivePath.string();

    #ifdef _WIN32
        // path -> host | platform
        std::string nativePathStr;
        std::string platformPathStr;

        if (pathStr.length() < 2) {
            // "" -> "" | ""
            // "x" -> "x" | "x"
            // "/" -> "/" | ""

            nativePathStr = pathStr;
            platformPathStr = ((pathStr == "/") ? "" : pathStr);
        } else if (pathStr[1] == ':') {
            // "c:x..." -> "/c/x..." | "c:/x..."
            // "c:/x..." -> "/c/x..." | "c:/x..."

            nativePathStr = pathStr;
            platformPathStr = pathStr;

            if (pathStr.length() > 2 && pathStr[2] != '/') {
                nativePathStr.insert(2, 1, '/');
                platformPathStr.insert(2, 1, '/');
            }

            nativePathStr[1] = nativePathStr[0];
            nativePathStr[0] = '/';
        } else if (pathStr[0] != '/') {
            // relative path, nothing to convert

            nativePathStr = pathStr;
            platformPathStr = pathStr;
        } else if (pathStr.length() == 2 || pathStr[2] == '/') {
            // "/c" -> "/c" | "c:"
            // "/c/..." -> "/c/..." | "c:/..."

            nativePathStr = pathStr;
            platformPathStr = pathStr;

            platformPathStr[0] = platformPathStr[1];
            platformPathStr[1] = ':';
        } else {
            // "/abc..." -> "/Z/abc..." | "Z:/abc" where "Z" is current drive

            boost::system::error_code ec;
            auto currentPath = boost::filesystem::current_path(ec);

            if (ec) {
                // an error occurred, so just ignore such path
            } else {
                platformPathStr = currentPath.string() + pathStr;
                nativePathStr = platformPathStr;

                nativePathStr[1] = nativePathStr[0];
                nativePathStr[0] = '/';
            }
        }

        boost::algorithm::replace_all(nativePathStr, "\\", "/");
        auto platformPath = boost::filesystem::path(platformPathStr);

        if (!archiveInnerPath.empty()) {
            return new ArchivePathImpl(
                this,
                archivePluginPath,
                boost::filesystem::path(nativePathStr),
                archiveInnerPath,
                std::shared_ptr<ArchiveEntries>(nullptr),
                platformPath
            );
        } else {
            return new FilePathImpl(this, boost::filesystem::path(nativePathStr), platformPath);
        }
    #else
        if (!archiveInnerPath.empty()) {
            return new ArchivePathImpl(
                this,
                archivePluginPath,
                archivePath,
                archiveInnerPath,
                std::shared_ptr<ArchiveEntries>(nullptr)
            );
        } else {
            return new FilePathImpl(this, nativePath);
        }
    #endif
}

PathPtr StorageImpl::detectArchivePlugin(const boost::filesystem::path& path) {
    boost::system::error_code ec;
    bool isFile = boost::filesystem::is_regular_file(path, ec);

    if (!isFile || ec) {
        return PathPtr(new EmptyPathImpl(this));
    }

    std::string extension = path.extension().string();

    if (extension.length() < 1) {
        return PathPtr(new EmptyPathImpl(this));
    }

    extension = extension.substr(1);

    #ifdef _WIN32
        PathPtr pluginPath = findExtras("arc", extension + ".bat");

        if (pluginPath->isEmpty()) {
            pluginPath = findExtras("arc", extension + ".exe");
        }
    #else
        PathPtr pluginPath = findExtras("arc", extension);
    #endif

    return pluginPath;
}

std::shared_ptr<ArchiveEntries> StorageImpl::listArchiveEntries(PathPtr pluginPath, boost::filesystem::path& archivePath) {
    std::vector<std::string> output;

    ArchiveEntries* entries = new ArchiveEntries();
    (*entries)[""] = {true, 0, std::set<std::string>()};

    if (!shellExecute(pluginPath->string(), {"l", archivePath.string()}, &output)) {
        return std::shared_ptr<ArchiveEntries>(entries);
    }

    for (auto& line : output) {
        size_t pos = line.find(' ');

        if (pos == std::string::npos) {
            continue;
        }

        std::string sizeStr = line.substr(0, pos);
        std::string innerPath = line.substr(pos + 1);

        if (sizeStr.empty() || innerPath.empty()) {
            continue;
        }

        boost::algorithm::replace_all(innerPath, "\\", "/");
        size_t prevPos = 0;
        std::string prevPath;

        for (;;) {
            size_t currentPos = innerPath.find('/', prevPos);
            std::string currentPath = innerPath.substr(0, currentPos);
            entries->at(prevPath).children.insert(currentPath);

            if (entries->find(currentPath) == entries->end()) {
                bool isDirectory = (currentPos != std::string::npos || sizeStr == "-");

                (*entries)[currentPath] = {
                    isDirectory,
                    (currentPos == std::string::npos && !isDirectory ? (uintmax_t)atol(sizeStr.c_str()) : 0),
                    std::set<std::string>()
                };
            }

            if (currentPos == std::string::npos) {
                break;
            }

            prevPos = currentPos + 1;
            prevPath = currentPath;
        }
    }

    return std::shared_ptr<ArchiveEntries>(entries);
}

//
// EmptyPathImpl
//

bool EmptyPathImpl::isWriteSupported() {
    return false;
}

bool EmptyPathImpl::isEmpty() {
    return true;
}

bool EmptyPathImpl::isRoot() {
    return false;
}

std::string EmptyPathImpl::string() {
    return "";
}

std::string EmptyPathImpl::fileName() {
    return "";
}

std::string EmptyPathImpl::extension() {
    return "";
}

PathPtr EmptyPathImpl::parent() { //-V524
    return PathPtr(new EmptyPathImpl(storage));
}

PathPtr EmptyPathImpl::concat(const std::string& value) { //-V524
    return storage->path(value);
}

PathPtr EmptyPathImpl::append(const std::string& value) { //-V524
    return storage->path(value);
}

PathPtr EmptyPathImpl::canonical() { //-V524
    return PathPtr(new EmptyPathImpl(storage));
}

bool EmptyPathImpl::isExists() {
    return false;
}

bool EmptyPathImpl::isFile() {
    return false;
}

bool EmptyPathImpl::isDirectory() {
    return false;
}

uintmax_t EmptyPathImpl::fileSize() {
    return 0;
}

void EmptyPathImpl::listEntries(std::vector<PathPtr>& into) {
    into.clear();
}

DataReaderPtr EmptyPathImpl::dataReader() {
    throw std::domain_error("Reading for empty path is not supported");
}

bool EmptyPathImpl::remove() {
    return false;
}

bool EmptyPathImpl::removeAll() {
    return false;
}

bool EmptyPathImpl::createDirectories() {
    return false;
}

DataWriterPtr EmptyPathImpl::dataWriter() {
    throw std::domain_error("Writing for empty path is not supported");
}

//
// FilePathImpl
//

bool FilePathImpl::isWriteSupported() {
    return true;
}

bool FilePathImpl::isEmpty() {
    return nativePath.empty();
}

bool FilePathImpl::isRoot() {
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

std::string FilePathImpl::string() {
    return nativePath.string();
}

std::string FilePathImpl::fileName() {
    return nativePath.filename().string();
}

std::string FilePathImpl::extension() {
    return nativePath.extension().string();
}

PathPtr FilePathImpl::parent() {
    #ifdef _WIN32
        auto parentNativePath = nativePath.parent_path();

        if (isAbsolute && parentNativePath.has_root_path() && parentNativePath.root_path() == parentNativePath) {
            return PathPtr(new FilePathImpl(storage, boost::filesystem::path(), parentNativePath));
        }

        return PathPtr(new FilePathImpl(storage, parentNativePath, platformPath.parent_path()));
    #else
        return PathPtr(new FilePathImpl(storage, nativePath.parent_path()));
    #endif
}

PathPtr FilePathImpl::concat(const std::string& value) {
    boost::filesystem::path newNativePath(nativePath);
    newNativePath += value;

    // We go through the storage->path() to support archives.
    return storage->path(newNativePath.string());
}

PathPtr FilePathImpl::append(const std::string& value) {
    boost::filesystem::path newNativePath(nativePath);
    newNativePath /= value;

    // We go through the storage->path() to support archives.
    return storage->path(newNativePath.string());
}

PathPtr FilePathImpl::canonical() {
    boost::system::error_code ec;

    #ifdef _WIN32
        auto canonicalPath = boost::filesystem::canonical(platformPath, ec);
        return storage->path(ec ? "" : canonicalPath.string());
    #else
        auto canonicalPath = boost::filesystem::canonical(nativePath, ec);
        return PathPtr(new FilePathImpl(storage, ec ? boost::filesystem::path() : canonicalPath));
    #endif
}

bool FilePathImpl::isExists() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::exists(platformPath, ec);
    #else
        bool result = boost::filesystem::exists(nativePath, ec);
    #endif

    return result && !ec;
}

bool FilePathImpl::isFile() {
    if (!archivePluginPath->isEmpty()) {
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

bool FilePathImpl::isDirectory() {
    if (!archivePluginPath->isEmpty()) {
        return true;
    }

    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::is_directory(platformPath, ec);
    #else
        bool result = boost::filesystem::is_directory(nativePath, ec);
    #endif

    return result && !ec;
}

uintmax_t FilePathImpl::fileSize() {
    boost::system::error_code ec;

    #ifdef _WIN32
        uintmax_t result = boost::filesystem::file_size(platformPath, ec);
    #else
        uintmax_t result = boost::filesystem::file_size(nativePath, ec);
    #endif

    return (ec ? 0 : result);
}

void FilePathImpl::listEntries(std::vector<PathPtr>& into) {
    into.clear();

    if (!archivePluginPath->isEmpty()) {
        into.push_back(append(".."));

        #ifdef _WIN32
            std::shared_ptr<ArchiveEntries> entries = storage->listArchiveEntries(archivePluginPath, platformPath);
        #else
            std::shared_ptr<ArchiveEntries> entries = storage->listArchiveEntries(archivePluginPath, nativePath);
        #endif

        for (auto& path : entries->at("").children) {
            #ifdef _WIN32
                into.push_back(PathPtr(new ArchivePathImpl(
                    storage,
                    archivePluginPath,
                    nativePath,
                    boost::filesystem::path(path),
                    entries,
                    platformPath
                )));
            #else
                into.push_back(PathPtr(new ArchivePathImpl(
                    storage,
                    archivePluginPath,
                    nativePath,
                    boost::filesystem::path(path),
                    entries
                )));
            #endif
        }

        return;
    }

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
                    into.push_back(storage->path(drivePath));
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
        into.push_back(storage->path(entry.path().string()));
    }
}

DataReaderPtr FilePathImpl::dataReader() {
    #ifdef _WIN32
        return DataReaderPtr(new FileDataReaderImpl(platformPath));
    #else
        return DataReaderPtr(new FileDataReaderImpl(nativePath));
    #endif
}

bool FilePathImpl::remove() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::remove(platformPath, ec);
    #else
        bool result = boost::filesystem::remove(nativePath, ec);
    #endif

    return result && !ec;
}

bool FilePathImpl::removeAll() {
    boost::system::error_code ec;

    #ifdef _WIN32
        boost::filesystem::remove_all(platformPath, ec);
    #else
        boost::filesystem::remove_all(nativePath, ec);
    #endif

    return !ec && !isExists();
}

bool FilePathImpl::createDirectories() {
    boost::system::error_code ec;

    #ifdef _WIN32
        bool result = boost::filesystem::create_directories(platformPath, ec);
    #else
        bool result = boost::filesystem::create_directories(nativePath, ec);
    #endif

    return result && !ec;
}

DataWriterPtr FilePathImpl::dataWriter() {
    #ifdef _WIN32
        return DataWriterPtr(new DataWriterImpl(platformPath));
    #else
        return DataWriterPtr(new DataWriterImpl(nativePath));
    #endif
}

//
// ArchivePathImpl
//

bool ArchivePathImpl::isWriteSupported() {
    return false;
}

bool ArchivePathImpl::isEmpty() {
    return false;
}

bool ArchivePathImpl::isRoot() {
    return false;
}

std::string ArchivePathImpl::string() {
    boost::filesystem::path fullPath(archivePath);
    fullPath /= innerPath;

    return fullPath.string();
}

std::string ArchivePathImpl::fileName() {
    return innerPath.filename().string();
}

std::string ArchivePathImpl::extension() {
    return innerPath.extension().string();
}

PathPtr ArchivePathImpl::parent() {
    auto parentInnerPath = innerPath.parent_path();

    if (parentInnerPath.empty()) {
        #ifdef _WIN32
            return PathPtr(new FilePathImpl(storage, archivePath, platformArchivePath));
        #else
            return PathPtr(new FilePathImpl(storage, archivePath));
        #endif
    }

    #ifdef _WIN32
        return PathPtr(new ArchivePathImpl(
            storage,
            pluginPath,
            archivePath,
            parentInnerPath,
            entries(),
            platformArchivePath
        ));
    #else
        return PathPtr(new ArchivePathImpl(storage, pluginPath, archivePath, parentInnerPath, entries()));
    #endif
}

PathPtr ArchivePathImpl::concat(const std::string& value) {
    boost::filesystem::path newInnerPath(innerPath);
    newInnerPath += value;

    #ifdef _WIN32
        return PathPtr(new ArchivePathImpl(
            storage,
            pluginPath,
            archivePath,
            newInnerPath,
            entries(),
            platformArchivePath
        ));
    #else
        return PathPtr(new ArchivePathImpl(storage, pluginPath, archivePath, newInnerPath, entries()));
    #endif
}

PathPtr ArchivePathImpl::append(const std::string& value) {
    boost::filesystem::path newInnerPath(innerPath);
    newInnerPath /= value;

    #ifdef _WIN32
        return PathPtr(new ArchivePathImpl(
            storage,
            pluginPath,
            archivePath,
            newInnerPath,
            entries(),
            platformArchivePath
        ));
    #else
        return PathPtr(new ArchivePathImpl(storage, pluginPath, archivePath, newInnerPath, entries()));
    #endif
}

PathPtr ArchivePathImpl::canonical() {
    return PathPtr(new ArchivePathImpl(this));
}

bool ArchivePathImpl::isExists() {
    if (isDotDot) {
        return true;
    }

    return entries()->find(innerPath.string()) != entries()->end();
}

bool ArchivePathImpl::isFile() {
    if (isDotDot) {
        return false;
    }

    auto it = entries()->find(innerPath.string());
    return (it == entries()->end() ? false : !it->second.isDirectory);
}

bool ArchivePathImpl::isDirectory() {
    if (isDotDot) {
        return true;
    }

    auto it = entries()->find(innerPath.string());
    return (it == entries()->end() ? false : it->second.isDirectory);
}

uintmax_t ArchivePathImpl::fileSize() {
    if (isDotDot) {
        return 0;
    }

    auto it = entries()->find(innerPath.string());
    return (it == entries()->end() ? 0 : it->second.fileSize);
}

void ArchivePathImpl::listEntries(std::vector<PathPtr>& into) {
    if (isDotDot) {
        parent()->listEntries(into);
        return;
    }

    into.clear();
    into.push_back(append(".."));

    auto it = entries()->find(innerPath.string());

    if (it == entries()->end()) {
        return;
    }

    for (auto& path : it->second.children) {
        #ifdef _WIN32
            into.push_back(PathPtr(new ArchivePathImpl(
                storage,
                pluginPath,
                archivePath,
                boost::filesystem::path(path),
                entries(),
                platformArchivePath
            )));
        #else
            into.push_back(PathPtr(new ArchivePathImpl(
                storage,
                pluginPath,
                archivePath,
                boost::filesystem::path(path),
                entries()
            )));
        #endif
    }
}

DataReaderPtr ArchivePathImpl::dataReader() {
    #ifdef _WIN32
        return DataReaderPtr(new ArchiveDataReaderImpl(storage->tempDirPath, pluginPath, platformArchivePath, innerPath));
    #else
        return DataReaderPtr(new ArchiveDataReaderImpl(storage->tempDirPath, pluginPath, archivePath, innerPath));
    #endif
}

bool ArchivePathImpl::remove() {
    return false;
}

bool ArchivePathImpl::removeAll() {
    return false;
}

bool ArchivePathImpl::createDirectories() {
    return false;
}

DataWriterPtr ArchivePathImpl::dataWriter() {
    throw std::domain_error("Writing into archives is not supported");
}

std::shared_ptr<ArchiveEntries>& ArchivePathImpl::entries() {
    if (!entriesInstance) {
        entriesInstance = storage->listArchiveEntries(pluginPath, archivePath);
    }

    return entriesInstance;
}

//
// FileDataReaderImpl
//

FileDataReaderImpl::FileDataReaderImpl(const boost::filesystem::path& path) {
    ifs.open(path.string(), std::ifstream::in | std::ifstream::binary);
}

FileDataReaderImpl::~FileDataReaderImpl() {
    ifs.close();
}

bool FileDataReaderImpl::isEof() {
    return ifs.eof();
}

char FileDataReaderImpl::readChar() {
    return ifs.get();
}

uint8_t FileDataReaderImpl::readByte() {
    return ifs.get();
}

uint16_t FileDataReaderImpl::readWord() {
    int l = ifs.get();
    int h = ifs.get();
    return (uint16_t)(l + 0x100 * h);
}

uint32_t FileDataReaderImpl::readDword() {
    uint32_t l = readWord();
    uint32_t h = readWord();
    return (uint32_t)(l + (uint32_t)0x10000 * h);
}

uintmax_t FileDataReaderImpl::readBlock(void* buffer, uintmax_t maxSize) {
    ifs.read((char*)buffer, maxSize);
    return ifs.gcount();
}

std::string FileDataReaderImpl::readLine() {
    std::string result;
    std::getline(ifs, result);
    return result;
}

uintmax_t FileDataReaderImpl::getPosition() {
    return ifs.tellg();
}

void FileDataReaderImpl::setPosition(uintmax_t position) {
    ifs.seekg(position);
}

//
// ArchiveDataReaderImpl
//

ArchiveDataReaderImpl::ArchiveDataReaderImpl(
    const PathPtr& tempDirPath,
    const PathPtr& pluginPath,
    const boost::filesystem::path& archivePath,
    const boost::filesystem::path& innerPath
) {
    boost::system::error_code ec;

    unpackedPath = tempDirPath->string();
    unpackedPath /= boost::filesystem::unique_path(std::string("%%%%-%%%%-%%%%-%%%%") + innerPath.extension().string(), ec);

    if (ec) {
        throw StorageException((boost::format("Failed obtain unique path for %s in \"%s\"")
            % innerPath
            % tempDirPath->string()
        ).str());
    }

    if (!shellExecute(pluginPath->string(), {"x", archivePath.string(), innerPath.string(), unpackedPath.string()}, nullptr)) {
        throw StorageException((boost::format("Failed to unpack %s from %s to %s using plugin \"%s\"")
            % innerPath
            % archivePath
            % unpackedPath
            % pluginPath->string()
        ).str());
    }

    bool isFile = boost::filesystem::is_regular_file(unpackedPath, ec);

    if (!isFile || ec) {
        throw StorageException((boost::format("Failed to unpack %s from %s to %s using plugin \"%s\"")
            % innerPath
            % archivePath
            % unpackedPath
            % pluginPath->string()
        ).str());
    }

    unpackedDataReader.reset(new FileDataReaderImpl(unpackedPath));
}

ArchiveDataReaderImpl::~ArchiveDataReaderImpl() {
    unpackedDataReader.reset();
    boost::filesystem::remove(unpackedPath);
}

bool ArchiveDataReaderImpl::isEof() {
    return unpackedDataReader->isEof();
}

char ArchiveDataReaderImpl::readChar() {
    return unpackedDataReader->readChar();
}

uint8_t ArchiveDataReaderImpl::readByte() {
    return unpackedDataReader->readByte();
}

uint16_t ArchiveDataReaderImpl::readWord() {
    return unpackedDataReader->readWord();
}

uint32_t ArchiveDataReaderImpl::readDword() {
    return unpackedDataReader->readDword();
}

uintmax_t ArchiveDataReaderImpl::readBlock(void* buffer, uintmax_t maxSize) {
    return unpackedDataReader->readBlock(buffer, maxSize);
}

std::string ArchiveDataReaderImpl::readLine() {
    return unpackedDataReader->readLine();
}

uintmax_t ArchiveDataReaderImpl::getPosition() {
    return unpackedDataReader->getPosition();
}

void ArchiveDataReaderImpl::setPosition(uintmax_t position) {
    unpackedDataReader->setPosition(position);
}

//
// DataWriterImpl
//

DataWriterImpl::DataWriterImpl(const boost::filesystem::path& path) {
    fmtBuffer[0] = '\0';
    ofs.open(path.string(), std::ifstream::out | std::ifstream::binary | std::ifstream::trunc);

    if (ofs.fail()) {
        throw StorageException((boost::format("Failed to write into \"%s\"") % path).str());
    }
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

bool DataWriterImpl::writeBlock(void* buffer, uintmax_t size) {
    ofs.write((char*)buffer, size);
    return !ofs.fail();
}

uintmax_t DataWriterImpl::getPosition() {
    return ofs.tellp();
}

void DataWriterImpl::setPosition(uintmax_t position) {
    ofs.seekp(position);
}
