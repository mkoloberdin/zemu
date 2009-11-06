/*
    Tanks - Tank arcade game
    Copyright (c) 2005 ProZroks

    <<< LICENSE >>>

    ProZroks
    support@prozroks.com

    Last modifed: Restorer (2 Jun, 2005)
*/

/*
    Whatsnew.

    Restorer (24 May, 2005)
        Только начало. Общий интерфейс.

    Restorer (2 Jun, 2005)
        Enum нафих, заменил define-ами
*/

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_ 1

#include <string>
using namespace std;

#define E_None					0	// 0..99 - Reserved
#define E_General				1
#define E_NotImplemented			100	// 100..199 - Общие исключения
#define E_FileNotFound				200	// 200.299 - File
#define E_ReadError				201
#define E_WriteError				202
#define E_FileNotOpened				203
#define E_IncorrectAccMode			204
#define E_UnGetError				205
#define E_FileIsCompressed			206
#define E_XMLParserError			300	// 300..399 - Config
#define E_ConfigError				301
#define E_DirWork				400

#define E_CantLock				1000	// 1000..1099 - Gfx
#define E_HandlerAlreadyDefined			1100	// 1100..1199 - Gadgets
#define E_HandlerNotDefined			1101
#define E_ObjGfxNotFound			1200

#define E_SnapZ80Error				10000	// ZEmu specific

class C_E
{
	public:

	C_E();
	C_E(int exc);
	C_E(int exc, const char *param);
	C_E(int exc, string param);
	const char* Descr(void) const;

	int exc;
	char param[0x100];
};

#endif
