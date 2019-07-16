// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdlib.h>
#include <string.h>
#include "wd1793_fdd.h"
#include "../defines.h"

uint8_t snbuf[SNBUF_LEN]; // large temporary buffer

C_Fdd::C_Fdd() {
    motor = 0;
    track = 0;

    rawdata = nullptr;
    rawsize = 0;
    cyls = 0;
    sides = 0;
    memset(trklen, 0, sizeof(trklen));
    memset(trkd, 0, sizeof(trkd));
    memset(trki, 0, sizeof(trki));
    optype = 0;
    snaptype = 0;
    snapsize = 0;
    is_wp = false;
    interleave = 1;

    name[0] = 0;
    dsc[0] = 0;
    trkcache = nullptr;
    appendboot[0] = 0;
}

C_Fdd::~C_Fdd() {
    free();
}

void C_Fdd::set_trkcache(C_TrkCache* tc) {
    trkcache = tc;
}

bool C_Fdd::is_wprotected() {
    return is_wp;
}

void C_Fdd::set_wprotected(bool wp) {
    is_wp = wp;
}

char* C_Fdd::get_appendboot(void) {
    return (appendboot[0] ? appendboot : nullptr);
}

void C_Fdd::set_trd_interleave(int iv) {
    interleave = iv;
}

int C_Fdd::is_disk_loaded() {
    return (rawdata != nullptr ? 1 : 0);
}

void C_Fdd::eject() {
    free();
}

int C_Fdd::save_dimage(const char* filename, enum DIMAGE_TYPE type) {
    FILE* fp;
    int ret = 0;

    if ((fp = fopen(filename, "wb")) == nullptr) {
        return 0;
    }

    switch(type) {
        case imgTRD:
            ret = write_trd(fp);
            break;

        case imgUDI:
            ret = write_udi(fp);
            break;

        case imgTD0:
            ret = write_td0(fp);
            break;

        case imgFDI:
            ret = write_fdi(fp);
            break;

        default:
            ret = 0;
    }

    fclose(fp);

    if (ret) {
        optype = 0; // reset "changed" flag after saving
    }

    return ret;
}

void C_Fdd::set_appendboot(const char* boot_name) {
    if (boot_name) {
        strncpy(appendboot, boot_name, sizeof(appendboot) - 1);
    } else {
        appendboot[0] = 0;
    }
}

char C_Fdd::is_changed() {
    return (optype ? 1 : 0);
}

void C_Fdd::free() {
    if (rawdata) {
        ::free(rawdata);
    }

    rawdata = nullptr;
    rawsize = 0;
    cyls = 0;
    sides = 0;

    memset(trklen, 0, sizeof(trklen));
    memset(trkd, 0, sizeof(trkd));
    memset(trki, 0, sizeof(trki));

    optype = 0;
    snaptype = 0;

    memset(name, 0, sizeof(name));
    memset(dsc, 0, sizeof(dsc));

    set_wprotected(false);

    if (trkcache) {
        trkcache->clear();
    }

    t.clear();
}

void C_Fdd::newdisk(unsigned cyls, unsigned sides) { //-V688
    free();

    this->cyls = cyls;
    this->sides = sides;

    unsigned len = MAX_TRACK_LEN;
    unsigned len2 = len + (len / 8) + ((len & 7) ? 1 : 0); //-V547

    rawsize = align_by(cyls * sides * len2, 4096);
    rawdata = (uint8_t*)malloc(rawsize);

    if (!rawdata) {
        StrikeError("Failed to allocate %zu bytes of memory", rawsize);
    }

    memset(rawdata, 0, rawsize);

    for (unsigned i = 0; i < cyls; i++) {
        for (unsigned j = 0; j < sides; j++) {
            trklen[i][j] = len;
            trkd[i][j] = rawdata + len2 * (i * sides + j);
            trki[i][j] = trkd[i][j] + len;
        }
    }
}

int C_Fdd::read(uint8_t type) {
    int ok = 0;

    if (type == snTRD) {
        ok = read_trd();
    } else if (type == snUDI) {
        ok = read_udi();
    } else if (type == snHOB) {
        ok = read_hob();
    } else if (type == snSCL) {
        ok = read_scl();
    } else if (type == snFDI) {
        ok = read_fdi();
    } else if (type == snTD0) {
        ok = read_td0();
    }

    return ok;
}

int C_Fdd::load_dimage(const char* filename) {
    uint8_t type = what_is(filename);

    if (!read(type)) {
        return 0;
    }

    if (get_appendboot()) {
        addboot();
    }

    strcpy(name, filename);

    // TODO
    // if (GetFileAttributes(filename) & FILE_ATTRIBUTE_READONLY) { set_wprotected(1); }

    snaptype = ((type == snHOB || type == snSCL) ? snTRD : type);
    return 1;
}

uint8_t C_Fdd::what_is(const char* filename) {
    FILE *ff = fopen(filename, "rb");

    if (!ff) {
        return snNOFILE;
    }

    snapsize = fread(snbuf, 1, sizeof(snbuf), ff);
    fclose(ff);

    if (snapsize >= sizeof(snbuf)) { // '>=' just for case
        return snTOOLARGE;
    }

    uint8_t type = snUNKNOWN;

    if (snapsize < 32) {
        return type;
    }

    const char* ptr = strrchr(filename, '.');

    unsigned ext = ((ptr && ((unsigned)((ptr - filename) + 3) < strlen(filename)))
        ? WORD4(ptr[1] | 0x20, ptr[2] | 0x20, ptr[3] | 0x20, ptr[4] | 0x20)
        : 0
    );

    if (!snbuf[13] && snbuf[14] && (int)snapsize == (snbuf[14] * 256 + 17)) {
        type = snHOB;
    } else if (snapsize >= 8192 && !(snapsize & 0xFF) && ext == WORD4('t','r','d',' ')) {
        type = snTRD;
    } else if (!memcmp(snbuf, "SINCLAIR", 8) && (int)snapsize >= (9 + (0x100 + 14) * snbuf[8])) {
        type = snSCL;
    } else if (!memcmp(snbuf, "FDI", 3)
        && WORD2(snbuf[4], snbuf[5]) <= (unsigned)MAX_CYLS
        && WORD2(snbuf[6], snbuf[7]) <= (unsigned)2
    ) {
        type = snFDI;
    } else if (WORD2(snbuf[0] | 0x20, snbuf[1] | 0x20) == WORD2('t', 'd')
        && snbuf[4] >= 10
        && snbuf[4] <= 21
        && snbuf[9] <= 2
    ) {
        type = snTD0;
    } else if (WORD4(snbuf[0], snbuf[1], snbuf[2], snbuf[3]) == WORD4('U','D','I','!')
        && WORD4(snbuf[4], snbuf[5], snbuf[6], snbuf[7]) == (unsigned)(snapsize - 4)
        && snbuf[9] < MAX_CYLS
        && snbuf[10] < 2
        && !snbuf[8]
    ) {
        type = snUDI;
    }

    return type;
}
