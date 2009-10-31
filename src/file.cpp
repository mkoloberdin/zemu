/*
    Tanks - Tank arcade game
    Copyright (c) 2005 ProZroks

    <<< LICENSE >>>

    ProZroks
    support@prozroks.com

    Last modifed: Restorer (31 Aug, 2005)
*/

/*
    Whatsnew.

    Restorer (24 May, 2005)
        Только начало. Общий интерфейс.

    Restorer (2 Jun, 2005)
        Куча фиксов. Чтение работает. Вроде бы ^_^ (да и всё остальное тоже должно бенчить)

    Restorer (12 Jun, 2005)
        [fix] FileSize

    Restorer (18 Jun, 2005)
        [add] SetFilePointer

    Restorer (7 Jyl, 2005)
        [fix] Read

    Restorer
        [add] AutoDehrust

    Restorer (31 Aug, 2005)
        [fix] GetS
*/

#include <stdio.h>
#include "file.h"
#include "exceptions.h"
#include <memory.h>

#ifdef AUTO_DEHRUST
	#include "dehrust.h"
#endif

#ifdef _WIN32
	#include <io.h>
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

	#ifdef _LINUX
		// there a no "special text files" in linux
		#define _O_BINARY 0
	#else
		// if O_BINARY is not defined on your system, define it to zero
		#define _O_BINARY O_BINARY
	#endif

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

C_File::C_File()
{
	accMode = ACC_NONE;
	un_ch = -1;
}

C_File::~C_File()
{
	TryNClose();
}

void C_File::TryNClose(void)
{
	if (accMode != ACC_NONE) Close();
}

bool C_File::IsOpened(void)
{
	return (accMode != ACC_NONE);
}

long C_File::FileSize(const char *filename)
{
	int h;
	long l;

	h = _open(filename, _O_BINARY | _O_RDONLY);
	if (h == -1) throw C_E(E_FileNotFound, filename);
	l = _lseek(h, 0L, SEEK_END);
	_close(h);

	return l;
}

bool C_File::FileExists(const char *filename)
{
	int h;

	h = _open(filename, _O_BINARY | _O_RDONLY);
	if (h == -1) return false;

	_close(h);
	return true;
}

void C_File::Read(const char *filename)
{
#ifdef AUTO_DEHRUST
	BYTE *tmp;
#endif

	handle = _open(filename, _O_BINARY | _O_RDONLY);
	if (handle == -1) throw C_E(E_ReadError, filename);

	readSize = _lseek(handle, 0L, SEEK_END);
	_lseek(handle, 0L, SEEK_SET);

	eof = (readSize == 0);
	len = 0;

	if (!eof)
	{
		if ((unsigned)_read(handle, buffer, FILECACHE_SIZE) != min(readSize, FILECACHE_SIZE)) {
			_DEBUG("_read failed");
		}

#ifdef AUTO_DEHRUST
		C_Dehrust dh;

		if (readSize >= 10)
		{
			if (buffer[0]=='H' && buffer[1]=='R')
			{
				tmp = new BYTE[0x10000];
				readSize = dh.Extract(buffer, tmp);
				memcpy(buffer, tmp, readSize);
				delete tmp;
				isCompressed = true;
			} else isCompressed = false;
		} else isCompressed = false;
#else
		isCompressed = false;
#endif
	}

	readFileSize = readSize;
	accMode = ACC_READ;
	un_ch = -1;
}

void C_File::Write(const char *filename)
{
	handle = _open(filename, _O_CREAT | _O_BINARY | _O_TRUNC | _O_WRONLY, _S_IREAD | _S_IWRITE);
	if (handle == -1) throw C_E(E_WriteError, filename);
	len = 0;

	accMode = ACC_WRITE;
}

void C_File::Append(const char *filename)
{
	throw C_E(E_NotImplemented);
}

void C_File::Close(void)
{
	if (accMode != ACC_NONE)
	{
		if (accMode!=ACC_READ && len!=0)
		{
			if (_write(handle, buffer, len) != len) {
				_DEBUG("_write failed");
			}
		}

		_close(handle);
		accMode = ACC_NONE;
	}
	else
	{
		throw C_E(E_FileNotOpened);
	}
}

BYTE C_File::GetBYTE(void)
{
	BYTE c;

	if (accMode != ACC_READ) throw C_E(E_IncorrectAccMode);

	if (un_ch != -1)
	{
		c = un_ch;
		un_ch = -1;
		eof = un_eof;
		return c;
	}

	if (eof) return 0;

	if (len >= FILECACHE_SIZE)
	{
		if ((unsigned)_read(handle, buffer, FILECACHE_SIZE) != min(readSize, FILECACHE_SIZE)) {
			_DEBUG("_read failed");
		}

		len = 0;
	}

	readSize--;
	if (readSize <= 0) eof = true;

	return buffer[len++];
}

void C_File::PutBYTE(BYTE b)
{
	if (accMode != ACC_WRITE) throw C_E(E_IncorrectAccMode);
	buffer[len++] = b;

	if (len >= FILECACHE_SIZE)
	{
		if (_write(handle, buffer, FILECACHE_SIZE) != FILECACHE_SIZE) {
			_DEBUG("_write failed");
		}

		len = 0;
	}
}

void C_File::UnGetBYTE(BYTE b)
{
	if (accMode != ACC_READ) throw C_E(E_IncorrectAccMode);
	if (un_ch != -1) throw C_E(E_UnGetError);

	un_ch = b;
	un_eof = eof;

	eof = false;
}

bool C_File::Eof(void)
{
	if (accMode != ACC_READ) throw C_E(E_IncorrectAccMode);
	return eof;
}

int C_File::GetC(void)
{
	return (int)GetBYTE();
}

void C_File::UnGetC(int c)
{
	UnGetBYTE((BYTE)c);
}

void C_File::PutC(int c)
{
	PutBYTE((BYTE)c);
}

void C_File::PutWORD(WORD w)
{
	PutBYTE((BYTE)(w & 0xFF));
	PutBYTE((BYTE)(w >> 8));
}

void C_File::PutDWORD(DWORD d)
{
	PutWORD((WORD)(d & (DWORD)0xFFFF));
	PutWORD((WORD)(d >> 0x10));
}

WORD C_File::GetWORD(void)
{
	int dummy1, dummy2;
	dummy1 = GetBYTE();
	dummy2 = GetBYTE();
	return (WORD)(dummy1 + 0x100*dummy2); // ��������� ����������� VisualC++6.0
}

DWORD C_File::GetDWORD(void)
{
	DWORD dummy1, dummy2;
	dummy1 = GetWORD();
	dummy2 = GetWORD();
	return (DWORD)(dummy1 + (DWORD)0x10000*dummy2); // ��������� ����������� VisualC++6.0
}

void C_File::PrintF(const char *fmt, ...)
{
	char buf[0x1000];
	int sz, i;
	va_list argptr;

	if (accMode != ACC_WRITE) throw C_E(E_IncorrectAccMode);

	va_start(argptr, fmt);
	sz = _vsnprintf(buf, sizeof(buf), fmt, argptr);
	va_end(argptr);

	for (i = 0; i < sz; i++) PutBYTE(buf[i]);
}

void C_File::GetS(char *buf, int size)
{
	bool r;
	int o, c;

	if (accMode != ACC_READ) throw C_E(E_IncorrectAccMode);

	o = 0;
	r = true;

	do
	{
		if (eof) r = false;
		else
		{
			c = GetC();

			if (c == 0x0D)
			{
				r = false;

				if (!eof)
				{
					c = GetC();
					if (c != 0x0A) UnGetC(c);
				}
			}
			else
			if (c == 0x0A)
			{
				r = false;

				if (!eof)
				{
					c = GetC();
					if (c != 0x0D) UnGetC(c);
				}
			}
			else
			{
				if (o >= size-1)
				{
					UnGetC(c);
					r = false;
				} else buf[o++] = c;
			}
		}
	} while (r);
	
	buf[o] = 0;
}

unsigned long C_File::GetFilePointer(void)
{
	if (accMode != ACC_READ) throw C_E(E_IncorrectAccMode);
	if (isCompressed) throw C_E(E_FileIsCompressed);

	return (eof ? readFileSize : readFileSize - readSize);
}

void C_File::SetFilePointer(unsigned long fp)
{
	if (accMode != ACC_READ) throw C_E(E_IncorrectAccMode);
	if (isCompressed) throw C_E(E_FileIsCompressed);

	_lseek(handle, fp, SEEK_SET);
	readSize = readFileSize - fp;

	eof = (readSize <= 0);
	len = FILECACHE_SIZE;

	accMode = ACC_READ;
}

int C_File::ReadBlock(void *buf, int size)
{
	BYTE *bf = (BYTE *)buf;
	int act = 0;

	for (; size && !Eof(); size--, act++) *(bf++) = GetBYTE();
	return act;
}

void C_File::WriteBlock(void *buf, int size)
{
	BYTE *bf = (BYTE *)buf;
	if (!size) return;
	while (size--) PutBYTE(*(bf++));
}
