#ifndef PLATFORM__FILESYSTEM_H__INCLUDED
#define PLATFORM__FILESYSTEM_H__INCLUDED

#include <memory>
#include <string>
#include <cstdint>

class FileSystem;
class Path;
class FileReader;
class FileWriter;

typedef std::shared_ptr<FileSystem> FileSystemPtr;
typedef std::shared_ptr<Path> PathPtr;
typedef std::unique_ptr<FileReader> FileReaderPtr;
typedef std::unique_ptr<FileWriter> FileWriterPtr;

class FileSystem {
    public:

    FileSystem() {}
    virtual ~FileSystem() {}

    virtual PathPtr path(const std::string& path) = 0;
    virtual PathPtr appDataPath() = 0;

    private:

    FileSystem(const FileSystem&);
    FileSystem& operator=(const FileSystem&);
};

class Path {
    public:

    Path() {}
    virtual ~Path() {}

    virtual bool isEmpty() = 0;
    virtual bool isRoot() = 0;
    virtual std::string string() = 0;
    virtual std::string fileName() = 0;
    virtual std::string extension() = 0;
    virtual PathPtr parent() = 0;
    virtual PathPtr concat(const std::string& value) = 0;
    virtual PathPtr append(const std::string& path) = 0;
    virtual PathPtr canonical() = 0;
    virtual bool exists() = 0;
    virtual bool fileExists() = 0;
    virtual bool isDirectory() = 0;
    virtual uintmax_t fileSize() = 0;
    virtual bool remove() = 0;
    virtual bool createDirectory() = 0;
    virtual FileReaderPtr fileReader() = 0;
    virtual FileWriterPtr fileWriter() = 0;
    virtual void listEntries(std::vector<PathPtr>& into) = 0;

    std::string extensionLc();

    private:

    Path(const Path&);
    Path& operator=(const Path&);
};

class FileReader {
    public:

    FileReader() {}
    virtual ~FileReader() {}

    virtual bool isEof() = 0;
    virtual char readChar() = 0;
    virtual uint8_t readByte() = 0;
    virtual uint16_t readWord() = 0;
    virtual uint32_t readDword() = 0;
    virtual uintmax_t readBlock(void* buffer, uintmax_t size) = 0;
    virtual std::string readLine() = 0;
    virtual uintmax_t getPosition() = 0;
    virtual void setPosition(uintmax_t position) = 0;

    private:

    FileReader(const FileReader&);
    FileReader& operator=(const FileReader&);
};

class FileWriter {
    public:

    FileWriter() {}
    virtual ~FileWriter() {}

    virtual void writeFmt(const char* fmt, ...) = 0;
    virtual void writeChar(char value) = 0;
    virtual void writeByte(uint8_t value) = 0;
    virtual void writeWord(uint16_t value) = 0;
    virtual void writeDword(uint32_t value) = 0;
    virtual void writeBlock(void* buffer, uintmax_t size) = 0;

    private:

    FileWriter(const FileWriter&);
    FileWriter& operator=(const FileWriter&);
};

#endif
