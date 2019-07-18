// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include "zemu_env.h"
#include "snap_sna.h"
#include "zemu.h"

bool load_sna_snap(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border) {
    bool banks[8];
    uint8_t buffer[16384];
    DataReaderPtr reader;

    try {
        reader = hostEnv->storage()->path(filename)->dataReader();
    } catch (StorageException& e) {
        printf("Load failed: %s\n", e.what());
        return false;
    }

    z80ex_set_reg(cpu, regI, reader->readByte());
    z80ex_set_reg(cpu, regHL_, reader->readWord());
    z80ex_set_reg(cpu, regDE_, reader->readWord());
    z80ex_set_reg(cpu, regBC_, reader->readWord());
    z80ex_set_reg(cpu, regAF_, reader->readWord());
    z80ex_set_reg(cpu, regHL, reader->readWord());
    z80ex_set_reg(cpu, regDE, reader->readWord());
    z80ex_set_reg(cpu, regBC, reader->readWord());
    z80ex_set_reg(cpu, regIY, reader->readWord());
    z80ex_set_reg(cpu, regIX, reader->readWord());

    uint8_t intm = reader->readByte();

    z80ex_set_reg(cpu, regIFF1, intm & 1);
    z80ex_set_reg(cpu, regIFF2, (intm >> 1) & 1);

    z80ex_set_reg(cpu, regR, reader->readByte());
    z80ex_set_reg(cpu, regAF, reader->readWord());
    z80ex_set_reg(cpu, regSP, reader->readWord());
    z80ex_set_reg(cpu, regIM, reader->readByte());

    uint8_t border_color = reader->readByte();
    mmgr.OnReset();

    for (int i = 0; i < 8; i++) {
        banks[i] = false;
    }

    for (int i = 0; i < 16384; i++) {
        mmgr.OnWriteByte(i + 16384, reader->readByte());
    }

    banks[5] = true;

    for (int i = 0; i < 16384; i++) {
        mmgr.OnWriteByte(i + 32768, reader->readByte());
    }

    banks[2] = true;

    for (int i = 0; i < 16384; i++) {
        buffer[i] = reader->readByte();
    }

    if (reader->isEof()) {
        mmgr.OnOutputByte(0x7ffd, 0x30); // set 48k mode

        for (int i = 0; i < 16384; i++) {
            mmgr.OnWriteByte(i + 49152, buffer[i]);
        }

        uint16_t sp = z80ex_get_reg(cpu, regSP);

        uint8_t retval = ReadByteDasm(sp, nullptr);
        uint16_t pc = retval;
        mmgr.OnWriteByte(sp, 0);
        sp++;

        retval = ReadByteDasm(sp, nullptr);
        pc |= ((uint16_t)retval << 8);
        mmgr.OnWriteByte(sp, 0);
        sp++;

        z80ex_set_reg(cpu, regSP, sp);
        z80ex_set_reg(cpu, regPC, pc);
    } else {
        z80ex_set_reg(cpu, regPC, reader->readWord());
        uint8_t port_7ffd = reader->readByte();
        uint8_t is_trdos = reader->readByte();
        mmgr.OnOutputByte(0x7ffd, port_7ffd & 7);

        for (int i = 0; i < 16384; i++) {
            mmgr.OnWriteByte(i + 49152, buffer[i]);
        }

        banks[port_7ffd & 7] = true;

        for (int k = 0; k < 8; k++) {
            if (banks[k]) {
                continue;
            }

            mmgr.OnOutputByte(0x7ffd, k);

            for (int i = 0; i < 16384; i++) {
                mmgr.OnWriteByte(i + 49152, reader->readByte());
            }
        }

        C_TrDos::trdos = (is_trdos != 0);
        mmgr.OnOutputByte(0x7ffd, port_7ffd);
    }

    // set border color
    border.OnOutputByte(0x00FE, border_color);

    return true;
}

bool save_sna_snap(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border) {
    bool banks[8];
    DataWriterPtr writer;

    try {
        writer = hostEnv->storage()->path(filename)->dataWriter();
    } catch (StorageException& e) {
        printf("Write failed: %s\n", e.what());
        return false;
    }

    writer->writeByte(z80ex_get_reg(cpu, regI));
    writer->writeWord(z80ex_get_reg(cpu, regHL_));
    writer->writeWord(z80ex_get_reg(cpu, regDE_));
    writer->writeWord(z80ex_get_reg(cpu, regBC_));
    writer->writeWord(z80ex_get_reg(cpu, regAF_));
    writer->writeWord(z80ex_get_reg(cpu, regHL));
    writer->writeWord(z80ex_get_reg(cpu, regDE));
    writer->writeWord(z80ex_get_reg(cpu, regBC));
    writer->writeWord(z80ex_get_reg(cpu, regIY));
    writer->writeWord(z80ex_get_reg(cpu, regIX));

    writer->writeByte((z80ex_get_reg(cpu, regIFF1) ? 1 : 0) | (z80ex_get_reg(cpu, regIFF2) ? 2 : 0));
    uint16_t sp = z80ex_get_reg(cpu, regSP);

    if (mmgr.port7FFD == 0x30) {
        // 48k
        sp -= 2;
    }

    writer->writeByte(z80ex_get_reg(cpu, regR));
    writer->writeWord(z80ex_get_reg(cpu, regAF));
    writer->writeWord(sp);
    writer->writeByte(z80ex_get_reg(cpu, regIM));

    writer->writeByte(border.portFB & 7);
    uint8_t buffer[0xC000];

    for (int i = 0; i < 8; i++) {
        banks[i] = false;
    }

    for (int i = 0; i < 0x4000; i++) {
        buffer[i] = mmgr.ram[5 * 0x4000 + i];
    }

    banks[5] = true;

    for (int i = 0; i < 0x4000; i++) {
        buffer[i + 0x4000] = mmgr.ram[2 * 0x4000 + i];
    }

    banks[2] = true;
    uint16_t pc = z80ex_get_reg(cpu, regPC);

    if (mmgr.port7FFD == 0x30) { // 48k
        for (int i = 0; i < 16384; i++) {
            buffer[i + 0x8000] = mmgr.ram[i];
        }

        if (sp >= 0x4000) {
            buffer[sp - 0x4000] = (uint8_t)(pc & 0xFF);
        }

        sp++;

        if (sp >= 0x4000) {
            buffer[sp - 0x4000] = (uint8_t)(pc >> 8);
        }

        writer->writeBlock(buffer, 0xC000);
    } else {
        writer->writeBlock(buffer, 0x8000);

        int bank = mmgr.port7FFD & 7;
        writer->writeBlock(&mmgr.ram[0x4000 * bank], 0x4000);
        banks[bank] = true;

        writer->writeWord(pc);
        writer->writeByte(mmgr.port7FFD);
        writer->writeByte(C_TrDos::trdos ? 1 : 0);

        for (int k = 0; k < 8; k++) {
            if (banks[k]) {
                continue;
            }

            writer->writeBlock(&mmgr.ram[0x4000 * k], 0x4000);
        }
    }

    return true;
}
