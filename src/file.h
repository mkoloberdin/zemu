#ifndef _FILE_H_INCLUDED_
#define _FILE_H_INCLUDED_

#include "defines.h"

#define ACC_NONE 0
#define ACC_READ 1
#define ACC_WRITE 2
#define ACC_APPEND 3

#ifdef AUTO_DEHRUST
    #define FILECACHE_SIZE (0x10000)
#else
    #define FILECACHE_SIZE 1024
#endif

class C_File {
    public:

    static size_t FileSize(const char* filename);
    static bool FileExists(const char* filename);
    static bool Unlink(const char* filename);

    // Very very bad API for creating files for read / write,
    // this will be reworked later.
    C_File(const char* filename);
    C_File(const char* filename, bool append);

    ~C_File();

    void PrintF(const char* fmt, ...);
    void PutBYTE(uint8_t b);
    void PutWORD(uint16_t w);
    void PutDWORD(uint32_t d);
    void PutC(int c);

    void GetS(char* buf, int size);
    uint8_t GetBYTE(void);
    uint16_t GetWORD(void);
    uint32_t GetDWORD(void);
    int GetC(void);

    size_t ReadBlock(void* buf, size_t size);
    void WriteBlock(void* buf, size_t size);

    bool Eof(void);
    void UnGetBYTE(uint8_t b);
    void UnGetC(int c);

    size_t GetFilePointer(void);
    void SetFilePointer(size_t fp);

    private:

    uint8_t buffer[FILECACHE_SIZE];
    int accMode;
    int handle;
    unsigned long readSize;
    unsigned long readFileSize;
    bool eof;
    bool isCompressed;
    int len;
    int un_ch;
    bool un_eof;

    C_File(const C_File&);
    C_File& operator=(const C_File&);
};

#endif
