// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <string.h>
#include <ctype.h>
#include "dialog.h"
#include "font.h"
#include "graphics.h"
#include "lib_wd1793/wd1793_chip.h"
#include <z80ex_dasm.h>
#include "devs.h"
#include "tape/tape.h"
#include "labels.h"
#include "images/zemu_ico.h"
#include <boost/algorithm/string.hpp>

#define MAX_INPUT_STRING 64
#define MAX_HEX_NUM_SZ 7
#define MAX_INSTR_SIZE 80

extern C_Font* font;
extern C_Font* fixed_font;

std::string oldFileName[4] = {"", "", "" ,""};
int currentDrive = 0;
bool wdDebug = false;
bool showLabels = false;
int restoreBpAddr = -1;
bool restoreBpVal = false;

namespace {
    struct FileEntry {
        PathPtr path;
        bool isDirectory;
        std::string fileName;
        std::string compareName;

        friend bool operator<(const FileEntry& lhs, const FileEntry& rhs) {
            bool isLhsFile = !lhs.isDirectory;
            bool isRhsFile = !rhs.isDirectory;

            return std::tie(isLhsFile, lhs.compareName) < std::tie(isRhsFile, rhs.compareName);
        }
    };

    struct s_Instruction {
        uint16_t addr;
        uint16_t size;
        char cmd[MAX_INSTR_SIZE];
    };
}

void DlgClearScreen(void) {
    if (!ZHW_VIDEO_LOCKSURFACE(screen)) {
        return;
    }

    int* s = (int*)screen->pixels;

    for (int i = 0; i < HEIGHT; i++) {
        int* p = s;
        int cl = (i & 1) ? ZHW_VIDEO_MAKERGB(0,0,64) : ZHW_VIDEO_MAKERGB(0,0,32);

        for (int j = 0; j < WIDTH; j++) {
            *(p++) = cl;
        }

        s += PITCH;
    }

    ZHW_VIDEO_UNLOCKSURFACE(screen);
}

bool DlgConfirm(const char* message) {
    int key;
    ZHW_Event event;

    int wdt = font->StrLenPx(message) + 0x10;
    int hgt = font->Height() + 0x10;
    int x = (WIDTH - wdt) / 2;
    int y = (HEIGHT - hgt) / 2;

    DlgClearScreen();
    Bar(x, y, x + wdt - 1, y + hgt - 1, ZHW_VIDEO_MAKERGB(0x80, 0x20, 0x20));
    font->PrintString(x + 8, y + 8, message);

    for (;;) {
        UpdateScreen();

        for (key = 0; ZHW_Event_Poll(&event);) {
            if (event.type == ZHW_EVENT_QUIT) {
                exit(0);
            }

            if (event.type == ZHW_EVENT_KEYUP && ZHW_EVENT_OKKEY(window, event)) {
                key = event.key.keysym.sym;
            }
        }

        if (key == ZHW_KEY_y || key == ZHW_KEY_RETURN) {
            return true;
        }

        if (key == ZHW_KEY_n || key == ZHW_KEY_ESCAPE) {
            return false;
        }

        ZHW_Timer_Delay(10);
    }
}

const char* DlgInputString(const char* message) {
    static char buffer[MAX_INPUT_STRING + 1];
    char buf[0x100 + MAX_INPUT_STRING];

    buffer[0] = 0;
    int bufferSz = 0;

    int key;
    ZHW_Event event;

    for (;;) {
        sprintf(buf, "%s: %s", message, buffer);

        int wdt = font->StrLenPx(buf) + 0x10;
        int hgt = font->Height() + 0x10;
        int x = (WIDTH - wdt) / 2;
        int y = (HEIGHT - hgt) / 2;

        DlgClearScreen();
        Bar(x, y, x + wdt - 1, y + hgt - 1, ZHW_VIDEO_MAKERGB(0x80, 0x20, 0x20));
        font->PrintString(x + 8, y + 8, buf);
        UpdateScreen();

        do {
            for (key = 0; ZHW_Event_Poll(&event);) {
                if (event.type == ZHW_EVENT_QUIT) {
                    exit(0);
                }

                if (event.type == ZHW_EVENT_KEYDOWN && ZHW_EVENT_OKKEY(window, event)) {
                    key = event.key.keysym.sym;
                }
            }
        } while (key == 0);

        if (key == ZHW_KEY_ESCAPE) {
            buffer[0] = 0;
            return buffer;
        }

        if (key == ZHW_KEY_RETURN) {
            return buffer;
        }

        if (key == ZHW_KEY_BACKSPACE) {
            if (bufferSz > 0) {
                buffer[--bufferSz] = 0;
            }
        } else if (key >= 32 && key <= 127) {
            if (bufferSz < MAX_INPUT_STRING - 1) {
                buffer[bufferSz++] = key;
                buffer[bufferSz] = 0;
            }
        }

        ZHW_Timer_Delay(10);
    }
}

std::string SelectFile(std::string oldFileArg) {
    ZHW_Event event;
    std::vector<PathPtr> paths;
    std::vector<FileEntry> entries;
    char buf[0x100];
    int key;

    auto lastFileName = hostEnv->storage()->path(oldFileArg)->fileName();
    auto currentDirPath = hostEnv->storage()->path(oldFileArg)->parent()->canonical();

    int bottom = HEIGHT - font->Height() - 8;
    int lineHeight = font->Height();
    int maxLines = bottom / lineHeight;
    int padding = font->StrLenPx("[]");

    do
    {
        currentDirPath->listEntries(paths);
        entries.clear();

        for (auto& path : paths) {
            auto fileName = path->fileName();
            bool isDirectory = path->isDirectory();

            entries.push_back({std::move(path), isDirectory, fileName, boost::algorithm::to_lower_copy(fileName)});
        }

        int filesCnt = entries.size();
        int firstEntry = (currentDirPath->isRoot() ? 0 : 1); // skip ".."
        int lastEntry = std::min(maxLines, filesCnt);

        if (filesCnt > 1) {
            auto it = entries.begin();
            std::advance(it, firstEntry);
            std::sort(it, entries.end());
        }

        int pos = 0;
        int csr = 0;

        for (int i = firstEntry; i < filesCnt; i++) {
            if (lastFileName == entries[i].fileName) {
                csr = i;

                if (filesCnt > maxLines) {
                    pos = csr - maxLines / 2;

                    if (pos < 0) {
                        pos = 0;
                    }
                }

                break;
            }
        }

        if (lastEntry + pos - 1 >= filesCnt) {
            pos = filesCnt - lastEntry;

            if (pos < 0) {
                pos = 0;
            }
        }

        do {
            do {
                for (key = 0; ZHW_Event_Poll(&event);) {
                    if (event.type == ZHW_EVENT_QUIT) {
                        exit(0);
                    }

                    if (event.type == ZHW_EVENT_KEYDOWN && ZHW_EVENT_OKKEY(window, event)) {
                        key = event.key.keysym.sym;
                    }
                }

                DlgClearScreen();
                Bar(0, bottom, WIDTH - 1, HEIGHT - 1, ZHW_VIDEO_MAKERGB(0x80, 0x20, 0x20));

                Bar(4 + currentDrive * 16, bottom + 4, 4 + currentDrive * 16 + 12, bottom + 4 + 12, ZHW_VIDEO_MAKERGB(0x40, 0x10, 0x10));
                font->PrintString(5, bottom + 5, "A");
                font->PrintString(5 + 0x10 + 1, bottom + 5, "B");
                font->PrintString(5 + 0x20 + 1, bottom + 5, "C");
                font->PrintString(5 + 0x30 + 1, bottom + 5, "D");

                Bar(
                    4 + 0x50,
                    bottom + 4,
                    4 + 0x50 + 20,
                    bottom + 4 + 12,
                    wd1793_is_disk_wprotected(currentDrive) ? ZHW_VIDEO_MAKERGB(255, 128, 32) : ZHW_VIDEO_MAKERGB(0x40, 0x10, 0x10)
                );

                Bar(
                    4 + 0x68,
                    bottom + 4,
                    4 + 0x68 + 20,
                    bottom + 4 + 12,
                    wd1793_is_disk_loaded(currentDrive) ? ZHW_VIDEO_MAKERGB(255, 128, 32) : ZHW_VIDEO_MAKERGB(0x40, 0x10, 0x10)
                );

                Bar(
                    4 + 0x80,
                    bottom + 4,
                    4 + 0x80 + 20,
                    bottom + 4 + 12,
                    wd1793_is_disk_changed(currentDrive) ? ZHW_VIDEO_MAKERGB(255,128,32) : ZHW_VIDEO_MAKERGB(0x40,0x10,0x10)
                );

                font->PrintString(5 + 0x50, bottom + 5, "WP");
                font->PrintString(5 + 0x68, bottom + 5, "LD");
                font->PrintString(5 + 0x80, bottom + 5, "CH");

                Bar(
                    4 + 0xB0,
                    bottom + 4,
                    4 + 0xB0 + 72,
                    bottom + 4 + 12,
                    C_Tape::IsActive() ? ZHW_VIDEO_MAKERGB(255, 128, 32) : ZHW_VIDEO_MAKERGB(0x40, 0x10, 0x10)
                );

                if (C_Tape::IsLoaded()) {
                    sprintf(buf, "TAPE %u%%", C_Tape::GetPosPerc());
                } else {
                    sprintf(buf, "TAPE NOP");
                }

                font->PrintString(5 + 0xB0, bottom + 5, buf);

                for (int i = 0; i < lastEntry; i++) {
                    if (csr - pos == i) {
                        Bar(
                            0,
                            i * lineHeight,
                            padding + font->StrLenPx(entries[i + pos].fileName.c_str()) + padding,
                            i * lineHeight + lineHeight - 1,
                            ZHW_VIDEO_MAKERGB(0x80, 0x20, 0x20)
                        );
                    }

                    if (entries[i + pos].isDirectory) {
                        font->PrintString(0, i * lineHeight, "[]");
                    }

                    font->PrintString(padding, i * lineHeight, entries[i + pos].fileName.c_str());
                }

                OutputGimpImage(WIDTH - img_zemuIco.width - 8, 8, (s_GimpImage*)((void*) &img_zemuIco));
                UpdateScreen();
                ZHW_Timer_Delay(10);
            } while (key == 0);

            if (key == ZHW_KEY_s) {
                if (DlgConfirm("Are you sure to save disk? (Y/N)")) {
                    printf("Saving \"%s\" (drive %c) ... ", oldFileName[currentDrive].c_str(), "ABCD"[currentDrive]);

                    auto path = hostEnv->storage()->path(oldFileName[currentDrive]);

                    if (path->extensionLc() != "trd") {
                        path = path->concat(".trd");
                    }

                    if (!wd1793_save_dimage(path->string().c_str(), currentDrive, imgTRD)) {
                        printf("ERROR\n");
                        DlgConfirm("Save failed :(");
                    } else {
                        printf("OK\n");
                        oldFileName[currentDrive] = path->string();
                    }
                }
            } else if (key == ZHW_KEY_e) {
                wd1793_eject_dimage(currentDrive);
            } else if (key == ZHW_KEY_d) {
                C_Tape::Eject();
            } else if (key == ZHW_KEY_r) {
                C_Tape::Rewind();
            } else if (key == ZHW_KEY_t) {
                if (C_Tape::IsActive()) {
                    C_Tape::Stop();
                } else {
                    C_Tape::Start();
                }
            } else if (key == ZHW_KEY_w) {
                wd1793_set_disk_wprotected(currentDrive, !wd1793_is_disk_wprotected(currentDrive));
            } else if (key == ZHW_KEY_UP) {
                csr--;

                if (csr - pos < 0) {
                    if (pos > 0) {
                        pos--;
                    } else {
                        csr++;
                    }
                }
            } else if (key == ZHW_KEY_DOWN) {
                csr++;

                if (csr - pos >= lastEntry) {
                    if (pos + lastEntry < filesCnt) {
                        pos++;
                    } else {
                        csr--;
                    }
                }
            } else if (key == ZHW_KEY_HOME) {
                csr = 0;
                pos = 0;
            } else if (key == ZHW_KEY_END) {
                csr = filesCnt - 1;

                if (filesCnt > maxLines) {
                    pos = filesCnt - maxLines;
                } else {
                    pos = 0;
                }
            } else if (key == ZHW_KEY_PAGEUP) {
                pos -= maxLines;
                csr -= maxLines;

                if (pos < 0) {
                    pos = 0;
                    csr = 0;
                }
            } else if (key == ZHW_KEY_PAGEDOWN) {
                pos += maxLines;
                csr += maxLines;

                if (pos + lastEntry > filesCnt) {
                    if (filesCnt > maxLines) {
                        csr = filesCnt - 1;
                        pos = filesCnt - maxLines;
                    } else {
                        pos = 0;
                        csr = filesCnt - 1;
                    }
                }
            } else if (key == ZHW_KEY_LEFT) {
                currentDrive--;

                if (currentDrive < 0) {
                    currentDrive = 0;
                }
            } else if (key == ZHW_KEY_RIGHT) {
                currentDrive++;

                if (currentDrive > 3) {
                    currentDrive = 3;
                }
            }
        } while (key != ZHW_KEY_RETURN && key != ZHW_KEY_ESCAPE && key != ZHW_KEY_BACKSPACE);

        int keyx;

        do {
            ZHW_Timer_Delay(1);

            for (keyx = 0; ZHW_Event_Poll(&event);) {
                if (event.type == ZHW_EVENT_QUIT) {
                    exit(0);
                }

                if (event.type == ZHW_EVENT_KEYUP && ZHW_EVENT_OKKEY(window, event)) {
                    keyx = event.key.keysym.sym;
                }
            }
        } while (key != keyx);

        if (key == ZHW_KEY_BACKSPACE) {
            lastFileName = currentDirPath->fileName();
            currentDirPath = currentDirPath->parent();
        } else if (key == ZHW_KEY_RETURN) {
            if (!entries[csr].isDirectory) {
                return entries[csr].path->string();
            }

            if (entries[csr].fileName == "..") {
                lastFileName = currentDirPath->fileName();
                currentDirPath = currentDirPath->parent();
            } else {
                lastFileName.clear();
                currentDirPath = entries[csr].path->canonical();
            }
        }
    } while (key != ZHW_KEY_ESCAPE);

    return "";
}

void FileDialog(void) {
    disableSound = true;
    ZHW_Keyboard_EnableKeyRepeat(window);
    auto fname = SelectFile(oldFileName[currentDrive]);
    ZHW_Keyboard_DisableKeyRepeat(window);

    if (!fname.empty()) {
        TryNLoadFile(fname.c_str(), currentDrive);
    }

    disableSound = false;
}

void FileDialogInit(void) {
    auto str = hostEnv->config()->getString("beta128", "diskA", "/");
    oldFileName[0] = (str.empty() ? "/" : str);
}

int DbgAskHexNum(const char* message) {
    static char buffer[MAX_HEX_NUM_SZ + 1];
    char buf[0x100 + MAX_HEX_NUM_SZ];

    buffer[0] = 0;
    int bufferSz = 0;

    int key;
    ZHW_Event event;

    int scrEnd = HEIGHT - fixed_font->Height() - 8;

    for (;;) {
        sprintf(buf, "%s: %s", message, buffer);

        Bar(0, scrEnd, WIDTH - 1, HEIGHT - 1, ZHW_VIDEO_MAKERGB(0x80, 0x20, 0x20));
        fixed_font->PrintString(4, scrEnd + 4, buf);
        UpdateScreen();

        do {
            for (key = 0; ZHW_Event_Poll(&event);) {
                if (event.type == ZHW_EVENT_QUIT) {
                    exit(0);
                }

                if (event.type == ZHW_EVENT_KEYDOWN && ZHW_EVENT_OKKEY(window, event)) {
                    key = event.key.keysym.sym;
                }
            }
        } while (key == 0);

        if (key == ZHW_KEY_ESCAPE) {
            return -1;
        }

        if (key == ZHW_KEY_RETURN) {
            if (!bufferSz) {
                return -1;
            }

            int res = 0;

            for (int i = 0; i < bufferSz; i++) {
                res = res * 0x10 + unhex(buffer[i]);
            }

            return res;
        }

        if (key == ZHW_KEY_BACKSPACE) {
            if (bufferSz > 0) {
                buffer[--bufferSz] = 0;
            }
        } else if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'F') || (key >= 'a' && key <= 'f')) {
            if (bufferSz < MAX_HEX_NUM_SZ) {
                buffer[bufferSz++] = ((key >= 'a' && key <= 'f') ? (key - 'a' + 'A') : key); //-V560 for the great justice
                buffer[bufferSz] = 0;
            }
        }

        ZHW_Timer_Delay(10);
    }
}

bool IsWatched(uint16_t addr) {
    for (unsigned i = 0; i < watchesCount; i++) {
        if (watches[i] == addr) {
            return true;
        }
    }

    return false;
}

const char* GetLabel(uint16_t addr) {
    auto it = labels.find(addr);
    return (it == labels.end() ? nullptr : it->second.c_str());
}

const char* ReplaceLabels(const char* cmd) {
    static char buf[0x100];

    char* p = buf;
    int l = strlen(cmd);

    while (l > 4) {
        if (cmd[0] == '#' && ishex(cmd[1]) && ishex(cmd[2]) && ishex(cmd[3]) && ishex(cmd[4])) {
            uint16_t addr = unhex(cmd[1]) * 0x1000 + unhex(cmd[2]) * 0x100 + unhex(cmd[3]) * 0x10 + unhex(cmd[4]);
            const char* lbl = GetLabel(addr);

            if (lbl) {
                cmd += 5;

                while (*lbl) {
                    *(p++) = *(lbl++);
                }
            }

            break;
        } else {
            *(p++) = *(cmd++);
            l--;
        }
    }

    while (*cmd) {
        *(p++) = *(cmd++);
    }

    *p = 0;
    return buf;
}

void DebugIt(void) {
    int key;
    int keyx;
    ZHW_Event event;

    int scrEnd = HEIGHT - fixed_font->Height() - 8;
    int h = fixed_font->Height();
    int mx = scrEnd / h;

    int t;
    int t2;
    char buf[100];
    s_Instruction* dispBuf = new s_Instruction[mx];

    uint16_t curAddr = z80ex_get_reg(cpu, regPC);
    uint16_t userAddr = curAddr;
    bool correctAddr = true;
    int userPos;

    bool hexMode = false;
    bool exactAddr = false;

    if (restoreBpAddr >= 0) {
        breakpoints[(uint16_t)restoreBpAddr] = restoreBpVal;
        restoreBpAddr = -1;
    }

    do {
        int pos = (mx / 2) - 2;
        uint16_t addr = userAddr;

        while (pos >= -2) {
            uint16_t nxaddr = addr - 1;
            uint16_t instrSz = 1;

            for (uint16_t off = 8; off > 0; off--) {
                instrSz = z80ex_dasm(buf, MAX_INSTR_SIZE, 0, &t, &t2, ReadByteDasm, (uint16_t)(addr - off), nullptr);

                if (instrSz == off) {
                    nxaddr = addr - off;
                    break;
                }
            }

            addr = nxaddr;
            pos--;
        }

        addr += z80ex_dasm(buf, MAX_INSTR_SIZE, 0, &t, &t2, ReadByteDasm, addr, nullptr);
        addr += z80ex_dasm(buf, MAX_INSTR_SIZE, 0, &t, &t2, ReadByteDasm, addr, nullptr);

        pos = 0;
        userPos = (mx / 2) - 1;

        while (pos < mx) {
            uint16_t instrSz = z80ex_dasm(dispBuf[pos].cmd, MAX_INSTR_SIZE, 0, &t, &t2, ReadByteDasm, addr, nullptr);

            if (exactAddr && (addr < userAddr && (addr+instrSz) > userAddr)) {
                strcpy(dispBuf[pos].cmd, "DB ");
                bool sep = false;

                for (uint16_t tmpAddr = addr; tmpAddr < userAddr ; tmpAddr++) {
                    if (sep) {
                        strcat(dispBuf[pos].cmd, ",");
                    }

                    sprintf(buf, "%02X", ReadByteDasm(tmpAddr, nullptr));
                    strcat(dispBuf[pos].cmd, buf); //-V512

                    sep = true;
                }

                dispBuf[pos].addr = addr;
                dispBuf[pos].size = userAddr - addr;
                addr = userAddr;

                exactAddr = false;
            } else {
                dispBuf[pos].addr = addr;
                dispBuf[pos].size = instrSz;
                addr += instrSz;
            }

            pos++;
        }

        do {
            for (key = 0; ZHW_Event_Poll(&event);) {
                if (event.type == ZHW_EVENT_QUIT) {
                    exit(0);
                }

                if (event.type == ZHW_EVENT_KEYDOWN && ZHW_EVENT_OKKEY(window, event)) {
                    key = event.key.keysym.sym;
                }
            }

            DlgClearScreen();
            Bar(0, scrEnd, WIDTH - 1, HEIGHT - 1, ZHW_VIDEO_MAKERGB(0x80, 0x20, 0x20));

            int lx = WIDTH / 2;

            for (int i = 0; i < mx; i++) {
                if ((dispBuf[i].addr <= curAddr) && ((dispBuf[i].addr + dispBuf[i].size) > curAddr)) {
                    Bar(0, i * h, lx, i * h + h - 1, ZHW_VIDEO_MAKERGB(0x80, 0x20, 0x20));
                }

                if (dispBuf[i].addr == userAddr) {
                    Bar(0, i * h, lx, i * h, ZHW_VIDEO_MAKERGB(0xFF, 0xCC, 0x00));
                    Bar(0, i * h + h - 1, lx, i * h + h - 1, ZHW_VIDEO_MAKERGB(0xFF, 0xCC, 0x00));
                    Bar(0, i * h + 1, 0, i * h + h - 2, ZHW_VIDEO_MAKERGB(0xFF, 0xCC, 0x00));
                    Bar(lx, i * h + 1, lx, i * h + h - 2, ZHW_VIDEO_MAKERGB(0xFF, 0xCC, 0x00));

                    userPos = i;
                }

                if (breakpoints[dispBuf[i].addr]) {
                    Bar(lx - 5, i * h + h / 2 - 2, lx - 2, i * h + h / 2 + 1, ZHW_VIDEO_MAKERGB(0x00, 0x00, 0x00));
                    Bar(lx - 4, i * h + h / 2 - 1, lx - 3, i * h + h / 2, ZHW_VIDEO_MAKERGB(0xFF, 0x00, 0x00));
                }

                if (IsWatched(dispBuf[i].addr)) {
                    Bar(lx - 10, i * h + h / 2 - 2, lx - 7, i * h + h / 2 + 1, ZHW_VIDEO_MAKERGB(0x00, 0x00, 0x00));
                    Bar(lx - 9, i * h + h / 2 - 1, lx - 8, i * h + h / 2, ZHW_VIDEO_MAKERGB(0x00, 0x00, 0xFF));
                }

                const char* lbl = (showLabels ? GetLabel(dispBuf[i].addr) : nullptr);

                if (lbl) {
                    fixed_font->PrintString(4, i * h, lbl);
                } else {
                    sprintf(buf, "%04X", dispBuf[i].addr);
                    fixed_font->PrintString(4, i * h, buf);
                }

                int xpos = 64;

                if (hexMode) {
                    for (int j = 0; j < dispBuf[i].size; j++) {
                        sprintf(buf, "%02X", ReadByteDasm(dispBuf[i].addr + j, nullptr));
                        fixed_font->PrintString(xpos, i*h, buf);
                        xpos += 4*3;
                    }
                } else if (showLabels) {
                    fixed_font->PrintString(xpos, i * h, ReplaceLabels(dispBuf[i].cmd));
                } else {
                    fixed_font->PrintString(xpos, i * h, dispBuf[i].cmd);
                }
            }

            int x = lx + 8;
            int y = 8;

            sprintf(buf, "AF   %04X", z80ex_get_reg(cpu, regAF));         fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "BC   %04X", z80ex_get_reg(cpu, regBC));         fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "DE   %04X", z80ex_get_reg(cpu, regDE));         fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "HL   %04X", z80ex_get_reg(cpu, regHL));         fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "IX   %04X", z80ex_get_reg(cpu, regIX));         fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "IY   %04X", z80ex_get_reg(cpu, regIY));         fixed_font->PrintString(x, y, buf); y += h;
            y += h;
            sprintf(buf, "AF'  %04X", z80ex_get_reg(cpu, regAF_));        fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "BC'  %04X", z80ex_get_reg(cpu, regBC_));        fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "DE'  %04X", z80ex_get_reg(cpu, regDE_));        fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "HL'  %04X", z80ex_get_reg(cpu, regHL_));        fixed_font->PrintString(x, y, buf); y += h;
            y += h;
            sprintf(buf, "SP   %04X", z80ex_get_reg(cpu, regSP));         fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "PC   %04X", z80ex_get_reg(cpu, regPC));         fixed_font->PrintString(x, y, buf); y += h;
            y += h;
            sprintf(buf, "I    %02X", z80ex_get_reg(cpu, regI));          fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "R    %02X", z80ex_get_reg(cpu, regR));          fixed_font->PrintString(x, y, buf); y += h;
            y += h;
            sprintf(buf, "IFF1 %02X", z80ex_get_reg(cpu, regIFF1));       fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "IFF2 %02X", z80ex_get_reg(cpu, regIFF2));       fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "IM   %02X", z80ex_get_reg(cpu, regIM));         fixed_font->PrintString(x, y, buf); y += h;
            y += h;
            sprintf(buf, "7FFD %02X", C_MemoryManager::port7FFD);         fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "EFF7 %02X", C_ExtPort::portEFF7);               fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "DOS  %s", C_TrDos::trdos ? "ON" : "OFF");       fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "INT? %s", z80ex_int_possible(cpu) ? "Y" : "N"); fixed_font->PrintString(x, y, buf); y += h;
            sprintf(buf, "T    %d", (int)cpuClk);                         fixed_font->PrintString(x, y, buf); y += h;

            OutputGimpImage(WIDTH - img_zemuIco.width - 8, 8, (s_GimpImage*)((void*) &img_zemuIco));
            UpdateScreen();
            ZHW_Timer_Delay(10);
        } while (key == 0);

        if (key == ZHW_KEY_UP) {
            userAddr = dispBuf[userPos > 0 ? userPos - 1 : 0].addr;
        } else if (key == ZHW_KEY_DOWN) {
            userAddr = dispBuf[userPos < mx - 1 ? userPos + 1 : mx - 1].addr;
        } else if (key == ZHW_KEY_PAGEUP) {
            userAddr = dispBuf[0].addr;
        } else if (key == ZHW_KEY_PAGEDOWN) {
            userAddr = dispBuf[mx - 2].addr;
        } else if (key == ZHW_KEY_F2) {
            breakpoints[userAddr] = !breakpoints[userAddr];
            exactAddr = true;
        } else if (key == ZHW_KEY_w) {
            if (IsWatched(userAddr)) {
                unsigned l = 0;

                for (unsigned i = 0; i < watchesCount; i++) {
                    if (l != i) {
                        watches[l] = watches[i];
                    }

                    if (watches[l] != userAddr) {
                        l++;
                    }
                }

                watchesCount = l;
            } else if (watchesCount < MAX_WATCHES) {
                watches[watchesCount++] = userAddr;
            }

            exactAddr = true;
        } else if (key == ZHW_KEY_F4) {
            hexMode = !hexMode;
            exactAddr = true;
        } else if (key == ZHW_KEY_F3) {
            showLabels = !showLabels;
            exactAddr = true;
        } else if (key == ZHW_KEY_g) {
            int newAddr = DbgAskHexNum("GoTo address");

            if (newAddr >= 0 && newAddr <= 0xFFFF) {
                userAddr = newAddr;
                exactAddr = true;
            }
        } else if (key == ZHW_KEY_p) {
            sprintf(buf, "Poke to #%04X", userAddr);
            int val = DbgAskHexNum(buf);

            if (val >= 0 && val <= 0xFF) {
                WriteByteDasm(userAddr, (uint8_t)val);
            }
        } else if (key == ZHW_KEY_F7) {
            DebugStep();
            curAddr = z80ex_get_reg(cpu, regPC);
            userAddr = curAddr;
            correctAddr = true;
        } else if (key == ZHW_KEY_F8) {
            addr = z80ex_get_reg(cpu, regPC);
            unsigned int maxCnt = 71680 / 4;

            do {
                DebugStep();
                maxCnt--;
            } while (z80ex_get_reg(cpu, regPC) == addr && maxCnt);

            curAddr = z80ex_get_reg(cpu, regPC);
            userAddr = curAddr;
            correctAddr = true;
        } else if (key == ZHW_KEY_F9) {
            restoreBpAddr = userAddr;
            restoreBpVal = breakpoints[userAddr];

            breakpoints[userAddr] = true;
            break;
        } else if (key == ZHW_KEY_F12) {
            wdDebug = !wdDebug;
            DlgConfirm(wdDebug ? "WD1793 debug enabled" : "WD1793 debug disabled");
        }
    } while (key != ZHW_KEY_ESCAPE);

    delete[] dispBuf;

    do {
        ZHW_Timer_Delay(1);

        for (keyx = 0; ZHW_Event_Poll(&event);) {
            if (event.type == ZHW_EVENT_QUIT) {
                exit(0);
            }

            if (event.type == ZHW_EVENT_KEYUP && ZHW_EVENT_OKKEY(window, event)) {
                keyx = event.key.keysym.sym;
            }
        }
    } while (key != keyx);
}

void RunDebugger(void) {
    disableSound = true;
    ZHW_Keyboard_EnableKeyRepeat(window);

    DebugIt();

    ZHW_Keyboard_DisableKeyRepeat(window);
    disableSound = false;
}
