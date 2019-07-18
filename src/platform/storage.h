#ifndef PLATFORM__STORAGE_H__INCLUDED
#define PLATFORM__STORAGE_H__INCLUDED

#include <memory>
#include <string>
#include <cstdint>
#include <stdexcept>

class Storage;
class Path;
class DataReader;
class DataWriter;

typedef std::shared_ptr<Path> PathPtr;
typedef std::unique_ptr<DataReader> DataReaderPtr;
typedef std::unique_ptr<DataWriter> DataWriterPtr;

class StorageException : public std::runtime_error {
public:
    StorageException(const std::string& whatArg) : std::runtime_error(whatArg) {};
    StorageException(const char* whatArg) : std::runtime_error(whatArg) {};
};

class Storage {
public:

    Storage() {}
    virtual ~Storage() {}

    virtual PathPtr path(const std::string& path) = 0;
    virtual PathPtr appDataPath() = 0;
    virtual PathPtr findExtras(const std::string& fileName) = 0;
    virtual PathPtr findExtras(const std::string& directory, const std::string& fileName) = 0;

    virtual uintmax_t readExtras(
        const char* directory,
        const std::string& fileName,
        uint8_t* buffer,
        uintmax_t maxSize,
        uintmax_t offset = 0
    ) = 0;

private:

    Storage(const Storage&);
    Storage& operator=(const Storage&);
};

class Path {
public:

    Path() {}
    virtual ~Path() {}

    virtual bool isWriteSupported() = 0;

    virtual bool isEmpty() = 0;
    virtual bool isRoot() = 0;

    virtual std::string string() = 0;
    virtual std::string fileName() = 0;
    virtual std::string extension() = 0;

    virtual PathPtr parent() = 0;
    virtual PathPtr concat(const std::string& value) = 0;
    virtual PathPtr append(const std::string& path) = 0;
    virtual PathPtr canonical() = 0;

    virtual bool isExists() = 0;
    virtual bool isFile() = 0;
    virtual bool isDirectory() = 0;
    virtual uintmax_t fileSize() = 0;
    virtual void listEntries(std::vector<PathPtr>& into) = 0;

    virtual DataReaderPtr dataReader() = 0;

    virtual bool remove() = 0;
    virtual bool removeAll() = 0;
    virtual bool createDirectories() = 0;
    virtual DataWriterPtr dataWriter() = 0;

    std::string extensionLc();

private:

    Path(const Path&);
    Path& operator=(const Path&);
};

class DataReader {
public:

    DataReader() {}
    virtual ~DataReader() {}

    virtual bool isEof() = 0;
    virtual char readChar() = 0;
    virtual uint8_t readByte() = 0;
    virtual uint16_t readWord() = 0;
    virtual uint32_t readDword() = 0;
    virtual uintmax_t readBlock(void* buffer, uintmax_t maxSize) = 0;
    virtual std::string readLine() = 0;
    virtual uintmax_t getPosition() = 0;
    virtual void setPosition(uintmax_t position) = 0;

private:

    DataReader(const DataReader&);
    DataReader& operator=(const DataReader&);
};

class DataWriter {
public:

    DataWriter() {}
    virtual ~DataWriter() {}

    virtual void writeFmt(const char* fmt, ...) = 0;
    virtual void writeChar(char value) = 0;
    virtual void writeByte(uint8_t value) = 0;
    virtual void writeWord(uint16_t value) = 0;
    virtual void writeDword(uint32_t value) = 0;
    virtual bool writeBlock(void* buffer, uintmax_t size) = 0;
    virtual uintmax_t getPosition() = 0;
    virtual void setPosition(uintmax_t position) = 0;

private:

    DataWriter(const DataWriter&);
    DataWriter& operator=(const DataWriter&);
};

#endif
