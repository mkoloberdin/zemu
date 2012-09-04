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
	void PutBYTE(uint8_t b);
	void PutWORD(uint16_t w);
	void PutDWORD(uint32_t d);
	void PutC(int c);

	void GetS(char *buf, int size);
	uint8_t GetBYTE(void);
	uint16_t GetWORD(void);
	uint32_t GetDWORD(void);
	int GetC(void);

	size_t ReadBlock(void *buf, size_t size);
	void WriteBlock(void *buf, size_t size);

	bool IsOpened(void);
	bool Eof(void);
	void UnGetBYTE(uint8_t b);
	void UnGetC(int c);

	static size_t FileSize(const char *filename);
	static bool FileExists(const char *filename);

	size_t GetFilePointer(void);
	void SetFilePointer(size_t fp);

	private:

	void TryNClose(void);

	uint8_t buffer[FILECACHE_SIZE];
	int handle, len, accMode, un_ch;
	unsigned long readSize, readFileSize;
	bool eof, un_eof, isCompressed;

	C_File& operator=(const C_File &dummy) { return *this; }
	C_File(const C_File &dummy) {}
};

#endif
