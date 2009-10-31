#include <string.h>
#include <ctype.h>
#include "dialog.h"
#include "font.h"
#include "dirwork.h"
#include "graphics.h"
#include "lib_wd1793/wd1793_chip.h"
#include <z80ex_dasm.h>
#include "devs.h"
#include "tape/tape.h"
#include "labels.h"

#include "images/zemu_ico.h"

#define MAX_FILES 4096
#define MAX_FNAME 256

extern C_Font font;
extern C_Font fixed;

char oldFileName[4][MAX_PATH] = {"", "", "" ,""};
int currentDrive = 0;

int FileNameCmp(int f1, char *s1, int f2, char *s2)
{
	if (f1 == f2) return stricmp(s1, s2);
	else
	if (f1) return (-1);
	else return (1);
}

void DlgClearScreen(void)
{
	int i, j, cl;
	int *s, *p;

	if (SDL_MUSTLOCK(screen)) {if (SDL_LockSurface(screen) < 0) return;}
	s = (int *)screen->pixels;

	for (i = 0; i < HEIGHT; i++)
	{
		p = s;
		cl = (i & 1) ? DRGB(0,0,64) : DRGB(0,0,32);
		for (j = 0; j < WIDTH; j++) *(p++) = cl;
		s += PITCH;
	}

	if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
}

bool DlgConfirm(const char *message)
{
	int x, y, key;
	SDL_Event event;
	int wdt = font.StrLenPx(message) + 0x10;
	int hgt = font.Height() + 0x10;

	x = (WIDTH - wdt) / 2;
	y = (HEIGHT - hgt) / 2;

	DlgClearScreen();
	Bar(x, y, x+wdt-1, y+hgt-1, DRGB(0x80, 0x20, 0x20));
	font.PrintString(x+8, y+8, message);
	ScaleImage();
	UpdateScreen();

	for (;;)
	{
		for (key = 0; SDL_PollEvent(&event);)
		{
			if (event.type == SDL_QUIT) exit(0);
			if (event.type == SDL_KEYUP) key = event.key.keysym.sym;
		}

		if (key==SDLK_y || key==SDLK_RETURN) return true;
		if (key==SDLK_n || key==SDLK_ESCAPE) return false;

		SDL_Delay(10);
	}
}

#define MAX_INPUT_STRING 64

const char* DlgInputString(const char *message)
{
	static char buffer[MAX_INPUT_STRING + 1];
	char buf[0x100 + MAX_INPUT_STRING];

	buffer[0] = 0;
	int bufferSz = 0;

	int wdt, hgt;
	int x, y, key;
	SDL_Event event;

	for (;;)
	{
		sprintf(buf, "%s: %s", message, buffer);

		wdt = font.StrLenPx(buf) + 0x10;
		hgt = font.Height() + 0x10;

		x = (WIDTH - wdt) / 2;
		y = (HEIGHT - hgt) / 2;

		DlgClearScreen();
		Bar(x, y, x+wdt-1, y+hgt-1, DRGB(0x80, 0x20, 0x20));
		font.PrintString(x+8, y+8, buf);
		ScaleImage();
		UpdateScreen();

		do
		{
			for (key = 0; SDL_PollEvent(&event);)
			{
				if (event.type == SDL_QUIT) exit(0);
				if (event.type == SDL_KEYDOWN) key = event.key.keysym.sym;
			}
		} while (key == 0);

		if (key == SDLK_ESCAPE)
		{
			buffer[0] = 0;
			return buffer;
		}
		else
		if (key == SDLK_RETURN)
		{
			return buffer;
		}
		else
		if (key == SDLK_BACKSPACE)
		{
			if (bufferSz > 0) {
				buffer[--bufferSz] = 0;
			}
		}
		else
		if (key>=32 && key<=127)
		{
			if (bufferSz < MAX_INPUT_STRING-1) {
				buffer[bufferSz++] = key;
				buffer[bufferSz] = 0;
			}
		}

		SDL_Delay(10);
	}
}

char* SelectFile(char *oldFile)
{
	int key;
	SDL_Event event;
	C_DirWork dw;
	char buf[0x100];
	static char path[MAX_PATH], ofl[MAX_FNAME+1];
	char fnames[MAX_FILES][MAX_FNAME+1], temp[MAX_FNAME+1];
	int i, j, filesCnt, folders[MAX_FILES], pos, csr, mx, gl, h, x, tmp;
	int scrEnd, keyx;
	bool isRoot;

	scrEnd = HEIGHT - font.Height() - 8;

	strcpy(ofl, C_DirWork::ExtractFileName(oldFile));
	strcpy(path, C_DirWork::ExtractPath(oldFile));
	strcpy(path, C_DirWork::Normalize(path));

	h = font.Height();
	mx = scrEnd / h;
	x = font.StrLenPx("[]");

	do
	{
		isRoot = !strcmp(path, "/");
		filesCnt = 0;

		if (dw.EnumFiles(path))
		{
			do
			{
				if (strcmp(dw.name, "."))
				{
					if (strcmp(dw.name, "..") || !isRoot)
					{
						strcpy(fnames[filesCnt], dw.name);
						folders[filesCnt] = (dw.attr == DW_FOLDER);
						filesCnt++;
					}
				}
			} while (dw.EnumNext());

			dw.EnumClose();
		}

		gl = ((filesCnt > 0) ? (strcmp(fnames[0], "..") ? 0 : 1) : 0);

		for (i = gl; i < filesCnt-1; i++)
		{
			for (j = i+1; j < filesCnt; j++)
			{
				if (FileNameCmp(folders[i], fnames[i], folders[j], fnames[j]) > 0)
				{
					strcpy(temp, fnames[i]);
					strcpy(fnames[i], fnames[j]);
					strcpy(fnames[j], temp);

					tmp = folders[i];
					folders[i] = folders[j];
					folders[j] = tmp;
				}
			}
		}

		pos = 0;
		csr = 0;

		for (i = gl; i < filesCnt; i++)
		{
			if (!strcmp(ofl, fnames[i]))
			{
				csr = i;

				if (filesCnt > mx)
				{
					pos = csr - mx / 2;
					if (pos < 0) pos = 0;
				}

				break;
			}
		}

		gl = (filesCnt>mx ? mx : filesCnt);

		if (gl+pos-1 >= filesCnt)
		{
			pos = filesCnt - gl;
			if (pos < 0) pos = 0;
		}

		do
		{
			do
			{
				for (key = 0; SDL_PollEvent(&event);)
				{
					if (event.type == SDL_QUIT) exit(0);
					if (event.type == SDL_KEYDOWN) key = event.key.keysym.sym;
				}

				DlgClearScreen();
				Bar(0, scrEnd, WIDTH-1, HEIGHT-1, DRGB(0x80, 0x20, 0x20));

				Bar(4+(currentDrive*16), scrEnd+4, 4+(currentDrive*16)+12, scrEnd+4+12, DRGB(0x40, 0x10, 0x10));
				font.PrintString(5, scrEnd+5, "A");
				font.PrintString(5+0x10+1, scrEnd+5, "B");
				font.PrintString(5+0x20+1, scrEnd+5, "C");
				font.PrintString(5+0x30+1, scrEnd+5, "D");

				Bar(4+0x50, scrEnd+4, 4+0x50+20, scrEnd+4+12, wd1793_is_disk_wprotected(currentDrive) ? DRGB(255,128,32) : DRGB(0x40,0x10,0x10));
				Bar(4+0x68, scrEnd+4, 4+0x68+20, scrEnd+4+12, wd1793_is_disk_loaded(currentDrive) ? DRGB(255,128,32) : DRGB(0x40,0x10,0x10));
				Bar(4+0x80, scrEnd+4, 4+0x80+20, scrEnd+4+12, wd1793_is_disk_changed(currentDrive) ? DRGB(255,128,32) : DRGB(0x40,0x10,0x10));

				font.PrintString(5+0x50, scrEnd+5, "WP");
				font.PrintString(5+0x68, scrEnd+5, "LD");
				font.PrintString(5+0x80, scrEnd+5, "CH");

				Bar(4+0xB0, scrEnd+4, 4+0xB0+72, scrEnd+4+12, C_Tape::IsActive() ? DRGB(255,128,32) : DRGB(0x40,0x10,0x10));

				if (C_Tape::IsLoaded()) sprintf(buf, "TAPE %d%%", C_Tape::GetPosPerc());
				else sprintf(buf, "TAPE NOP");

				font.PrintString(5+0xB0, scrEnd+5, buf);

				for (i = 0; i < gl; i++)
				{
					if (csr-pos == i) Bar(0, i*h, x+font.StrLenPx(fnames[i+pos])+x, i*h+h-1, DRGB(0x80, 0x20, 0x20));

					if (folders[i+pos]) font.PrintString(0, i*h, "[]");
					font.PrintString(x, i*h, fnames[i+pos]);
				}

				OutputGimpImage(WIDTH - img_zemuIco.width - 8, 8, (s_GimpImage *) &img_zemuIco);
				ScaleImage();
				UpdateScreen();
				SDL_Delay(10);
			} while (key == 0);

			if (key == SDLK_s)
			{
				if (DlgConfirm("Are you sure to save disk? (Y/N)"))
				{
					printf("Saving \"%s\" (drive %c)\n", oldFileName[currentDrive], "ABCD"[currentDrive]);

					#ifdef _WIN32
						char tname[MAX_PATH];

						strcpy(tname, oldFileName[currentDrive]);

						if (tname[0]=='/' && tname[2]=='/')
						{
							tname[0] = tname[1];
							tname[1] = ':';
						}
					#else
						char *tname = oldFileName[currentDrive];
					#endif

					if (!wd1793_save_dimage(tname, currentDrive, imgTRD)) {
						DlgConfirm("Save failed :(");
					}
				}
			}
			else
			if (key == SDLK_e)
			{
				wd1793_eject_dimage(currentDrive);
			}
			else
			if (key == SDLK_d)
			{
				C_Tape::Eject();
			}
			else
			if (key == SDLK_r)
			{
				C_Tape::Rewind();
			}
			else
			if (key == SDLK_t)
			{
				if (C_Tape::IsActive()) C_Tape::Stop();
				else C_Tape::Start();
			}
			else
			if (key == SDLK_w)
			{
				wd1793_set_disk_wprotected(currentDrive, !wd1793_is_disk_wprotected(currentDrive));
			}
			else
			if (key == SDLK_UP)
			{
				csr--;

				if (csr-pos < 0)
				{
					if (pos > 0) pos--;
					else csr++;
				}
			}
			else
			if (key == SDLK_DOWN)
			{
				csr++;

				if (csr-pos >= gl)
				{
					if (pos+gl < filesCnt) pos++;
					else csr--;
				}
			}
			else
			if (key == SDLK_HOME)
			{
				csr = 0;
				pos = 0;
			}
			else
			if (key == SDLK_END)
			{
				csr = filesCnt - 1;

				if (filesCnt > mx)
				{
					pos = filesCnt - mx;
				}
				else pos = 0;
			}
			else
			if (key == SDLK_PAGEUP)
			{
				pos -= mx;
				csr -= mx;

				if (pos < 0)
				{
					pos = 0;
					csr = 0;
				}
			}
			else
			if (key == SDLK_PAGEDOWN)
			{
				pos += mx;
				csr += mx;

				if (pos+gl > filesCnt)
				{
					if (filesCnt > mx)
					{
						csr = filesCnt - 1;
						pos = filesCnt - mx;
					}
					else
					{
						pos = 0;
						csr = filesCnt - 1;
					}
				}
			}
			else
			if (key == SDLK_LEFT)
			{
				currentDrive--;
				if (currentDrive < 0) currentDrive = 0;
			}
			else
			if (key == SDLK_RIGHT)
			{
				currentDrive++;
				if (currentDrive > 3) currentDrive = 3;
			}

		} while (key!=SDLK_RETURN && key!=SDLK_ESCAPE && key!=SDLK_BACKSPACE);

		do
		{
			SDL_Delay(1);

			for (keyx = 0; SDL_PollEvent(&event);)
			{
				if (event.type == SDL_QUIT) exit(0);
				if (event.type == SDL_KEYUP) keyx = event.key.keysym.sym;
			}
		} while (key != keyx);

		if (key == SDLK_BACKSPACE)
		{
			strcpy(ofl, C_DirWork::LastDirName(path));
			strcpy(path, C_DirWork::LevelUp(path));
		}
		else
		if (key == SDLK_RETURN)
		{
			if (folders[csr])
			{
				strcpy(ofl, C_DirWork::LastDirName(path));

				if (!strcmp(fnames[csr], ".."))
				{
					strcpy(path, C_DirWork::LevelUp(path));
				}
				else
				{
					strcpy(path, C_DirWork::ExtractPath(path));
					strcat(path, fnames[csr]);
					strcat(path, "/");
				}
			}
			else
			{
				strcpy(path, dw.ExtractPath(path));
				strcat(path, fnames[csr]);
				return path;
			}
		}
	} while (key != SDLK_ESCAPE);

	return NULL;
}

void FileDialog(void)
{
	char *fname;

	disableSound = true;

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	fname = SelectFile(oldFileName[currentDrive]);
	SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);

	if (fname != NULL)
	{
		strcpy(oldFileName[currentDrive], fname);
		TryNLoadFile(oldFileName[currentDrive], currentDrive);
	}

	disableSound = false;
}

void FileDialogInit(void)
{
	char *str;

	if (config.GetString("root/Drives/A", &str)) strcpy(oldFileName[0], str);
	else strcpy(oldFileName[0], "/");
}

#define MAX_HEX_NUM_SZ 7

int DbgAskHexNum(const char *message)
{
	static char buffer[MAX_HEX_NUM_SZ + 1];
	char buf[0x100 + MAX_HEX_NUM_SZ];

	buffer[0] = 0;
	int bufferSz = 0;

	int key;
	SDL_Event event;

	int scrEnd = HEIGHT - fixed.Height() - 8;

	for (;;)
	{
		sprintf(buf, "%s: %s", message, buffer);

		Bar(0, scrEnd, WIDTH-1, HEIGHT-1, DRGB(0x80, 0x20, 0x20));
		fixed.PrintString(4, scrEnd+4, buf);
		ScaleImage();
		UpdateScreen();

		do
		{
			for (key = 0; SDL_PollEvent(&event);)
			{
				if (event.type == SDL_QUIT) exit(0);
				if (event.type == SDL_KEYDOWN) key = event.key.keysym.sym;
			}
		} while (key == 0);

		if (key == SDLK_ESCAPE)
		{
		    return -1;
		}
		else
		if (key == SDLK_RETURN)
		{
			if (!bufferSz) return -1;

			int res = 0;

			for (int i = 0; i < bufferSz; i++) {
				res = res * 0x10 + unhex(buffer[i]);
			}

			return res;
		}
		else
		if (key == SDLK_BACKSPACE)
		{
			if (bufferSz > 0) {
				buffer[--bufferSz] = 0;
			}
		}
		else
		if ((key>='0' && key<='9') || (key>='A' && key<='F') || (key>='a' && key<='f'))
		{
			if (bufferSz < MAX_HEX_NUM_SZ)
			{
				buffer[bufferSz++] = ((key>='a' && key<='f') ? (key - 'a' + 'A') : key);
				buffer[bufferSz] = 0;
			}
		}

		SDL_Delay(10);
	}
}

#define MAX_INSTR_SIZE 80

struct s_Instruction
{
	WORD addr;
	WORD size;
	char cmd[MAX_INSTR_SIZE];
};

bool IsWatched(WORD addr)
{
	for (unsigned i = 0; i < watchesCount; i++) {
		if (watches[i] == addr) {
			return true;
		}
	}

	return false;
}

const char * GetLabel(WORD addr)
{
	std::list<s_LabelItem>::iterator it;

	for (it = labels.begin(); it != labels.end(); it++) {
		if (it->addr == addr) {
			return it->label.c_str();
		}
	}

	return NULL;
}

const char * ReplaceLabels(const char * cmd)
{
	static char buf[0x100];
	char *p = buf;

	int l = strlen(cmd);

	while (l > 4)
	{
		if (cmd[0]=='#' && ishex(cmd[1]) && ishex(cmd[2]) && ishex(cmd[3]) && ishex(cmd[4]))
		{
			WORD addr = unhex(cmd[1]) * 0x1000 + unhex(cmd[2]) * 0x100 + unhex(cmd[3]) * 0x10 + unhex(cmd[4]);
			const char *lbl = GetLabel(addr);

			if (lbl)
			{
				cmd += 5;
				while (*lbl) *(p++) = *(lbl++);
			}

			break;
		}
		else
		{
			*(p++) = *(cmd++);
			l--;
		}
	}

	while (*cmd) *(p++) = *(cmd++);
	*p = 0;

	return buf;
}

bool wdDebug = false;
bool showLabels = false;

int restoreBpAddr = -1;
bool restoreBpVal = false;

void DebugIt(void)
{
	int key, keyx;
	SDL_Event event;

	int scrEnd = HEIGHT - fixed.Height() - 8;
	int h = fixed.Height();
	int mx = scrEnd / h;

	int t, t2;
	char buf[100];
	s_Instruction* dispBuf = new s_Instruction[mx];

	WORD curAddr = z80ex_get_reg(cpu, regPC);
	WORD userAddr = curAddr;
	bool correctAddr = true;
	int userPos;

	bool hexMode = false;
	bool exactAddr = false;

	if (restoreBpAddr >= 0)
	{
		breakpoints[(WORD)restoreBpAddr] = restoreBpVal;
		restoreBpAddr = -1;
	}

	do
	{
		int pos = (mx / 2) - 2;
		WORD addr = userAddr;

		while (pos >= -2)
		{
			WORD nxaddr = addr - 1;
			WORD instrSz = 1;

			for (WORD off = 8; off > 0; off--)
			{
				instrSz = z80ex_dasm(buf, MAX_INSTR_SIZE, 0, &t, &t2, ReadByteDasm, (WORD)(addr - off), NULL);

				if (instrSz == off)
				{
					nxaddr = addr - off;
					break;
				}
			}

			addr = nxaddr;
			pos--;
		}

		addr += z80ex_dasm(buf, MAX_INSTR_SIZE, 0, &t, &t2, ReadByteDasm, addr, NULL);
		addr += z80ex_dasm(buf, MAX_INSTR_SIZE, 0, &t, &t2, ReadByteDasm, addr, NULL);

		pos = 0;
		userPos = (mx / 2) - 1;

		while (pos < mx)
		{
			WORD instrSz = z80ex_dasm(dispBuf[pos].cmd, MAX_INSTR_SIZE, 0, &t, &t2, ReadByteDasm, addr, NULL);

			if (exactAddr && (addr < userAddr && (addr+instrSz) > userAddr))
			{
				strcpy(dispBuf[pos].cmd, "DB ");
				bool sep = false;

				for (WORD tmpAddr = addr; tmpAddr < userAddr ; tmpAddr++)
				{
					if (sep) strcat(dispBuf[pos].cmd, ",");

					sprintf(buf, "%02X", ReadByteDasm(tmpAddr, NULL));
					strcat(dispBuf[pos].cmd, buf);

					sep = true;
				}

				dispBuf[pos].addr = addr;
				dispBuf[pos].size = userAddr - addr;
				addr = userAddr;

				exactAddr = false;
			}
			else
			{
				dispBuf[pos].addr = addr;
				dispBuf[pos].size = instrSz;
				addr += instrSz;
			}

			pos++;
		}

		do
		{
			for (key = 0; SDL_PollEvent(&event);)
			{
				if (event.type == SDL_QUIT) exit(0);
				if (event.type == SDL_KEYDOWN) key = event.key.keysym.sym;
			}
 
	 		DlgClearScreen();
			Bar(0, scrEnd, WIDTH-1, HEIGHT-1, DRGB(0x80, 0x20, 0x20));

			int lx = WIDTH/2;

			for (int i = 0; i < mx; i++)
			{
				if ( (dispBuf[i].addr <= curAddr) && ((dispBuf[i].addr+dispBuf[i].size) > curAddr) ) {
					Bar(0, i*h, lx, i*h+h-1, DRGB(0x80, 0x20, 0x20));
				}

				if (dispBuf[i].addr == userAddr)
				{
					Bar(0, i*h, lx, i*h, DRGB(0xFF, 0xCC, 0x00));
					Bar(0, i*h+h-1, lx, i*h+h-1, DRGB(0xFF, 0xCC, 0x00));
					Bar(0, i*h+1, 0, i*h+h-2, DRGB(0xFF, 0xCC, 0x00));
					Bar(lx, i*h+1, lx, i*h+h-2, DRGB(0xFF, 0xCC, 0x00));

					userPos = i;
				}

				if (breakpoints[dispBuf[i].addr])
				{
					Bar(lx-5, i*h+h/2-2, lx-2, i*h+h/2+1, DRGB(0x00, 0x00, 0x00));
					Bar(lx-4, i*h+h/2-1, lx-3, i*h+h/2, DRGB(0xFF, 0x00, 0x00));
				}

				if (IsWatched(dispBuf[i].addr))
				{
					Bar(lx-10, i*h+h/2-2, lx-7, i*h+h/2+1, DRGB(0x00, 0x00, 0x00));
					Bar(lx-9, i*h+h/2-1, lx-8, i*h+h/2, DRGB(0x00, 0x00, 0xFF));
				}

				const char * lbl = (showLabels ? GetLabel(dispBuf[i].addr) : NULL);

				if (lbl)
				{
					fixed.PrintString(4, i*h, lbl);
				}
				else
				{
					sprintf(buf, "%04X", dispBuf[i].addr);
					fixed.PrintString(4, i*h, buf);
				}

				int xpos = 64;

				if (hexMode)
				{
					for (int j = 0; j < dispBuf[i].size; j++)
					{
						sprintf(buf, "%02X", ReadByteDasm(dispBuf[i].addr + j, NULL));
						fixed.PrintString(xpos, i*h, buf);
						xpos += 4*3;
					}
				}
				else
				{
					if (showLabels) {
						fixed.PrintString(xpos, i*h, ReplaceLabels(dispBuf[i].cmd));
					} else {
						fixed.PrintString(xpos, i*h, dispBuf[i].cmd);
					}
				}
			}

			int x = lx + 8;
			int y = 8;

			sprintf(buf, "AF   %04X", z80ex_get_reg(cpu, regAF));         fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "BC   %04X", z80ex_get_reg(cpu, regBC));         fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "DE   %04X", z80ex_get_reg(cpu, regDE));         fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "HL   %04X", z80ex_get_reg(cpu, regHL));         fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "IX   %04X", z80ex_get_reg(cpu, regIX));         fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "IY   %04X", z80ex_get_reg(cpu, regIY));         fixed.PrintString(x, y, buf); y += h;
			y += h;
			sprintf(buf, "AF'  %04X", z80ex_get_reg(cpu, regAF_));        fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "BC'  %04X", z80ex_get_reg(cpu, regBC_));        fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "DE'  %04X", z80ex_get_reg(cpu, regDE_));        fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "HL'  %04X", z80ex_get_reg(cpu, regHL_));        fixed.PrintString(x, y, buf); y += h;
			y += h;
			sprintf(buf, "SP   %04X", z80ex_get_reg(cpu, regSP));         fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "PC   %04X", z80ex_get_reg(cpu, regPC));         fixed.PrintString(x, y, buf); y += h;
			y += h;
			sprintf(buf, "I    %02X", z80ex_get_reg(cpu, regI));          fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "R    %02X", z80ex_get_reg(cpu, regR));          fixed.PrintString(x, y, buf); y += h;
			y += h;
			sprintf(buf, "IFF1 %02X", z80ex_get_reg(cpu, regIFF1));       fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "IFF2 %02X", z80ex_get_reg(cpu, regIFF2));       fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "IM   %02X", z80ex_get_reg(cpu, regIM));         fixed.PrintString(x, y, buf); y += h;
			y += h;
			sprintf(buf, "7FFD %02X", C_MemoryManager::port7FFD);         fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "EFF7 %02X", C_ExtPort::portEFF7);               fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "DOS  %s", C_TrDos::trdos ? "ON" : "OFF");       fixed.PrintString(x, y, buf); y += h;
			sprintf(buf, "INT? %s", z80ex_int_possible(cpu) ? "Y" : "N"); fixed.PrintString(x, y, buf); y += h;

			OutputGimpImage(WIDTH - img_zemuIco.width - 8, 8, (s_GimpImage *) &img_zemuIco);
			ScaleImage();
			UpdateScreen();
			SDL_Delay(10);
	 	} while (key == 0);

		if (key == SDLK_UP)
		{
			userAddr = dispBuf[userPos > 0 ? userPos-1 : 0].addr;
		}
		else
		if (key == SDLK_DOWN)
		{
			userAddr = dispBuf[userPos < mx-1 ? userPos+1 : mx-1].addr;
		}
		else
		if (key == SDLK_PAGEUP)
		{
			userAddr = dispBuf[0].addr;
		}
		else
		if (key == SDLK_PAGEDOWN)
		{
			userAddr = dispBuf[mx-2].addr;
		}
		else
		if (key == SDLK_F2)
		{
			breakpoints[userAddr] = !breakpoints[userAddr];
	    	exactAddr = true;
		}
		else
		if (key=='w' || key=='W')
		{
			if (IsWatched(userAddr))
			{
				unsigned l = 0;

				for (unsigned i = 0; i < watchesCount; i++)
				{
					if (l != i) watches[l] = watches[i];
					if (watches[l] != userAddr) l++;
				}

				watchesCount = l;
			}
			else if (watchesCount < MAX_WATCHES)
			{
				watches[watchesCount++] = userAddr;
			}

	    	exactAddr = true;
		}
		else
		if (key == SDLK_F4)
		{
			hexMode = !hexMode;
	    	exactAddr = true;
		}
		else
		if (key == SDLK_F3)
		{
			showLabels = !showLabels;
			exactAddr = true;
		}
		else
		if (key=='g' || key=='G')
		{
			int newAddr = DbgAskHexNum("GoTo address");

			if (newAddr>=0 && newAddr<=0xFFFF)
			{
				userAddr = newAddr;
				exactAddr = true;
			}
		}
		else
		if (key=='p' || key=='P')
		{
			sprintf(buf, "Poke to #%04X", userAddr);
			int val = DbgAskHexNum(buf);

			if (val>=0 && val<=0xFF) {
				WriteByteDasm(userAddr, (BYTE)val);
			}
		}
		else
		if (key == SDLK_F7)
		{
			DebugStep();
			curAddr = z80ex_get_reg(cpu, regPC);
			userAddr = curAddr;
			correctAddr = true;
		}
		else
		if (key == SDLK_F8)
		{
			addr = z80ex_get_reg(cpu, regPC);
			unsigned int maxCnt = 71680 / 4;

			do
			{
				DebugStep();
				maxCnt--;
			} while (z80ex_get_reg(cpu, regPC)==addr && maxCnt);

			curAddr = z80ex_get_reg(cpu, regPC);
			userAddr = curAddr;
			correctAddr = true;
		}
		else
		if (key == SDLK_F9)
		{
			restoreBpAddr = userAddr;
			restoreBpVal = breakpoints[userAddr];

			breakpoints[userAddr] = true;
			break;
		}
		else
		if (key == SDLK_F12)
		{
			wdDebug = !wdDebug;
			DlgConfirm(wdDebug ? "WD1793 debug enabled" : "WD1793 debug disabled");
			
		}

	} while (key != SDLK_ESCAPE);

	delete[] dispBuf;

	do
	{
		SDL_Delay(1);

		for (keyx = 0; SDL_PollEvent(&event);)
		{
			if (event.type == SDL_QUIT) exit(0);
			if (event.type == SDL_KEYUP) keyx = event.key.keysym.sym;
		}
	} while (key != keyx);
}

void RunDebugger(void)
{
	disableSound = true;
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	DebugIt();

	SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);
	disableSound = false;
}
