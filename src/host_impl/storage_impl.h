#ifndef HOST_IMPL__STORAGE_IMPL_H__INCLUDED
#define HOST_IMPL__STORAGE_IMPL_H__INCLUDED

#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <boost/filesystem.hpp>
#include "host/storage.h"

class FilePathImpl;
class ArchivePathImpl;
class ArchiveDataReaderImpl;

struct ArchiveEntry {
    bool isDirectory;
    uintmax_t fileSize;
    std::set<std::string> children;
};

typedef std::map<std::string, ArchiveEntry> ArchiveEntries;

class StorageImpl : public Storage {
public:

    StorageImpl(const std::string& applicationId, const std::string& executablePathStr);
    ~StorageImpl();

    PathPtr path(const std::string& path);
    PathPtr appDataPath();
    PathPtr findExtras(const std::string& fileName);
    PathPtr findExtras(const std::string& directory, const std::string& fileName);

    uintmax_t readExtras(
        const char* directory,
        const std::string& fileName,
        uint8_t* buffer,
        uintmax_t maxSize,
        uintmax_t offset = 0
    );

private:

    std::string applicationId;
    PathPtr tempDirPath;
    PathPtr appDataPathInstance;
    std::vector<PathPtr> extrasPaths;

    Path* createPath(const std::string& path);
    PathPtr detectArchivePlugin(const boost::filesystem::path& path);
    std::shared_ptr<ArchiveEntries> listArchiveEntries(PathPtr pluginPath, boost::filesystem::path& archivePath);

    friend class FilePathImpl;
    friend class ArchivePathImpl;
};

class EmptyPathImpl : public Path {
public:

    bool isWriteSupported();

    bool isEmpty();
    bool isRoot();

    std::string string();
    std::string fileName();
    std::string extension();

    PathPtr parent();
    PathPtr concat(const std::string& value);
    PathPtr append(const std::string& value);
    PathPtr canonical();

    bool isExists();
    bool isFile();
    bool isDirectory();
    uintmax_t fileSize();
    void listEntries(std::vector<PathPtr>& into);

    DataReaderPtr dataReader();

    bool remove();
    bool removeAll();
    bool createDirectories();
    DataWriterPtr dataWriter();

private:

    StorageImpl* storage;

    EmptyPathImpl(StorageImpl* storage) : storage(storage) {}

    friend class StorageImpl;
};

class FilePathImpl : public Path {
public:

    bool isWriteSupported();

    bool isEmpty();
    bool isRoot();

    std::string string();
    std::string fileName();
    std::string extension();

    PathPtr parent();
    PathPtr concat(const std::string& value);
    PathPtr append(const std::string& value);
    PathPtr canonical();

    bool isExists();
    bool isFile();
    bool isDirectory();
    uintmax_t fileSize();
    void listEntries(std::vector<PathPtr>& into);

    DataReaderPtr dataReader();

    bool remove();
    bool removeAll();
    bool createDirectories();
    DataWriterPtr dataWriter();

private:

    StorageImpl* storage;
    boost::filesystem::path nativePath;
    bool isAbsolute;
    PathPtr archivePluginPath;

    #ifdef _WIN32
        boost::filesystem::path platformPath;

        FilePathImpl(
            StorageImpl* storage,
            const boost::filesystem::path& nativePath,
            const boost::filesystem::path& platformPath
        ) : storage(storage),
            nativePath(nativePath),
            isAbsolute(nativePath.is_absolute()),
            archivePluginPath(storage->detectArchivePlugin(platformPath)),
            platformPath(platformPath) {}
    #else
        FilePathImpl(StorageImpl* storage, const boost::filesystem::path& nativePath) : storage(storage),
            nativePath(nativePath),
            isAbsolute(nativePath.is_absolute()),
            archivePluginPath(storage->detectArchivePlugin(nativePath)) {}
    #endif

    friend class StorageImpl;
    friend class ArchivePathImpl;
};

class ArchivePathImpl : public Path {
public:

    bool isWriteSupported();

    bool isEmpty();
    bool isRoot();

    std::string string();
    std::string fileName();
    std::string extension();

    PathPtr parent();
    PathPtr concat(const std::string& value);
    PathPtr append(const std::string& value);
    PathPtr canonical();

    bool isExists();
    bool isFile();
    bool isDirectory();
    uintmax_t fileSize();
    void listEntries(std::vector<PathPtr>& into);

    DataReaderPtr dataReader();

    bool remove();
    bool removeAll();
    bool createDirectories();
    DataWriterPtr dataWriter();

private:

    StorageImpl* storage;
    PathPtr pluginPath;
    boost::filesystem::path archivePath;
    boost::filesystem::path innerPath;
    std::shared_ptr<ArchiveEntries> entriesInstance;
    bool isDotDot;

    #ifdef _WIN32
        boost::filesystem::path platformArchivePath;

        ArchivePathImpl(ArchivePathImpl* path) : storage(path->storage),
            pluginPath(path->pluginPath),
            archivePath(path->archivePath),
            innerPath(path->innerPath),
            entriesInstance(path->entriesInstance),
            isDotDot(path->isDotDot),
            platformArchivePath(path->platformArchivePath) {}

        ArchivePathImpl(
            StorageImpl* storage,
            const PathPtr& pluginPath,
            const boost::filesystem::path& archivePath,
            const boost::filesystem::path& innerPath,
            const std::shared_ptr<ArchiveEntries>& entriesInstance,
            const boost::filesystem::path& platformArchivePath
        ) : storage(storage),
            pluginPath(pluginPath),
            archivePath(archivePath),
            innerPath(innerPath),
            entriesInstance(entriesInstance),
            isDotDot(innerPath.filename() == ".."),
            platformArchivePath(platformArchivePath) {}
    #else
        ArchivePathImpl(ArchivePathImpl* path) : storage(path->storage),
            pluginPath(path->pluginPath),
            archivePath(path->archivePath),
            innerPath(path->innerPath),
            entriesInstance(path->entriesInstance),
            isDotDot(path->isDotDot) {}

        ArchivePathImpl(
            StorageImpl* storage,
            const PathPtr& pluginPath,
            const boost::filesystem::path& archivePath,
            const boost::filesystem::path& innerPath,
            const std::shared_ptr<ArchiveEntries>& entriesInstance
        ) : storage(storage),
            pluginPath(pluginPath),
            archivePath(archivePath),
            innerPath(innerPath),
            entriesInstance(entriesInstance),
            isDotDot(innerPath.filename() == "..") {}
    #endif

    std::shared_ptr<ArchiveEntries>& entries();

    friend class FilePathImpl;
    friend class StorageImpl;
};

class FileDataReaderImpl : public DataReader {
public:

    ~FileDataReaderImpl();

    bool isEof();
    char readChar();
    uint8_t readByte();
    uint16_t readWord();
    uint32_t readDword();
    uintmax_t readBlock(void* buffer, uintmax_t maxSize);
    std::string readLine();
    uintmax_t getPosition();
    void setPosition(uintmax_t position);

private:

    std::ifstream ifs;

    FileDataReaderImpl(const boost::filesystem::path& path);

    friend class FilePathImpl;
    friend class ArchiveDataReaderImpl;
};

class ArchiveDataReaderImpl : public DataReader {
public:
    ~ArchiveDataReaderImpl();

    bool isEof();
    char readChar();
    uint8_t readByte();
    uint16_t readWord();
    uint32_t readDword();
    uintmax_t readBlock(void* buffer, uintmax_t maxSize);
    std::string readLine();
    uintmax_t getPosition();
    void setPosition(uintmax_t position);

private:

    boost::filesystem::path unpackedPath;
    DataReaderPtr unpackedDataReader;

    ArchiveDataReaderImpl(
        const PathPtr& tempDirPath,
        const PathPtr& pluginPath,
        const boost::filesystem::path& archivePath,
        const boost::filesystem::path& innerPath
    );

    friend class ArchivePathImpl;
};

class DataWriterImpl : public DataWriter {
public:

    ~DataWriterImpl();

    void writeFmt(const char* fmt, ...);
    void writeChar(char value);
    void writeByte(uint8_t value);
    void writeWord(uint16_t value);
    void writeDword(uint32_t value);
    bool writeBlock(void* buf, uintmax_t size);
    uintmax_t getPosition();
    void setPosition(uintmax_t position);

private:

    std::ofstream ofs;
    char fmtBuffer[0x400];

    DataWriterImpl(const boost::filesystem::path& path);

    friend class FilePathImpl;
};

#endif
