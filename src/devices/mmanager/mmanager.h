#ifndef _MMANAGER_H_INCLDUED_
#define _MMANAGER_H_INCLDUED_

#include "../../zemu.h"
#include "../device.h"

#define RAM_BANK2 (0x4000 * 2)
#define RAM_BANK5 (0x4000 * 5)
#define RAM_BANK7 (0x4000 * 7)

#define RAM_BANK4 (0x4000 * 4)
#define RAM_BANK6 (0x4000 * 6)

string split_romname(string& romname, size_t* offset);

class C_MemoryManager : public C_Device {
    public:

    static uint8_t port7FFD;
    static uint8_t rom[0x8000];
    static uint8_t ram[0x4000 * 64];

    static uint8_t* rom_map;
    static uint8_t* ram_map;
    static bool enable512;
    static bool enable1024;

    static void ReadFile(void);
    void Init(void);
    void Close(void);

    static void Remap(void);
    static ptrOnReadByteFunc ReadByteCheckAddr(uint16_t addr, bool m1);
    static uint8_t OnReadByte_ROM(uint16_t addr, bool m1);
    static uint8_t OnReadByte_Bank5(uint16_t addr, bool m1);
    static uint8_t OnReadByte_Bank2(uint16_t addr, bool m1);
    static uint8_t OnReadByte_C000(uint16_t addr, bool m1);
    static bool WriteByteCheckAddr(uint16_t addr);
    static bool OnWriteByte(uint16_t addr, uint8_t value);
    static bool OutputByteCheckPort(uint16_t port);
    static bool OnOutputByte(uint16_t port, uint8_t value);
    static void OnReset(void);
};

#endif
