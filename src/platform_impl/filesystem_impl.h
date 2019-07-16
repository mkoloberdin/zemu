#ifndef PLATFORM_IMPL__FILESYSTEM_IMPL_H__INCLUDED
#define PLATFORM_IMPL__FILESYSTEM_IMPL_H__INCLUDED

#include <fstream>
#include <boost/filesystem.hpp>
#include "platform/filesystem.h"

class FileSystemImpl : public FileSystem {
public:

    FileSystemImpl(const std::string& applicationId) : applicationId(applicationId) {}

    PathPtr path(const std::string& path);
    PathPtr appDataPath();

private:

    std::string applicationId;
    PathPtr appDataPathPtr;
};

class PathImpl : public Path {
public:

    bool isEmpty();
    bool isRoot();
    std::string string();
    std::string fileName();
    std::string extension();
    PathPtr parent();
    PathPtr concat(const std::string& value);
    PathPtr append(const std::string& value);
    PathPtr canonical();
    bool exists();
    bool fileExists();
    bool isDirectory();
    uintmax_t fileSize();
    bool remove();
    bool createDirectory();
    FileReaderPtr fileReader();
    FileWriterPtr fileWriter();
    void listEntries(std::vector<PathPtr>& into);

private:

    #ifdef _WIN32
        boost::filesystem::path platformPath;
    #endif

    boost::filesystem::path hostPath;
    bool isAbsolute;

    PathImpl(const std::string& path);

    #ifdef _WIN32
        PathImpl(const std::shared_ptr<PathImpl>& path) : platformPath(path->platformPath),
            hostPath(path->hostPath),
            isAbsolute(path->isAbsolute) {}

        PathImpl(
            const boost::filesystem::path& platformPath,
            const boost::filesystem::path& hostPath
        ) : platformPath(platformPath), hostPath(hostPath), isAbsolute(hostPath.is_absolute()) {}
    #else
        PathImpl(const std::shared_ptr<PathImpl>& path) : hostPath(path->hostPath), isAbsolute(path->isAbsolute) {}
        PathImpl(const boost::filesystem::path& hostPath) : hostPath(hostPath), isAbsolute(hostPath.is_absolute()) {}
    #endif

    friend class FileSystemImpl;
};

class FileReaderImpl : public FileReader {
public:

    ~FileReaderImpl();

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

    FileReaderImpl(const boost::filesystem::path& path);
    friend class PathImpl;
};

class FileWriterImpl : public FileWriter {
public:

    ~FileWriterImpl();

    void writeFmt(const char* fmt, ...);
    void writeChar(char value);
    void writeByte(uint8_t value);
    void writeWord(uint16_t value);
    void writeDword(uint32_t value);
    void writeBlock(void* buf, uintmax_t size);

private:

    std::ofstream ofs;
    char fmtBuffer[0x400];

    FileWriterImpl(const boost::filesystem::path& path);
    friend class PathImpl;
};

#endif
