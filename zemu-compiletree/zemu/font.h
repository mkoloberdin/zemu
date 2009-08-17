#ifndef _FONT_H
#define _FONT_H_ 1

#include "zemu.h"

class C_Font
{
	public:

	C_Font();
	~C_Font();

	void Init(BYTE *data);
	void Init(SDL_Surface *surf);

	void PrintChar(int x, int y, char c);
	void PrintString(int x, int y, const char *str);
	int StrLenPx(const char *str);

	int Height(void);

	void CopySym(char from, char to);
	void SetSymOff(char c, int x, int y);

	private:

	int spitch;
	SDL_Surface *surf;
	int off[0x100], len[0x100], xoff[0x100], yoff[0x100];

	void CalcFont(void);
};

#endif
