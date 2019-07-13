#ifndef PLATFORM__FILESYSTEM_H__INCLUDED
#define PLATFORM__FILESYSTEM_H__INCLUDED

#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

//
// Interface
//

class Path;
class FileReader;
class FileWriter;

class FileSystem {
    public:

    FileSystem() {}
    virtual ~FileSystem() {}

    virtual std::unique_ptr<Path> path(std::string path) = 0;

    private:

    FileSystem(const FileSystem&);
    FileSystem& operator=(const FileSystem&);
};

class Path {
    public:

    Path() {}
    virtual ~Path() {}

    virtual std::string string() = 0;
    virtual std::string fileName() = 0;
    virtual std::string extension() = 0;
    virtual std::unique_ptr<Path> concat(std::string value) = 0;
    virtual std::unique_ptr<Path> append(std::string path) = 0;
    virtual bool fileExists() = 0;
    virtual uintmax_t fileSize() = 0;
    virtual bool remove() = 0;
    virtual bool createFolder() = 0;
    virtual std::unique_ptr<FileReader> fileReader() = 0;
    virtual std::unique_ptr<FileWriter> fileWriter() = 0;

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

//
// Implementation
//

class FileSystemImpl : public FileSystem {
    public:

    std::unique_ptr<Path> path(std::string path);
};

class PathImpl : public Path {
    public:

    std::string string();
    std::string fileName();
    std::string extension();
    std::unique_ptr<Path> concat(std::string value);
    std::unique_ptr<Path> append(std::string value);
    bool fileExists();
    uintmax_t fileSize();
    bool remove();
    bool createFolder();
    std::unique_ptr<FileReader> fileReader();
    std::unique_ptr<FileWriter> fileWriter();

    private:

    boost::filesystem::path path;

    PathImpl(std::string& path) : path(path) {}
    PathImpl(boost::filesystem::path& path) : path(path) {}
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

    FileReaderImpl(boost::filesystem::path& path);
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

    FileWriterImpl(boost::filesystem::path& path);
    friend class PathImpl;
};

#endif
