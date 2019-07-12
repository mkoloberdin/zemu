// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include "file.h"
#include "exceptions.h"
#include <memory.h>
#include <algorithm>

#ifdef AUTO_DEHRUST
    #include "dehrust.h"
#endif

#ifdef _WIN32
    #include <io.h>
    #include <sys/stat.h>
#else
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

#include <fcntl.h>

#ifndef _WIN32
    #define _open open
    #define _lseek lseek
    #define _close close
    #define _read read
    #define _write write
    #define _vsnprintf vsnprintf

    #define _O_BINARY 0 // there a no "special text files" in *nix

    #define _O_RDONLY O_RDONLY
    #define _O_CREAT O_CREAT
    #define _O_TRUNC O_TRUNC
    #define _O_WRONLY O_WRONLY

    #ifdef S_IREAD
        #define _S_IREAD S_IREAD
    #else
        #define _S_IREAD 0
    #endif

    #ifdef S_IWRITE
        #define _S_IWRITE S_IWRITE
    #else
        #define _S_IWRITE 0
    #endif
#endif

size_t C_File::FileSize(const char* filename) {
    int h = _open(filename, _O_BINARY | _O_RDONLY);

    if (h == -1) {
        throw C_E(E_FileNotFound, filename);
    }

    long l = _lseek(h, 0L, SEEK_END);
    _close(h);

    return l;
}

bool C_File::FileExists(const char* filename) {
    int h = _open(filename, _O_BINARY | _O_RDONLY);

    if (h == -1) {
        return false;
    }

    _close(h);
    return true;
}

bool C_File::Unlink(const char* filename) {
    return (unlink(filename) == 0);
}

C_File::C_File(const char* filename) {
    if (!strlen(filename)) {
        throw C_E(E_EmptyFileName);
    }

    handle = _open(filename, _O_BINARY | _O_RDONLY);

    if (handle == -1) {
        throw C_E(E_ReadError, filename);
    }

    readSize = _lseek(handle, 0L, SEEK_END);
    _lseek(handle, 0L, SEEK_SET);

    eof = (readSize == 0);

    if (!eof) {
        if ((unsigned)_read(handle, buffer, FILECACHE_SIZE) != std::min((unsigned)readSize, (unsigned)FILECACHE_SIZE)) {
            DEBUG_MESSAGE("_read failed");
        }

        #ifdef AUTO_DEHRUST
            C_Dehrust dh;

            if (readSize >= 10 && buffer[0] == 'H' && buffer[1] == 'R') {
                uint8_t* tmp = new uint8_t[0x10000];
                readSize = dh.Extract(buffer, tmp);
                memcpy(buffer, tmp, readSize);
                delete[] tmp;
                isCompressed = true;
            } else {
                isCompressed = false;
            }
        #else
            isCompressed = false;
        #endif
    }

    accMode = ACC_READ;
    len = 0;
    un_ch = -1;
    un_eof = false;
    readFileSize = readSize;
}

C_File::C_File(const char* filename, bool append) { //-V730
    if (!strlen(filename)) {
        throw C_E(E_EmptyFileName);
    }

    if (append) {
        throw C_E(E_NotImplemented);
    }

    handle = _open(filename, _O_CREAT | _O_BINARY | _O_TRUNC | _O_WRONLY, _S_IREAD | _S_IWRITE);

    if (handle == -1) {
        throw C_E(E_WriteError, filename);
    }

    accMode = ACC_WRITE;
    readSize = 0;
    readFileSize = 0;
    eof = false;
    isCompressed = false;
    len = 0;
    un_ch = -1;
    un_eof = false;
}

C_File::~C_File() {
    if ((accMode == ACC_WRITE || accMode == ACC_APPEND) && len != 0 && _write(handle, buffer, len) != len) {
        DEBUG_MESSAGE("_write failed");
    }

    _close(handle);
}

uint8_t C_File::GetBYTE(void) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    uint8_t c;

    if (un_ch != -1) {
        c = un_ch;
        un_ch = -1;
        eof = un_eof;
        return c;
    }

    if (eof) {
        return 0;
    }

    if (len >= FILECACHE_SIZE) {
        if ((unsigned)_read(handle, buffer, FILECACHE_SIZE) != std::min((unsigned)readSize, (unsigned)FILECACHE_SIZE)) {
            DEBUG_MESSAGE("_read failed");
        }

        len = 0;
    }

    readSize--;

    if (readSize <= 0) {
        eof = true;
    }

    return buffer[len++];
}

void C_File::PutBYTE(uint8_t b) {
    if (accMode != ACC_WRITE && accMode != ACC_APPEND) {
        throw C_E(E_IncorrectAccMode);
    }

    buffer[len++] = b;

    if (len >= FILECACHE_SIZE) {
        if (_write(handle, buffer, FILECACHE_SIZE) != FILECACHE_SIZE) {
            DEBUG_MESSAGE("_write failed");
        }

        len = 0;
    }
}

void C_File::UnGetBYTE(uint8_t b) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    if (un_ch != -1) {
        throw C_E(E_UnGetError);
    }

    un_ch = b;
    un_eof = eof;
    eof = false;
}

bool C_File::Eof(void) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    return eof;
}

int C_File::GetC(void) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    return (int)GetBYTE();
}

void C_File::UnGetC(int c) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    UnGetBYTE((uint8_t)c);
}

void C_File::PutC(int c) {
    if (accMode != ACC_WRITE && accMode != ACC_APPEND) {
        throw C_E(E_IncorrectAccMode);
    }

    PutBYTE((uint8_t)c);
}

void C_File::PutWORD(uint16_t w) {
    if (accMode != ACC_WRITE && accMode != ACC_APPEND) {
        throw C_E(E_IncorrectAccMode);
    }

    PutBYTE((uint8_t)(w & 0xFF));
    PutBYTE((uint8_t)(w >> 8));
}

void C_File::PutDWORD(uint32_t d) {
    if (accMode != ACC_WRITE && accMode != ACC_APPEND) {
        throw C_E(E_IncorrectAccMode);
    }

    PutWORD((uint16_t)(d & (uint32_t)0xFFFF));
    PutWORD((uint16_t)(d >> 0x10));
}

uint16_t C_File::GetWORD(void) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    int l = GetBYTE();
    int h = GetBYTE();
    return (uint16_t)(l + 0x100 * h);
}

uint32_t C_File::GetDWORD(void) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    uint32_t l = GetWORD();
    uint32_t h = GetWORD();
    return (uint32_t)(l + (uint32_t)0x10000 * h);
}

void C_File::PrintF(const char* fmt, ...) {
    if (accMode != ACC_WRITE && accMode != ACC_APPEND) {
        throw C_E(E_IncorrectAccMode);
    }

    char buf[0x1000];
    va_list argptr;

    va_start(argptr, fmt);
    int sz = _vsnprintf(buf, sizeof(buf), fmt, argptr);
    va_end(argptr);

    for (int i = 0; i < sz; i++) {
        PutBYTE(buf[i]);
    }
}

void C_File::GetS(char* buf, int size) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    int o = 0;
    bool r = true;

    do {
        if (eof) {
            r = false;
        } else {
            int c = GetC();

            if (c == 0x0D) {
                r = false;

                if (!eof) {
                    c = GetC();

                    if (c != 0x0A) {
                        UnGetC(c);
                    }
                }
            } else if (c == 0x0A) {
                r = false;

                if (!eof) {
                    c = GetC();

                    if (c != 0x0D) {
                        UnGetC(c);
                    }
                }
            } else if (o >= size-1) {
                UnGetC(c);
                r = false;
            } else {
                buf[o++] = c;
            }
        }
    } while (r);

    buf[o] = 0;
}

size_t C_File::GetFilePointer(void) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    if (isCompressed) {
        throw C_E(E_FileIsCompressed);
    }

    return (eof ? readFileSize : (readFileSize - readSize));
}

void C_File::SetFilePointer(size_t fp) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    if (isCompressed) {
        throw C_E(E_FileIsCompressed);
    }

    _lseek(handle, fp, SEEK_SET);
    readSize = readFileSize - fp;

    eof = (readSize <= 0);
    len = FILECACHE_SIZE;
    accMode = ACC_READ;
}

size_t C_File::ReadBlock(void* buf, size_t size) {
    if (accMode != ACC_READ) {
        throw C_E(E_IncorrectAccMode);
    }

    uint8_t* bf = (uint8_t*)buf;
    size_t act = 0;

    for (; size && !Eof(); size--, act++) {
        *(bf++) = GetBYTE();
    }

    return act;
}

void C_File::WriteBlock(void* buf, size_t size) {
    if (accMode != ACC_WRITE && accMode != ACC_APPEND) {
        throw C_E(E_IncorrectAccMode);
    }

    uint8_t* bf = (uint8_t*)buf;

    if (!size) {
        return;
    }

    while (size--) {
        PutBYTE(*(bf++));
    }
}
