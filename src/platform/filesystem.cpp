// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <stdarg.h>
#include "filesystem.h"

//
// FileSystemImpl
//

std::unique_ptr<Path> FileSystemImpl::path(std::string path) {
    return std::unique_ptr<Path>(new PathImpl(path));
}

//
// PathImpl
//

std::string PathImpl::string() {
    return path.string();
}

std::string PathImpl::fileName() {
    return path.filename().string();
}

std::string PathImpl::extension() {
    return path.extension().string();
}

std::unique_ptr<Path> PathImpl::concat(std::string value) {
    boost::filesystem::path newPath(path);
    newPath += value;

    return std::unique_ptr<Path>(new PathImpl(newPath));
}

std::unique_ptr<Path> PathImpl::append(std::string value) {
    boost::filesystem::path newPath(path);
    newPath /= value;

    return std::unique_ptr<Path>(new PathImpl(newPath));
}

bool PathImpl::fileExists() {
    return boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path);
}

uintmax_t PathImpl::fileSize() {
    return boost::filesystem::file_size(path);
}

bool PathImpl::remove() {
    return boost::filesystem::remove(path);
}

bool PathImpl::createFolder() {
    return boost::filesystem::create_directory(path);
}

std::unique_ptr<FileReader> PathImpl::fileReader() {
    return std::unique_ptr<FileReader>(new FileReaderImpl(path));
}

std::unique_ptr<FileWriter> PathImpl::fileWriter() {
    return std::unique_ptr<FileWriter>(new FileWriterImpl(path));
}

//
// FileReaderImpl
//

FileReaderImpl::FileReaderImpl(boost::filesystem::path& path) {
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

FileWriterImpl::FileWriterImpl(boost::filesystem::path& path) {
    ofs.open(path.string(), std::ifstream::out | std::ifstream::binary | std::ifstream::trunc);
}

FileWriterImpl::~FileWriterImpl() {
    ofs.close();
}

void FileWriterImpl::writeFmt(const char* fmt, ...) {
    va_list arg_ptr;
    char buffer[0x400];

    va_start(arg_ptr, fmt);
    int size = vsnprintf(buffer, sizeof(buffer), fmt, arg_ptr);
    va_end(arg_ptr);

    ofs.write(buffer, size);
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
