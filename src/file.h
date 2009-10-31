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

#ifndef _FILE_H_
#define _FILE_H_ 1

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

class C_File
{
	public:

	C_File();
	~C_File();

	void Read(const char *filename);
	void Write(const char *filename);
	void Append(const char *filename);
	void Close(void);

	void PrintF(const char *fmt, ...);
	void PutBYTE(BYTE b);
	void PutWORD(WORD w);
	void PutDWORD(DWORD d);
	void PutC(int c);

	void GetS(char *buf, int size);
	BYTE GetBYTE(void);
	WORD GetWORD(void);
	DWORD GetDWORD(void);
	int GetC(void);

	int ReadBlock(void *buf, int size);
	void WriteBlock(void *buf, int size);

	bool IsOpened(void);
	bool Eof(void);
	void UnGetBYTE(BYTE b);
	void UnGetC(int c);

	static long FileSize(const char *filename);
	static bool FileExists(const char *filename);

	unsigned long GetFilePointer(void);
	void SetFilePointer(unsigned long fp);

	private:

	void TryNClose(void);

	BYTE buffer[FILECACHE_SIZE];
	int handle, len, accMode, un_ch;
	unsigned long readSize, readFileSize;
	bool eof, un_eof, isCompressed;
};

#endif
