#ifndef PLATFORM_IMPL__STORAGE_IMPL_H__INCLUDED
#define PLATFORM_IMPL__STORAGE_IMPL_H__INCLUDED

#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>
#include "platform/storage.h"

class StorageImpl : public Storage {
public:

    StorageImpl(const std::string& applicationId, const std::string& executablePathStr);

    PathPtr path(const std::string& path);
    PathPtr appDataPath();
    PathPtr findExtras(const std::string& fileName);
    PathPtr findExtras(const std::string& directory, const std::string& fileName);

    uintmax_t readExtras(
        const char* directory,
        const std::string& fileName,
        uint8_t* buffer,
        uintmax_t size,
        uintmax_t offset = 0
    );

private:

    std::string applicationId;
    PathPtr appDataPathPtr;
    std::vector<PathPtr> extrasPaths;
};

class PathImpl : public Path {
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
    bool isFileExists();
    bool isDirectory();
    uintmax_t fileSize();
    void listEntries(std::vector<PathPtr>& into);

    DataReaderPtr dataReader();

    bool remove();
    bool createDirectory();
    DataWriterPtr dataWriter();

private:

    #ifdef _WIN32
        boost::filesystem::path platformPath;
    #endif

    boost::filesystem::path nativePath;
    bool isAbsolute;

    PathImpl(const std::string& path);

    #ifdef _WIN32
        PathImpl(const std::shared_ptr<PathImpl>& path) : platformPath(path->platformPath),
            nativePath(path->nativePath),
            isAbsolute(path->isAbsolute) {}

        PathImpl(
            const boost::filesystem::path& platformPath,
            const boost::filesystem::path& nativePath
        ) : platformPath(platformPath), nativePath(nativePath), isAbsolute(nativePath.is_absolute()) {}
    #else
        PathImpl(const std::shared_ptr<PathImpl>& path) : nativePath(path->nativePath), isAbsolute(path->isAbsolute) {}
        PathImpl(const boost::filesystem::path& nativePath) : nativePath(nativePath), isAbsolute(nativePath.is_absolute()) {}
    #endif

    friend class StorageImpl;
};

class DataReaderImpl : public DataReader {
public:

    ~DataReaderImpl();

    bool isEof();
    char readChar();
    uint8_t readByte();
    uint16_t readWord();
    uint32_t readDword();
    uintmax_t readBlock(void* buffer, uintmax_t size);
    std::string readLine();
    uintmax_t getPosition();
    void setPosition(uintmax_t position);

private:

    std::ifstream ifs;

    DataReaderImpl(const boost::filesystem::path& path);

    friend class PathImpl;
};

class DataWriterImpl : public DataWriter {
public:

    ~DataWriterImpl();

    void writeFmt(const char* fmt, ...);
    void writeChar(char value);
    void writeByte(uint8_t value);
    void writeWord(uint16_t value);
    void writeDword(uint32_t value);
    void writeBlock(void* buf, uintmax_t size);

private:

    std::ofstream ofs;
    char fmtBuffer[0x400];

    DataWriterImpl(const boost::filesystem::path& path);

    friend class PathImpl;
};

#endif
