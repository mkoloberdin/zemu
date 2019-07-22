// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdexcept>
#include <string.h>
#include <ctype.h>
#include <cctype>
#include <algorithm>
#include <boost/format.hpp>
#include <list>
#include <cmath>
#include "zemu_env.h"
#include "zemu.h"
#include "lib_wd1793/wd1793_chip.h"
#include "font.h"
#include "dialog.h"
#include "graphics.h"
#include "cpu_trace.h"
#include "tape/tape.h"
#include "labels.h"
#include "renderer/render_speccy.h"
#include "renderer/render_16c.h"
#include "renderer/render_multicolor.h"
#include "devs.h"
#include "snap_z80.h"
#include "snap_sna.h"
#include "data/font_bold.h"
#include "data/font_thin.h"
#include "data/img_floppy.h"
#include "data/img_floppy_read.h"
#include "data/img_floppy_write.h"
#include "data/img_turbo_off.h"
#include "data/img_turbo_on.h"
#include "host_impl/host_impl.h"

#ifdef _WIN32
    #include "windows/resource.h"
#endif

#define SNAP_FORMAT_Z80 0
#define SNAP_FORMAT_SNA 1
#define INT_LENGTH 32

Host* host;
unsigned turboMultiplier = 1;
unsigned turboMultiplierNx = 1;
bool unturbo = false;
bool unturboNx = false;
bool isPaused = false;
bool isPausedNx = false;
bool joyOnKeyb = false;
Z80EX_CONTEXT* cpu;
uint64_t cpuClk;
uint64_t devClk;
uint64_t lastDevClk;
uint64_t devClkCounter;
s_Params params;
bool drawFrame;
int frames;
C_Font* font = nullptr;
C_Font* fixed_font = nullptr;
bool doCopyOfSurfaces = false;
bool recordWav = false;
const char* wavFileName = "output.wav"; // TODO: make configurable + full filepath
int attributesHack = 0;
bool flashColor = false;
int screensHack = 0;
uint64_t actDevClkCounter = 0;
uint64_t actClk = 0;
int (* DoCpuStep)(Z80EX_CONTEXT* cpu) = z80ex_step;
int (* DoCpuInt)(Z80EX_CONTEXT* cpu) = z80ex_int;
unsigned long prevRenderClk;
void (* renderPtr)(unsigned long) = nullptr;

uint32_t* screen;
uint32_t* renderScreen;
uint32_t* renderScreenBuffer[2];

//--------------------------------------------------------------------------------------------------------------

ptrOnReadByteFunc* devMapRead;
bool (** devMapWrite)(uint16_t, uint8_t);
bool (** devMapInput)(uint16_t, uint8_t&);
bool (** devMapOutput)(uint16_t, uint8_t);

ptrOnReadByteFunc devMapRead_base[0x20000];
bool (* devMapWrite_base[0x10000])(uint16_t, uint8_t);
bool (* devMapInput_base[0x10000])(uint16_t, uint8_t&);
bool (* devMapOutput_base[0x10000])(uint16_t, uint8_t);

ptrOnReadByteFunc devMapRead_trdos[0x20000];
bool (* devMapInput_trdos[0x10000])(uint16_t, uint8_t&);
bool (* devMapOutput_trdos[0x10000])(uint16_t, uint8_t);

//--------------------------------------------------------------------------------------------------------------

struct s_ReadItem {
    ptrOnReadByteFunc (* check)(uint16_t, bool);
    // bool (* func)(uint16_t, bool, uint8_t&);
};

struct s_WriteItem {
    bool (* check)(uint16_t);
    bool (* func)(uint16_t, uint8_t);
};

struct s_InputItem {
    bool (* check)(uint16_t);
    bool (* func)(uint16_t, uint8_t&);
};

typedef s_WriteItem s_OutputItem;

struct s_HwItem {
    StageEventType eventType;
    bool (* func)(StageEvent&);
};

//--------------------------------------------------------------------------------------------------------------

s_ReadItem hnd_z80read[MAX_HANDLERS];
s_WriteItem hnd_z80write[MAX_HANDLERS];
s_InputItem hnd_z80input[MAX_HANDLERS];
s_OutputItem hnd_z80output[MAX_HANDLERS];
void (* hnd_frameStart[MAX_HANDLERS])(void);
void (* hnd_afterFrameRender[MAX_HANDLERS])(void);
s_HwItem hnd_hw[MAX_HANDLERS];
void (* hnd_reset[MAX_HANDLERS])(void);

int cnt_z80read = 0;
int cnt_z80write = 0;
int cnt_z80input = 0;
int cnt_z80output = 0;
int cnt_frameStart = 0;
int cnt_afterFrameRender = 0;
int cnt_hw = 0;
int cnt_reset = 0;

void AttachZ80ReadHandler(ptrOnReadByteFunc (* check)(uint16_t, bool)) {
    if (cnt_z80read >= MAX_HANDLERS) {
        StrikeError("Increase MAX_HANDLERS");
    }

    s_ReadItem item;
    item.check = check;

    hnd_z80read[cnt_z80read++] = item;
}

void AttachZ80WriteHandler(bool (* check)(uint16_t), bool (* func)(uint16_t, uint8_t)) {
    if (cnt_z80write >= MAX_HANDLERS) {
        StrikeError("Increase MAX_HANDLERS");
    }

    s_WriteItem item;
    item.check = check;
    item.func = func;

    hnd_z80write[cnt_z80write++] = item;
}

void AttachZ80InputHandler(bool (* check)(uint16_t), bool (* func)(uint16_t, uint8_t&)) {
    if (cnt_z80input >= MAX_HANDLERS) {
        StrikeError("Increase MAX_HANDLERS");
    }

    s_InputItem item;
    item.check = check;
    item.func = func;

    hnd_z80input[cnt_z80input++] = item;
}

void AttachZ80OutputHandler(bool (* check)(uint16_t), bool (* func)(uint16_t, uint8_t)) {
    if (cnt_z80output >= MAX_HANDLERS) {
        StrikeError("Increase MAX_HANDLERS");
    }

    s_OutputItem item;
    item.check = check;
    item.func = func;

    hnd_z80output[cnt_z80output++] = item;
}

void AttachFrameStartHandler(void (* func)(void)) {
    if (cnt_frameStart >= MAX_HANDLERS) {
        StrikeError("Increase MAX_HANDLERS");
    }

    hnd_frameStart[cnt_frameStart++] = func;
}

void AttachAfterFrameRenderHandler(void (* func)(void)) {
    if (cnt_afterFrameRender >= MAX_HANDLERS) {
        StrikeError("Increase MAX_HANDLERS");
    }

    hnd_afterFrameRender[cnt_afterFrameRender++] = func;
}

void AttachHwHandler(StageEventType eventType, bool (* func)(StageEvent&)) {
    if (cnt_hw >= MAX_HANDLERS) {
        StrikeError("Increase MAX_HANDLERS");
    }

    s_HwItem item;
    item.eventType = eventType;
    item.func = func;

    hnd_hw[cnt_hw++] = item;
}

void AttachResetHandler(void (* func)(void)) {
    if (cnt_reset >= MAX_HANDLERS) {
        StrikeError("Increase MAX_HANDLERS");
    }

    hnd_reset[cnt_reset++] = func;
}

//--------------------------------------------------------------------------------------------------------------

C_Border dev_border;
C_ExtPort dev_extport;
C_Keyboard dev_keyboard;
C_TrDos dev_trdos;
C_MemoryManager dev_mman;
C_TsFm dev_tsfm;
C_Mouse dev_mouse;
C_Covox dev_covox;
C_KempstonStick dev_kempston;
C_GSound dev_gsound;

C_Device* devs[] = {
    &dev_border,
    &dev_extport,
    &dev_keyboard,
    &dev_trdos,
    &dev_mman,
    &dev_tsfm,
    &dev_mouse,
    &dev_covox,
    &dev_kempston,
    &dev_gsound,
    nullptr
};

int colors_base[0x10] = {
    STAGE_MAKERGB(  0,   0,   0),
    STAGE_MAKERGB(  0,   0, 192),
    STAGE_MAKERGB(192,   0,   0),
    STAGE_MAKERGB(192,   0, 192),
    STAGE_MAKERGB(  0, 192,   0),
    STAGE_MAKERGB(  0, 192, 192),
    STAGE_MAKERGB(192, 192,   0),
    STAGE_MAKERGB(192, 192, 192),
    STAGE_MAKERGB(  0,   0,   0),
    STAGE_MAKERGB(  0,   0, 255),
    STAGE_MAKERGB(255,   0,   0),
    STAGE_MAKERGB(255,   0, 255),
    STAGE_MAKERGB(  0, 255,   0),
    STAGE_MAKERGB(  0, 255, 255),
    STAGE_MAKERGB(255, 255,   0),
    STAGE_MAKERGB(255, 255, 255)
};

/*
 * Work in progress
 *
int c64_colors_base[0x10] = {
    STAGE_MAKERGB(0x00, 0x00, 0x00),
    STAGE_MAKERGB(0x35, 0x28, 0x79),
    STAGE_MAKERGB(0x9A, 0x67, 0x59),
    STAGE_MAKERGB(0x6F, 0x3D, 0x86),
    STAGE_MAKERGB(0x58, 0x8D, 0x43),
    STAGE_MAKERGB(0x70, 0xA4, 0xB2),
    STAGE_MAKERGB(0xB8, 0xC7, 0x6F),
    STAGE_MAKERGB(0x95, 0x95, 0x95),

    STAGE_MAKERGB(0x00, 0x00, 0x00),
    STAGE_MAKERGB(0x35, 0x28, 0x79),
    STAGE_MAKERGB(0x9A, 0x67, 0x59),
    STAGE_MAKERGB(0x6F, 0x3D, 0x86),
    STAGE_MAKERGB(0x58, 0x8D, 0x43),
    STAGE_MAKERGB(0x70, 0xA4, 0xB2),
    STAGE_MAKERGB(0xB8, 0xC7, 0x6F),
    STAGE_MAKERGB(0xFF, 0xFF, 0xFF)
};
*/

int colors[0x10];

//--------------------------------------------------------------------------------------------------------------

int messageTimeout = 0;
char message[0x100];

void OutputText(char* str) {
    int x = (WIDTH - font->StrLenPx(str)) / 2;
    int y = HEIGHT - font->Height() - 4;

    font->PrintString(x, y, str);
}

void ShowMessage(void) {
    if (messageTimeout <= 0) {
        return;
    }

    messageTimeout--;
    OutputText(message);
}

void SetMessage(const char* str) {
    strcpy(message, str);
    messageTimeout = 50;
}

//--------------------------------------------------------------------------------------------------------------

void ResetSequence(void);

void StrToLower(char* str) {
    while (*str) {
        *(str) = tolower(*str);
        str++;
    }
}

void LoadNormalFile(const char* fname, int drive, const char* arcName = nullptr) {
    if (C_Tape::IsTapeFormat(fname)) {
        try {
            C_Tape::Insert(fname);
        } catch (StorageException& e) {
            printf("Load failed: %s\n", e.what());
        }

        return;
    }

    auto ext = host->storage()->path(fname)->extensionLc();

    if (ext == ".z80") {
        if (load_z80_snap(fname, cpu, dev_mman, dev_border)) {
            dev_tsfm.OnReset();
        } else {
            StrikeMessage("Error loading snapshot");
        }

        return;
    }

    if (ext == ".sna") {
        if (load_sna_snap(fname, cpu, dev_mman, dev_border)) {
            dev_tsfm.OnReset();
        } else {
            StrikeMessage("Error loading snapshot");
        }

        return;
    }

    try {
        wd1793_load_dimage(fname, drive);
    } catch (StorageException& e) {
        printf("Load failed: %s\n", e.what());
    }

    oldFileName[drive] = host->storage()->path(arcName ? arcName : fname)->canonical()->string();
}

void TryNLoadFile(const char* fname, int drive) {
    char bname[MAX_PATH];
    strcpy(bname, fname);

    char* tname = bname;

    if (tname[0] == '"') {
        tname++;
        int len = strlen(tname);

        if (tname[len - 1] == '"') {
            tname[len - 1] = '\0';
        }
    }

    #ifdef _WIN32
        if (tname[0]=='/' && tname[2]=='/')
        {
            tname[0] = tname[1];
            tname[1] = ':';
        }
    #endif

    if (tname[0] != 0) {
        printf("Trying to load \"%s\" ...\n", tname);
        LoadNormalFile(tname, drive);
    }
}

//--------------------------------------------------------------------------------------------------------------

void Action_Reset(void) {
    isPaused = false;
    ResetSequence();
    dev_gsound.Reset();
}

void Action_ResetTrDos(void) {
    isPaused = false;
    ResetSequence();
    dev_mman.OnOutputByte(0x7FFD, 0x10);
    dev_trdos.Enable();
}

void Action_MaxSpeed(void) {
    isPaused = false;
    params.maxSpeed = !params.maxSpeed;
    SetMessage(params.maxSpeed ? "MaxSpeed ON" : "MaxSpeed OFF");
}

void Action_QuickLoad(void) {
    bool (* loadSnap)(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border);

    isPaused = false;

    const char* snapName = (params.snapFormat == SNAP_FORMAT_SNA ? "snap.sna" : "snap.z80");
    loadSnap = (params.snapFormat == SNAP_FORMAT_SNA ? load_sna_snap : load_z80_snap);

    if (!loadSnap(snapName, cpu, dev_mman, dev_border)) {
        SetMessage("Error loading snapshot");
    } else {
        SetMessage("Snapshot loaded");
    }
}

void Action_QuickSave(void) {
    bool (* saveSnap)(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border);

    isPaused = false;

    const char* snapName = (params.snapFormat == SNAP_FORMAT_SNA ? "snap.sna" : "snap.z80");
    saveSnap = (params.snapFormat == SNAP_FORMAT_SNA ? save_sna_snap : save_z80_snap);

    if (!saveSnap(snapName, cpu, dev_mman, dev_border)) {
        SetMessage("Error saving snapshot");
    } else {
        SetMessage("Snapshot saved");
    }
}

void Action_AntiFlicker(void) {
    isPaused = false;
    params.antiFlicker = !params.antiFlicker;

    if (params.antiFlicker) {
        doCopyOfSurfaces = true;
    }

    SetMessage(params.antiFlicker ? "AntiFlicker ON" : "AntiFlicker OFF");
}

void Action_LoadFile(void) {
    FileDialog();
}

void Action_Fullscreen(void) {
    host->stage()->setFullscreen(!host->stage()->isFullscreen());
}

void Action_Debugger(void) {
    isPaused = false;
    RunDebugger();
}

void Action_Turbo(void) {
    isPaused = false;

    if (unturboNx && turboMultiplierNx > 1) {
        turboMultiplierNx /= 2;
        unturboNx = (turboMultiplierNx > 1);
    } else {
        unturboNx = false;
        turboMultiplierNx = (turboMultiplierNx == 8 ? 1 : (turboMultiplierNx * 2));
    }

    DisplayTurboMessage();
}

void Action_UnTurbo(void) {
    isPaused = false;

    if (!unturboNx && turboMultiplierNx > 1) {
        turboMultiplierNx /= 2;
    } else {
        turboMultiplierNx = (turboMultiplierNx == 256 ? 1 : (turboMultiplierNx * 2));
        unturboNx = (turboMultiplierNx > 1);
    }

    DisplayTurboMessage();
}

void DisplayTurboMessage(void) {
    if (turboMultiplierNx < 2) {
        SetMessage("Turbo OFF");
    } else {
        char buf[0x10];
        sprintf(buf, (unturboNx ? "Slowness %dx" : "Turbo %dx"), turboMultiplierNx);
        SetMessage(buf);
    }
}

void Action_AttributesHack(void) {
    isPaused = false;
    attributesHack = (attributesHack + 1) % 3;

    if (attributesHack) {
        char buf[0x20];
        sprintf(buf, "AttributesHack #%d", attributesHack);
        SetMessage(buf);
    } else {
        SetMessage("AttributesHack OFF");
    }
}

void Action_ScreensHack(void) {
    isPaused = false;
    screensHack = screensHack ^ 8;
    SetMessage(screensHack ? "ScreensHack ON" : "ScreensHack OFF");
}

void Action_FlashColor(void) {
    isPaused = false;
    flashColor = !flashColor;
    SetMessage(flashColor ? "FlashColor ON" : "FlashColor OFF");
}

void Action_Pause(void) {
    isPausedNx = !isPausedNx;
    SetMessage(isPausedNx ? "Pause ON" : "Pause OFF");
}

void Action_JoyOnKeyb(void) {
    isPaused = false;
    joyOnKeyb = !joyOnKeyb;
    dev_kempston.joyOnKeybState = 0;
    SetMessage(joyOnKeyb ? "Kempston on keyboard ON" : "Kempston on keyboard OFF");
}


s_Action cfgActions[] = {
    {"reset",           Action_Reset},
    {"reset_trdos",     Action_ResetTrDos},
    {"max_speed",       Action_MaxSpeed},
    {"quick_load",      Action_QuickLoad},
    {"quick_save",      Action_QuickSave},
    {"anti_flicker",    Action_AntiFlicker},
    {"load_file",       Action_LoadFile},
    {"fullscreen",      Action_Fullscreen},
    {"debugger",        Action_Debugger},
    {"turbo",           Action_Turbo},
    {"unturbo",         Action_UnTurbo},
    {"attrib_hack",     Action_AttributesHack},
    {"screens_hack",    Action_ScreensHack},
    {"flash_color",     Action_FlashColor},
    {"pause",           Action_Pause},
    {"joy_on_keyb",     Action_JoyOnKeyb},
    {"",                nullptr}
};

//--------------------------------------------------------------------------------------------------------------

bool runDebuggerFlag = false;
bool breakpoints[0x10000];

uint16_t watches[MAX_WATCHES];
unsigned watchesCount = 0;

uint8_t ReadByteDasm(uint16_t addr, void* userData) {
    ptrOnReadByteFunc func = devMapRead[addr];
    return func(addr, false);
}

void WriteByteDasm(uint16_t addr, uint8_t value) {
    for (;;) {
        bool (* func)(uint16_t, uint8_t) = devMapWrite[addr];

        if (!func || func(addr, value)) {
            return;
        }
    }
}

uint8_t ReadByte(Z80EX_CONTEXT_PARAM uint16_t addr, int m1_state, void* userData) {
    unsigned raddr = addr + (m1_state ? 0x10000 : 0);
    ptrOnReadByteFunc func = devMapRead[raddr];
    return func(addr, m1_state);
}

void WriteByte(Z80EX_CONTEXT_PARAM uint16_t addr, uint8_t value, void* userData) {
    for (;;) {
        bool (* func)(uint16_t, uint8_t) = devMapWrite[addr];

        if (!func || func(addr, value)) {
            return;
        }
    }
}

uint8_t InputByte(Z80EX_CONTEXT_PARAM uint16_t port, void* userData) {
    uint8_t retval;

    for (;;) {
        bool (* func)(uint16_t, uint8_t&) = devMapInput[port];

        if (!func) {
            return 0xFF;
        }

        if (func(port, retval)) {
            return retval;
        }
    }
}

void OutputByte(Z80EX_CONTEXT_PARAM uint16_t port, uint8_t value, void* userData) {
    for (;;) {
        bool (* func)(uint16_t, uint8_t) = devMapOutput[port];

        if (!func || func(port, value)) {
            return;
        }
    }
}

uint8_t ReadIntVec(Z80EX_CONTEXT_PARAM void* userData) {
    return 0xFF;
}

//--------------------------------------------------------------------------------------------------------------

void InitDevMapRead(ptrOnReadByteFunc* map) {
    ptrOnReadByteFunc func;

    for (unsigned m1_state = 0; m1_state < 2; m1_state++) {
        for (unsigned addr = 0; addr < 0x10000; addr++) {
            map[addr + (m1_state ? 0x10000 : 0)] = nullptr;

            for (int i = 0; i < cnt_z80read; i++) {
                if ((func = hnd_z80read[i].check(addr, m1_state)) != nullptr) {
                    map[addr + (m1_state ? 0x10000 : 0)] = func;
                    break;
                }
            }
        }
    }
}

void InitDevMapWrite(bool (** map)(uint16_t, uint8_t)) {
    for (unsigned addr = 0; addr < 0x10000; addr++) {
        map[addr] = nullptr;

        for (int i = 0; i < cnt_z80write; i++) {
            if (hnd_z80write[i].check(addr)) {
                map[addr] = hnd_z80write[i].func;
                break;
            }
        }
    }
}

void InitDevMapInput(bool (** map)(uint16_t, uint8_t&)) {
    for (unsigned port = 0; port < 0x10000; port++) {
        map[port] = nullptr;

        for (int i = 0; i < cnt_z80input; i++) {
            if (hnd_z80input[i].check(port)) {
                map[port] = hnd_z80input[i].func;
                break;
            }
        }
    }
}

void InitDevMapOutput(bool (** map)(uint16_t, uint8_t)) {
    for (unsigned port = 0; port < 0x10000; port++) {
        map[port] = nullptr;

        for (int i = 0; i < cnt_z80output; i++) {
            if (hnd_z80output[i].check(port)) {
                map[port] = hnd_z80output[i].func;
                break;
            }
        }
    }
}

void InitDevMaps(void) {
    C_TrDos::trdos = true;

    InitDevMapRead(devMapRead_trdos);
    InitDevMapInput(devMapInput_trdos);
    InitDevMapOutput(devMapOutput_trdos);

    C_TrDos::trdos = false;

    InitDevMapRead(devMapRead_base);
    InitDevMapWrite(devMapWrite_base);
    InitDevMapInput(devMapInput_base);
    InitDevMapOutput(devMapOutput_base);

    devMapRead = devMapRead_base;
    devMapWrite = devMapWrite_base;
    devMapInput = devMapInput_base;
    devMapOutput = devMapOutput_base;
}

//--------------------------------------------------------------------------------------------------------------

void InitFont(void) {
    font = new C_Font(font_boldData);
    font->CopySym('-', '_');
    font->SetSymOff('_', 0, 4);
    font->SetSymOff('-', 0, 1);

    fixed_font = new C_Font(font_thinData);
}

void InitAll(void) {
    screen = new uint32_t[WIDTH * HEIGHT];
    renderScreenBuffer[0] = new uint32_t[WIDTH * HEIGHT];
    renderScreenBuffer[1] = new uint32_t[WIDTH * HEIGHT];

    for (int i = 0; devs[i]; i++) {
        devs[i]->Init();
    }

    InitDevMaps();

    for (int i = 0; i < 0x10; i++) {
        colors[i] = colors_base[i];
    }

    InitFont();
    FileDialogInit();
    C_Tape::Init();

    for (int i = 0; i < 0x10000; i++) {
        breakpoints[i] = false;
    }

    cpu = z80ex_create(
        ReadByte,
        nullptr,
        WriteByte,
        nullptr,
        InputByte,
        nullptr,
        OutputByte,
        nullptr,
        ReadIntVec,
        nullptr
    );
}

// ----------------------------------

void AntiFlicker(int copyFrom, int copyTo) {
    if (doCopyOfSurfaces) {
        memcpy(renderScreenBuffer[copyTo], renderScreenBuffer[copyFrom], WIDTH * HEIGHT * sizeof(uint32_t));
        doCopyOfSurfaces = false;
    }

    uint8_t* s1 = (uint8_t*)renderScreenBuffer[0];
    uint8_t* s2 = (uint8_t*)renderScreenBuffer[1];
    uint8_t* sr = (uint8_t*)screen;

    for (int i = WIDTH * HEIGHT; i--;) {
        #ifdef STAGE_FLIPPED_ARGB
            *(sr++) = 0; ++s1; ++s2;
        #endif

        *(sr++) = (uint8_t)(((unsigned int)(*(s1++)) + (unsigned int)(*(s2++))) >> 1);
        *(sr++) = (uint8_t)(((unsigned int)(*(s1++)) + (unsigned int)(*(s2++))) >> 1);
        *(sr++) = (uint8_t)(((unsigned int)(*(s1++)) + (unsigned int)(*(s2++))) >> 1);

        #ifndef STAGE_FLIPPED_ARGB
            *(sr++) = 0; ++s1; ++s2;
        #endif
    }
}

void InitActClk(void) {
    actDevClkCounter = devClkCounter * (uint64_t)turboMultiplier;
    actClk = cpuClk * (uint64_t)turboMultiplier;
}

// TODO:
// CpuCalcTacts - pointer to function
// CpuCalcTacts_Normal
// CpuCalcTacts_Turbo
// CpuCalcTacts_Unturbo
// DebugCpuCalcTacts_Normal
// ...
// ...
// TODO: refactor

inline void CpuCalcTacts(unsigned long cmdClk) {
    if (turboMultiplier < 2) {
        devClkCounter += (uint64_t)cmdClk;
        cpuClk += (uint64_t)cmdClk;
    } else if (unturbo) {
        cmdClk *= turboMultiplier;
        devClkCounter += (uint64_t)cmdClk;
        cpuClk += (uint64_t)cmdClk;
    } else {
        actDevClkCounter += (uint64_t)cmdClk;
        actClk += (uint64_t)cmdClk;

        devClkCounter = (actDevClkCounter + (uint64_t)(turboMultiplier - 1)) / (uint64_t)turboMultiplier;
        cpuClk = (actClk + (uint64_t)(turboMultiplier - 1)) / (uint64_t)turboMultiplier;
    }

    devClk = cpuClk;
    C_Tape::Process();

    if (runDebuggerFlag || breakpoints[z80ex_get_reg(cpu, regPC)]) {
        runDebuggerFlag = false;
        RunDebugger();
    }
}

inline void DebugCpuCalcTacts(unsigned long cmdClk) {
    if (turboMultiplier < 2) {
        devClkCounter += (uint64_t)cmdClk;
        cpuClk += (uint64_t)cmdClk;
    } else if (unturbo) {
        cmdClk *= turboMultiplier;
        devClkCounter += (uint64_t)cmdClk;
        cpuClk += (uint64_t)cmdClk;
    } else {
        actDevClkCounter += (uint64_t)cmdClk;
        actClk += (uint64_t)cmdClk;

        devClkCounter = (actDevClkCounter + (uint64_t)(turboMultiplier-1)) / (uint64_t)turboMultiplier;
        cpuClk = (actClk + (uint64_t)(turboMultiplier-1)) / (uint64_t)turboMultiplier;
    }

    devClk = cpuClk;
    C_Tape::Process();
}

int TraceCpuStep(Z80EX_CONTEXT* cpu) {
    CpuTrace_Log();
    cpuTrace_intReq = 0;
    cpuTrace_dT = z80ex_step(cpu);
    return cpuTrace_dT;
}

int TraceCpuInt(Z80EX_CONTEXT* cpu) {
    CpuTrace_Log();
    int dt = z80ex_int(cpu);
    cpuTrace_dT += dt;
    cpuTrace_intReq++;
    return dt;
}

inline void CpuStep(void) {
    CpuCalcTacts(DoCpuStep(cpu));
}

inline void CpuInt(void) {
    CpuCalcTacts(DoCpuInt(cpu));
}

inline void DebugCpuStep(void) {
    DebugCpuCalcTacts(DoCpuStep(cpu));
}

inline void DebugCpuInt(void) {
    DebugCpuCalcTacts(DoCpuInt(cpu));
}

void DebugStep(void) {
    int cnt = 4;

    do {
        if (cpuClk < MAX_FRAME_TACTS) {
            CpuStep();

            if (cpuClk < INT_LENGTH) {
                CpuInt();
            }
        } else {
            lastDevClk = devClk;
            cpuClk -= MAX_FRAME_TACTS;
            devClk = cpuClk;

            InitActClk();
        }

        cnt--;
    } while (z80ex_last_op_type(cpu) && cnt > 0);
}

void Render(void) {
    static int sn = 0;

    if (params.antiFlicker) {
        renderScreen = renderScreenBuffer[sn];
        sn = 1 - sn;
    } else {
        renderScreen = screen;
    }

    if (dev_extport.Is16Colors()) {
        renderPtr = Render16c;
    } else if (dev_extport.IsMulticolor()) {
        renderPtr = RenderMulticolor;
    } else {
        renderPtr = RenderSpeccy;
    }

    // TODO: if (dev_extport.Is512x192()) { renderPtr = Render512x192; }
    // TODO: if (dev_extport.Is384x304()) { renderPtr = Render384x304; }

    InitActClk();
    prevRenderClk = 0;

    while (cpuClk < INT_LENGTH) {
        CpuStep();
        CpuInt();
    }

    if (drawFrame) {
        while (cpuClk < MAX_FRAME_TACTS) {
            CpuStep();
            renderPtr(cpuClk);
        }
    } else {
        while (cpuClk < MAX_FRAME_TACTS) {
            CpuStep();
        }
    }

    renderPtr = nullptr;
    lastDevClk = devClk;
    cpuClk -= MAX_FRAME_TACTS;
    devClk = cpuClk;

    if (params.antiFlicker && drawFrame) {
        AntiFlicker(1 - sn, sn);
    }
}

void DrawIndicators(void) {
    char buf[0x100];
    DRIVE_STATE st = dev_trdos.GetIndicatorState();

    if (st == DS_READ) {
        OutputGimpImage(8, 0, (s_GimpImage*)((void*) &img_floppyRead));
    } else if (st == DS_WRITE) {
        OutputGimpImage(8, 0, (s_GimpImage*)((void*) &img_floppyWrite));
    } else if (params.showInactiveIcons) {
        OutputGimpImage(8, 0, (s_GimpImage*)((void*) &img_floppy));
    }

    if (params.maxSpeed) {
        OutputGimpImage(32, 0, (s_GimpImage*)((void*) &img_turboOn));
    } else if (params.showInactiveIcons) {
        OutputGimpImage(32, 0, (s_GimpImage*)((void*) &img_turboOff));
    }

    if (C_Tape::IsActive()) {
        sprintf(buf, "%u%%", C_Tape::GetPosPerc());
        font->PrintString(WIDTH - 4 - font->StrLenPx(buf), 4, buf);
    }

    for (unsigned i = 0; i < watchesCount; i++) {
        uint8_t val = ReadByteDasm(watches[i], nullptr);
        sprintf(buf, "%04X:%02X", watches[i], val);
        fixed_font->PrintString(WIDTH - 4 - fixed_font->StrLenPx(buf), 4 + fixed_font->Height() * (i + 1), buf);
    }
}

void ResetSequence(void) {
    int cnt = cnt_reset;
    void (** ptr)(void) = hnd_reset;

    z80ex_reset(cpu);

    while (cnt) {
        (*ptr)();

        ptr++;
        cnt--;
    }
}

void UpdateScreen(void) {
    host->stage()->renderFrame(screen, WIDTH, HEIGHT);
}

void Process(void) {
    StageEvent event;
    int i;
    int frameSkip = 0;
    bool tapePrevActive = false;

    devClkCounter = 0;
    cpuClk = 0;
    devClk = 0;
    lastDevClk = 0;
    frames = 0;
    params.maxSpeed = false;
    uint32_t ntick = host->timer()->getElapsedMillis() + ((params.maxSpeed || host->stage()->isSoundEnabled()) ? 0 : FRAME_WAIT_MS);

    for (;;) {
        if (!isPaused) {
            i = cnt_frameStart;
            void (** ptr_frameStart)(void) = hnd_frameStart;

            while (i) {
                (*ptr_frameStart)();

                ptr_frameStart++;
                i--;
            }

            if (params.maxSpeed) {
                if (frameSkip > 0) {
                    frameSkip--;
                    drawFrame = false;
                } else {
                    frameSkip = MAX_SPEED_FRAME_SKIP;
                    drawFrame = true;
                }
            } else {
                drawFrame = true;
            }

            tapePrevActive = C_Tape::IsActive();

            Render();
            frames++;

            if (drawFrame) {
                DrawIndicators();
                ShowMessage();
                UpdateScreen();
            }

            if (!params.maxSpeed) {
                host->timer()->wait(1);
                uint32_t ctick = host->timer()->getElapsedMillis();

                if (ctick < ntick) {
                    host->timer()->wait(ntick - ctick);
                }
            }

            i = cnt_afterFrameRender;
            void (** ptr_afterFrameRender)(void) = hnd_afterFrameRender;

            while (i) {
                (*ptr_afterFrameRender)();

                ptr_afterFrameRender++;
                i--;
            }

            soundMixer.FlushFrame(SHOULD_OUTPUT_SOUND);
        }

        ntick = host->timer()->getElapsedMillis() + ((params.maxSpeed || host->stage()->isSoundEnabled()) ? 0 : FRAME_WAIT_MS);
        isPaused = isPausedNx;
        bool quitMode = false;

        while (host->stage()->pollEvent(&event)) {
            if (event.type == STAGE_EVENT_QUIT) {
                exit(0);
            }

            if (event.type == STAGE_EVENT_KEYUP && event.keyCode == STAGE_KEYCODE_ESCAPE) {
                quitMode = true;
            }

            i = cnt_hw;
            s_HwItem* ptr_hw = hnd_hw;

            while (i) {
                if (ptr_hw->eventType == event.type && ptr_hw->func(event)) {
                    break;
                }

                ptr_hw++;
                i--;
            }
        }

        if (quitMode) {
            isPaused = false;

            if (DlgConfirm("Do you really want to quit?")) {
                return;
            }
        }

        turboMultiplier = turboMultiplierNx;
        unturbo = unturboNx;

        if (!isPaused && tapePrevActive && !C_Tape::IsActive()) {
            if (params.maxSpeed) {
                SetMessage("Tape end : MaxSpeed OFF");
                params.maxSpeed = false;
            } else {
                SetMessage("Tape end");
            }
        }
    }
}

void FreeAll(void) {
    for (int i = 0; devs[i]; i++) {
        devs[i]->Close();
    }

    C_Tape::Close();

    if (fixed_font) {
        delete fixed_font;
    }

    if (font) {
        delete font;
    }

    if (cpu) {
        z80ex_destroy(cpu);
    }

    if (renderScreenBuffer[1]) {
        delete[] renderScreenBuffer[1];
    }

    if (renderScreenBuffer[0]) {
        delete[] renderScreenBuffer[0];
    }

    if (screen) {
        delete[] screen;
    }

    delete host;
}

void ParseCmdLine(int argc, char *argv[]) {
    argv++;
    argc--;

    while (argc > 0) {
        if (!strcmp(*argv, "-l")) {
            if (argc > 1) {
                argv++;
                argc--;

                Labels_Load(*argv);
            }
        } else if (!strcmp(*argv, "-w")) {
            recordWav = true;
        } else {
            TryNLoadFile(*argv);
            return;
        }

        argv++;
        argc--;
    }
}

void OutputLogo(void) {
    printf("                                        \n");
    printf("    $ww,.                               \n");
    printf("     `^$$$ww,.                          \n");
    printf("        `^$$$$$$                        \n");
    printf("          ,$$7'                         \n");
    printf("        _j$$'   __    __  _ _           \n");
    printf("      ,j$$7      /__ (-_ | ) ) (_|      \n");
    printf("     $$$$$$w.                           \n");
    printf("      `^^T$$$w,             rst'o6      \n");
    printf("            `^T$$                       \n");
    printf("                                        \n");
    printf(" restorer     [ restorer.fct@gmail.com ]\n");
    printf(" boo_boo      [    boo_boo@inbox.ru    ]\n");
    printf(" mastermind   [   zxmmind@gmail.com    ]\n");
    printf("                                        \n");
    printf(" breeze (gfx) [  breeze.fbn@gmail.com  ]\n");
    printf("                                        \n");
    printf(" with help of SMT                       \n");
    printf("                                        \n");
}

int main(int argc, char *argv[]) {
    OutputLogo();

    host = new HostImpl(argc, argv, "zemu");
    atexit(FreeAll);

    auto config = host->config();

    try {
        StageConfig stageConfig;

        stageConfig.title = "zEmu";
        stageConfig.desiredFrameWidth = WIDTH;
        stageConfig.desiredFrameHeight = HEIGHT;
        stageConfig.soundFreq = SOUND_FREQ;

        #ifdef _WIN32
            stageConfig.windowsIconResource = MAKEINTRESOURCE(IDI_ICON1);
        #endif

        // core
        std::string str = config->getString("core", "snapformat", "sna");
        transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);

        if (str == "sna") {
            params.snapFormat = SNAP_FORMAT_SNA;
        } else {
            params.snapFormat = SNAP_FORMAT_Z80;
        }

        // beta128
        str = config->getString("beta128", "diskA", "");

        if (!str.empty()) {
            wd1793_load_dimage(str.c_str(), 0);
        }

        str = config->getString("beta128", "diskB", "");

        if (!str.empty()) {
            wd1793_load_dimage(str.c_str(), 1);
        }

        str = config->getString("beta128", "diskC", "");

        if (!str.empty()) {
            wd1793_load_dimage(str.c_str(), 2);
        }

        str = config->getString("beta128", "diskD", "");

        if (!str.empty()) {
            wd1793_load_dimage(str.c_str(), 3);
        }

        wd1793_set_nodelay(config->getBool("beta128", "nodelay", false));

        str = host->storage()->findExtras("boot", config->getString("beta128", "sclboot", "boot.$b"))->string();

        if (!str.empty()) {
            wd1793_set_appendboot(str.c_str());
        }

        // display
        stageConfig.fullscreen = config->getBool("display", "fullscreen", false);

        if (!config->getBool("display", "scale2x", true)) {
            stageConfig.renderMode = STAGE_RENDER_MODE_1X;
        } else if (config->getBool("display", "scanlines", false)) {
            stageConfig.renderMode = STAGE_RENDER_MODE_2X_SCANLINES;
        } else {
            stageConfig.renderMode = STAGE_RENDER_MODE_2X;
        }

        #ifdef USE_SDL1
            if (config->getBool("display", "sdl_useflipsurface", false)) {
                stageConfig.hints |= STAGE_HINT_FLIP_SURFACE;
            }
        #endif

        params.antiFlicker = config->getBool("display", "antiflicker", false);
        params.showInactiveIcons = config->getBool("display", "showinactiveicons", false);

        // input
        params.mouseDiv = config->getInt("input", "mousediv", 1);

        if (params.mouseDiv <= 0) {
            params.mouseDiv = 1;
        } else if (params.mouseDiv > 8) {
            params.mouseDiv = 8;
        }

        // sound
        stageConfig.soundEnabled = config->getBool("sound", "enable", true);
        params.mixerMode = config->getInt("sound", "mixermode", 1);

        str = config->getString("sound", "sound_backend", "auto");
        transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);

        if (str == "none") {
            stageConfig.soundDriver = STAGE_SOUND_DRIVER_NONE;
        } else if (str == "sdl") {
            stageConfig.soundDriver = STAGE_SOUND_DRIVER_GENERIC;
            stageConfig.soundParams[0] = std::log2(config->getInt("sound", "sdlbuffersize", 0));
        }
        #ifdef _WIN32
            else if (str == "win32") {
                stageConfig.soundDriver = STAGE_SOUND_DRIVER_WIN32;
                stageConfig.soundParams[0] = config->getInt("sound", "wqsize", 0);
            }
        #elif __unix__
            else if (str == "oss") {
                stageConfig.soundDriver = STAGE_SOUND_DRIVER_OSS;
                stageConfig.soundParams[0] = config->getInt("sound", "ossfragnum", 0);
            }
        #endif
        else {
            #ifdef _WIN32
                stageConfig.soundDriver = STAGE_SOUND_DRIVER_NATIVE;
            #else
                stageConfig.soundDriver = STAGE_SOUND_DRIVER_GENERIC;
            #endif
        }

        // joystick
        stageConfig.joystickEnabled = (config->getInt("kempstonjoystick", "sysjoysticknum", -1) >= 0);
        stageConfig.joystickAxisThreshold = config->getInt("kempstonjoystick", "axisthreshold", stageConfig.joystickAxisThreshold);

        // cputrace
        params.cpuTraceEnabled = config->getBool("cputrace", "enable", false);

        str = config->getString(
            "cputrace",
            "format",
            "[PC]> [M1]:[M2] dT=[DT] AF=[AF] BC=[BC] DE=[DE] HL=[HL] IX=[IX] IY=[IY] SP=[SP] I=[I] R=[R] AF'=[AF'] BC'=[BC'] DE'=[DE'] HL'=[HL'] IFF1=[IFF1] IFF2=[IFF2] IM=[IM] INTR=[INTR]"
        );

        strcpy(params.cpuTraceFormat, str.c_str());
        str = config->getString("cputrace", "filename", "cputrace.log");
        strcpy(params.cpuTraceFileName, str.c_str());

        host->setStageConfig(stageConfig);
        host->stage(); // force stage initialization

        if (params.cpuTraceEnabled) {
            DoCpuStep = TraceCpuStep;
            DoCpuInt = TraceCpuInt;
            CpuTrace_Init();
        }

        if (argc != 1) {
            ParseCmdLine(argc, argv);
        }

        soundMixer.Init(params.mixerMode, recordWav, wavFileName);

        InitAll();
        ResetSequence();

        if (config->getBool("core", "trdos_at_start", false)) {
            dev_mman.OnOutputByte(0x7FFD, 0x10);
            dev_trdos.Enable();
        }

        Process();

        if (params.cpuTraceEnabled) {
            CpuTrace_Close();
        }
    } catch (std::exception &e) {
        StrikeError("%s", e.what());
    }

    return 0;
}
