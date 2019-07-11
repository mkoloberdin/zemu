#include <stdio.h>
#include "snap_z80.h"
#include "exceptions.h"
#include "zemu.h"
#include "bin.h"
#include "file.h"

#define HANDLE_PRAGMA_PACK_PUSH_POP
#pragma pack(push,1)

struct Z80_snap_header {
    Z80EX_BYTE A;
    Z80EX_BYTE F;

    Z80EX_BYTE C;
    Z80EX_BYTE B;

    Z80EX_BYTE L;
    Z80EX_BYTE H;

    Z80EX_BYTE PCl;
    Z80EX_BYTE PCh;

    Z80EX_BYTE SPl;
    Z80EX_BYTE SPh;

    Z80EX_BYTE I;
    Z80EX_BYTE R;
    Z80EX_BYTE R7_n_misc; // bit0: R7, bit1-3: border, bit5: data is compressed

    Z80EX_BYTE E;
    Z80EX_BYTE D;

    Z80EX_BYTE C_;
    Z80EX_BYTE B_;

    Z80EX_BYTE E_;
    Z80EX_BYTE D_;

    Z80EX_BYTE L_;
    Z80EX_BYTE H_;

    Z80EX_BYTE A_;
    Z80EX_BYTE F_;

    Z80EX_BYTE IYl;
    Z80EX_BYTE IYh;

    Z80EX_BYTE IXl;
    Z80EX_BYTE IXh;

    Z80EX_BYTE IFF1;
    Z80EX_BYTE IFF2;

    Z80EX_BYTE IM_n_misc; // bit0-1: Interrupt mode
};

#pragma pack(pop)

bool block_read(C_File& fl, C_MemoryManager& mmgr, Z80EX_WORD mem_offset, Z80EX_WORD len, int is_compressed) {
    Z80EX_BYTE b;
    Z80EX_BYTE b_prev;
    Z80EX_BYTE rep_byte;
    Z80EX_BYTE rep_count;
    unsigned addr;
    int cb;

    for (addr = mem_offset, b = 0, b_prev = 0; addr < mem_offset + len; b_prev = b) {
        if (fl.Eof()) {
            return false;
        }

        b = fl.GetBYTE();

        if (is_compressed && b == 0xED && b_prev == 0xED) {
            if (fl.Eof()) {
                return false;
            }

            rep_count = fl.GetBYTE();

            if (fl.Eof()) {
                return false;
            }

            rep_byte = fl.GetBYTE();

            for (cb = 0; cb < rep_count; cb++) {
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
    C_File fl;
    Z80_snap_header hdr;
    Z80EX_WORD tmp;
    Z80EX_BYTE* header_add = NULL;
    Z80EX_BYTE border_color;
    Z80EX_BYTE mode;
    Z80EX_BYTE page_num;
    int is_compressed;
    int ver = 1;
    int i;

    try {
        fl.Read(filename);
    } catch (C_E &e) {
        return false;
    }

    if (fl.ReadBlock(&hdr, sizeof(hdr)) != sizeof(hdr)) {
        fl.Close();
        return false;
    }

    z80ex_set_reg(cpu, regAF, MAKEWORD(hdr.A, hdr.F));
    z80ex_set_reg(cpu, regBC, MAKEWORD(hdr.B, hdr.C));
    z80ex_set_reg(cpu, regHL, MAKEWORD(hdr.H, hdr.L));
    z80ex_set_reg(cpu, regSP, MAKEWORD(hdr.SPh, hdr.SPl));
    z80ex_set_reg(cpu, regI, hdr.I);

    if (hdr.R7_n_misc == 255) {
        hdr.R7_n_misc = 1; // compatibility issue
    }

    z80ex_set_reg(cpu, regR, ((hdr.R7_n_misc & 0x01) << 7) | (hdr.R & 0x7F));
    border_color = (hdr.R7_n_misc >> 1) & 0x07;
    is_compressed = hdr.R7_n_misc & 0x20;

    z80ex_set_reg(cpu, regDE, MAKEWORD(hdr.D, hdr.E));
    z80ex_set_reg(cpu, regBC_, MAKEWORD(hdr.B_, hdr.C_));
    z80ex_set_reg(cpu, regDE_, MAKEWORD(hdr.D_, hdr.E_));
    z80ex_set_reg(cpu, regHL_, MAKEWORD(hdr.H_, hdr.L_));
    z80ex_set_reg(cpu, regAF_, MAKEWORD(hdr.A_, hdr.F_));
    z80ex_set_reg(cpu, regIY, MAKEWORD(hdr.IYh, hdr.IYl));
    z80ex_set_reg(cpu, regIX, MAKEWORD(hdr.IXh, hdr.IXl));
    z80ex_set_reg(cpu, regIFF1, hdr.IFF1 ? 1 : 0);
    z80ex_set_reg(cpu, regIFF2, hdr.IFF2 ? 1 : 0);
    z80ex_set_reg(cpu, regIM, hdr.IM_n_misc & 0x03);

    mmgr.OnReset();

    if (hdr.PCh | hdr.PCl) { // PC != 0: Z80 v1
        // read 48k block
        if (!block_read(fl, mmgr, 0x4000, 0xC000, is_compressed)) {
            fl.Close();
            return false;
        }

        mmgr.OnOutputByte(0x7ffd, 0x30); // set 48k mode
        z80ex_set_reg(cpu, regPC, MAKEWORD(hdr.PCh, hdr.PCl));
    } else { // Z80 v2 or v3
        if (fl.Eof()) {
            fl.Close();
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

        // вроде как нет понту проверять header_addr на NULL, ибо stdc++ либа сама кинет эксцэпшн если не получиться выделить память
        header_add = new Z80EX_BYTE[tmp];

        try {
            if (fl.ReadBlock(header_add, tmp) != tmp) {
                throw C_E(E_SnapZ80Error);
            }

            z80ex_set_reg(cpu, regPC, MAKEWORD(header_add[1], header_add[0]));
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
            fl.Close();
            return false;
        }
    }

    // set border color
    border.OnOutputByte(0x00FE, border_color);
    fl.Close();

    return true;
}

void save_z80_snap(const char* filename, Z80EX_CONTEXT* cpu, C_MemoryManager& mmgr, C_Border& border) {
    C_File fl;
    Z80_snap_header hdr;
    Z80EX_BYTE add_header[23]; // Z80 v3 header
    int i;
    int j;

    fl.Write(filename);

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

    for (i = 0; i < 8; i++) {
        fl.PutWORD(0xFFFF); // not compressed
        fl.PutBYTE(i + 3); // page

        for (j = i * 0x4000; j < i * 0x4000 + 0x4000; j++) {
            fl.PutBYTE(mmgr.ram[j]);
        }
    }

    fl.Close();
}
