// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include "snap_z80.h"
#include "exceptions.h"
#include "zemu.h"
#include "file.h"

#define HANDLE_PRAGMA_PACK_PUSH_POP
#pragma pack(push,1)

struct Z80_snap_header {
    uint8_t A;
    uint8_t F;

    uint8_t C;
    uint8_t B;

    uint8_t L;
    uint8_t H;

    uint8_t PCl;
    uint8_t PCh;

    uint8_t SPl;
    uint8_t SPh;

    uint8_t I;
    uint8_t R;
    uint8_t R7_n_misc; // bit0: R7, bit1-3: border, bit5: data is compressed

    uint8_t E;
    uint8_t D;

    uint8_t C_;
    uint8_t B_;

    uint8_t E_;
    uint8_t D_;

    uint8_t L_;
    uint8_t H_;

    uint8_t A_;
    uint8_t F_;

    uint8_t IYl;
    uint8_t IYh;

    uint8_t IXl;
    uint8_t IXh;

    uint8_t IFF1;
    uint8_t IFF2;

    uint8_t IM_n_misc; // bit0-1: Interrupt mode
};

#pragma pack(pop)

bool block_read(C_File& fl, C_MemoryManager& mmgr, uint16_t mem_offset, uint16_t len, int is_compressed) {
    uint8_t b = 0;
    uint8_t b_prev = 0;

    for (unsigned addr = mem_offset; addr < mem_offset + len; b_prev = b) {
        if (fl.Eof()) {
            return false;
        }

        b = fl.GetBYTE();

        if (is_compressed && b == 0xED && b_prev == 0xED) {
            if (fl.Eof()) {
                return false;
            }

            uint8_t rep_count = fl.GetBYTE();

            if (fl.Eof()) {
                return false;
            }

            uint8_t rep_byte = fl.GetBYTE();

            for (int cb = 0; cb < rep_count; cb++) {
                mmgr.OnWriteByte(addr + cb - 1, rep_byte);
            }

            addr += rep_count - 1;
            b = 0;
        } else {
            mmgr.OnWriteByte(addr++, b);
        }
    }

    return true;
}

bool load_z80_snap(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border) {
    Z80_snap_header hdr;
    uint16_t tmp;
    uint8_t* header_add = nullptr;
    uint8_t border_color;
    uint8_t mode;
    uint8_t page_num;
    int is_compressed;
    int ver = 1;
    int i;

    try {
        C_File fl(filename);

        if (fl.ReadBlock(&hdr, sizeof(hdr)) != sizeof(hdr)) {
            return false;
        }

        z80ex_set_reg(cpu, regAF, ZEMU_MAKEWORD(hdr.A, hdr.F));
        z80ex_set_reg(cpu, regBC, ZEMU_MAKEWORD(hdr.B, hdr.C));
        z80ex_set_reg(cpu, regHL, ZEMU_MAKEWORD(hdr.H, hdr.L));
        z80ex_set_reg(cpu, regSP, ZEMU_MAKEWORD(hdr.SPh, hdr.SPl));
        z80ex_set_reg(cpu, regI, hdr.I);

        if (hdr.R7_n_misc == 255) {
            hdr.R7_n_misc = 1; // compatibility issue
        }

        z80ex_set_reg(cpu, regR, ((hdr.R7_n_misc & 0x01) << 7) | (hdr.R & 0x7F));
        border_color = (hdr.R7_n_misc >> 1) & 0x07;
        is_compressed = hdr.R7_n_misc & 0x20;

        z80ex_set_reg(cpu, regDE, ZEMU_MAKEWORD(hdr.D, hdr.E));
        z80ex_set_reg(cpu, regBC_, ZEMU_MAKEWORD(hdr.B_, hdr.C_));
        z80ex_set_reg(cpu, regDE_, ZEMU_MAKEWORD(hdr.D_, hdr.E_));
        z80ex_set_reg(cpu, regHL_, ZEMU_MAKEWORD(hdr.H_, hdr.L_));
        z80ex_set_reg(cpu, regAF_, ZEMU_MAKEWORD(hdr.A_, hdr.F_));
        z80ex_set_reg(cpu, regIY, ZEMU_MAKEWORD(hdr.IYh, hdr.IYl));
        z80ex_set_reg(cpu, regIX, ZEMU_MAKEWORD(hdr.IXh, hdr.IXl));
        z80ex_set_reg(cpu, regIFF1, hdr.IFF1 ? 1 : 0);
        z80ex_set_reg(cpu, regIFF2, hdr.IFF2 ? 1 : 0);
        z80ex_set_reg(cpu, regIM, hdr.IM_n_misc & 0x03);

        mmgr.OnReset();

        if (hdr.PCh | hdr.PCl) { // PC != 0: Z80 v1
            // read 48k block
            if (!block_read(fl, mmgr, 0x4000, 0xC000, is_compressed)) {
                return false;
            }

            mmgr.OnOutputByte(0x7ffd, 0x30); // set 48k mode
            z80ex_set_reg(cpu, regPC, ZEMU_MAKEWORD(hdr.PCh, hdr.PCl));
        } else { // Z80 v2 or v3
            if (fl.Eof()) {
                return false;
            }

            tmp = fl.GetWORD(); // additional block length

            switch (tmp & 0xFF) { // can we use just "tmp" ?
                case 23:
                    ver = 2;
                    break;

                case 54: // fallthrough
                case 55:
                    ver = 3;
                    break;

                default:
                    ver=3;
            }

            // вроде как нет понту проверять header_addr на nullptr, ибо stdc++ либа сама кинет эксцэпшн если не получиться выделить память
            header_add = new uint8_t[tmp];

            try {
                if (fl.ReadBlock(header_add, tmp) != tmp) {
                    throw C_E(E_SnapZ80Error);
                }

                z80ex_set_reg(cpu, regPC, ZEMU_MAKEWORD(header_add[1], header_add[0]));
                mode = header_add[2];

                if (mode == 10) {
                    // TODO: Scorpion256 mode
                    throw C_E(E_SnapZ80Error);
                } else if ((ver == 2 && mode < 3) || (ver == 3 && mode < 4)) {
                    // 48k or 16k mode
                    int pages;

                    if (header_add[5] & 0x80) {
                        pages = 1; // 16k
                    } else {
                        pages = 3; // 48k
                    }

                    // set 48k mode
                    mmgr.OnOutputByte(0x7ffd, 0x30);

                    for (i = 0; i < pages; ++i) {
                        if (fl.Eof()) {
                            throw C_E(E_SnapZ80Error);
                        }

                        tmp = fl.GetWORD();
                        is_compressed = (tmp != 0xFFFF);

                        if (fl.Eof()) {
                            throw C_E(E_SnapZ80Error);
                        }

                        page_num = fl.GetBYTE();

                        switch (page_num) {
                            case 4:
                                if (!block_read(fl, mmgr, 0x8000, 0x4000, is_compressed)) {
                                    throw C_E(E_SnapZ80Error);
                                }

                                break;

                            case 5:
                                if (!block_read(fl, mmgr, 0xC000, 0x4000, is_compressed)) {
                                    throw C_E(E_SnapZ80Error);
                                }

                                break;

                            case 8:
                                if (!block_read(fl, mmgr, 0x4000, 0x4000, is_compressed)) {
                                    throw C_E(E_SnapZ80Error);
                                }

                                break;

                            default: // unknown page
                                throw C_E(E_SnapZ80Error);
                        }
                    }
                } else {
                    // 128k mode

                    for (i = 0; i < 8; i++) {
                        if (fl.Eof()) {
                            throw C_E(E_SnapZ80Error);
                        }

                        tmp = fl.GetWORD();
                        is_compressed = (tmp != 0xFFFF);

                        if (fl.Eof()) {
                            throw C_E(E_SnapZ80Error);
                        }

                        page_num = fl.GetBYTE();
                        mmgr.OnOutputByte(0x7ffd, page_num - 3);

                        if (!block_read(fl, mmgr, 0xC000, 0x4000, is_compressed)) {
                            throw C_E(E_SnapZ80Error);
                        }
                    }

                    mmgr.OnOutputByte(0x7ffd, header_add[3]);
                }

                // TODO: set fffd, 1ffd ports, AY/YM registers
                delete[] header_add;
            } catch (C_E &e) {
                delete[] header_add;
                return false;
            }
        }

        // set border color
        border.OnOutputByte(0x00FE, border_color);

        return true;
    } catch (C_E &e) {
        return false;
    }
}

void save_z80_snap(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border) {
    Z80_snap_header hdr;
    uint8_t add_header[23]; // Z80 v3 header

    C_File fl(filename, false);

    hdr.A = z80ex_get_reg(cpu, regAF) >> 8;
    hdr.F = z80ex_get_reg(cpu, regAF) & 0xFF;
    hdr.B = z80ex_get_reg(cpu, regBC) >> 8;
    hdr.C = z80ex_get_reg(cpu, regBC) & 0xFF;
    hdr.H = z80ex_get_reg(cpu, regHL) >> 8;
    hdr.L = z80ex_get_reg(cpu, regHL) & 0xFF;
    hdr.SPh = z80ex_get_reg(cpu, regSP) >> 8;
    hdr.SPl = z80ex_get_reg(cpu, regSP) & 0xFF;
    hdr.I = z80ex_get_reg(cpu, regI);
    hdr.R = z80ex_get_reg(cpu, regR) & 0x7F;
    hdr.R7_n_misc = ((z80ex_get_reg(cpu, regR) & 0x80) >> 7) | ((border.portFB & 7) << 1);
    hdr.D = z80ex_get_reg(cpu, regDE) >> 8;
    hdr.E = z80ex_get_reg(cpu, regDE) & 0xFF;
    hdr.B_ = z80ex_get_reg(cpu, regBC_) >> 8;
    hdr.C_ = z80ex_get_reg(cpu, regBC_) & 0xFF;
    hdr.D_ = z80ex_get_reg(cpu, regDE_) >> 8;
    hdr.E_ = z80ex_get_reg(cpu, regDE_) & 0xFF;
    hdr.H_ = z80ex_get_reg(cpu, regHL_) >> 8;
    hdr.L_ = z80ex_get_reg(cpu, regHL_) & 0xFF;
    hdr.A_ = z80ex_get_reg(cpu, regAF_) >> 8;
    hdr.F_ = z80ex_get_reg(cpu, regAF_) & 0xFF;
    hdr.IYh = z80ex_get_reg(cpu, regIY) >> 8;
    hdr.IYl = z80ex_get_reg(cpu, regIY) & 0xFF;
    hdr.IXh = z80ex_get_reg(cpu, regIX) >> 8;
    hdr.IXl = z80ex_get_reg(cpu, regIX) & 0xFF;
    hdr.IFF1 = z80ex_get_reg(cpu, regIFF1);
    hdr.IFF2 = z80ex_get_reg(cpu, regIFF2);
    hdr.IM_n_misc = z80ex_get_reg(cpu, regIM);
    hdr.PCh = 0; // Z80 v2
    hdr.PCl = 0; // Z80 v2

    fl.WriteBlock(&hdr, sizeof(hdr));
    fl.PutWORD(sizeof(add_header));

    // TODO: save 48k properly

    add_header[1] = z80ex_get_reg(cpu, regPC) >> 8;
    add_header[0] = z80ex_get_reg(cpu, regPC) & 0xFF;
    add_header[2] = 9; // Pentagon
    add_header[3] = mmgr.port7FFD;

    fl.WriteBlock(add_header, sizeof(add_header));

    for (int i = 0; i < 8; i++) {
        fl.PutWORD(0xFFFF); // not compressed
        fl.PutBYTE(i + 3); // page

        for (int j = i * 0x4000; j < i * 0x4000 + 0x4000; j++) {
            fl.PutBYTE(mmgr.ram[j]);
        }
    }
}
