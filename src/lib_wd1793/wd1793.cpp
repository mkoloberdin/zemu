// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdlib.h>
#include <string.h>
#include "wd1793.h"
#include "wd1793_crc_utils.h"

void AddLog(const char* fmt, ...);
extern bool wdDebug;

C_Wd1793::C_Wd1793() {
    for (int i = 0; i < 4; i++) {
        fdd[i].set_trkcache(&trkcache);
        fdd[i].set_appendboot(nullptr);
    }

    seldrive = &fdd[0];
    wd93_nodelay = 0;
}

int C_Wd1793::load_dimage(const char* filename, int drive) {
    return fdd[drive].load_dimage(filename);
}

void C_Wd1793::eject_dimage(int drive) {
    fdd[drive].eject();
}

int C_Wd1793::is_disk_changed(int drive) {
    return fdd[drive].is_changed();
}

void C_Wd1793::set_nodelay(int nodelay) {
    wd93_nodelay = nodelay;
}

uint8_t C_Wd1793::get_status_reg() {
    return status;
}

void C_Wd1793::set_appendboot(const char* boot_name) {
    for (int i = 0; i < 4; i++) {
        fdd[i].set_appendboot(boot_name);
    }
}

void C_Wd1793::set_trd_interleave(int iv) {
    for (int i = 0; i < 4; i++) {
        fdd[i].set_trd_interleave(iv);
    }
}

int C_Wd1793::save_dimage(char* filename, int drive, enum DIMAGE_TYPE type) {
    return fdd[drive].save_dimage(filename, type);
}

int C_Wd1793::is_disk_loaded(int drive) {
    return fdd[drive].is_disk_loaded();
}

void C_Wd1793::set_disk_wprotected(int drive, bool wp) {
    fdd[drive].set_wprotected(wp);
}

bool C_Wd1793::is_disk_wprotected(int drive) {
    return fdd[drive].is_wprotected();
}

int C_Wd1793::get_current_drive() {
    return (seldrive - fdd);
}

int C_Wd1793::is_drive_spinning(int drive, int64_t tm) {
    if (fdd[drive].motor < tm) {
        fdd[drive].motor = 0;
    }

    return (fdd[drive].motor ? 1 : 0);
}

unsigned C_Wd1793::get_state() {
    return state2;
}

int C_Wd1793::get_drive_head(int drive) {
    return fdd[drive].track;
}

int C_Wd1793::process() {
    static bool is_index = false;
    static bool read_track = false;

    static unsigned last_track = 0;
    static unsigned last_drvTrack = 0;
    static int64_t last_stepTime = 0;

    // inactive drives disregard HLT bit

    if ((time > seldrive->motor) && (system & 0x08)) {
        seldrive->motor = 0;
    }

    if (seldrive->is_disk_loaded()) {
        status &= ~WDS_NOTRDY;
    } else {
        status |= WDS_NOTRDY;
    }

    if (!(cmd & 0x80)) {
        // seek / step commands

        status &= ~(WDS_TRK00 | WDS_INDEX);

        if (seldrive->motor && (system & 0x08)) {
            status |= WDS_HEADL;
        }

        if (!seldrive->track) {
            status |= WDS_TRK00;
        }

        // TODO: test spinning

        if (seldrive->rawdata && seldrive->motor && (((time + tshift) % TACTS_PER_ROTATE(1)) < TACTS_PER_INDEX)) {
            status |= WDS_INDEX; // index every turn, len=4ms (if disk present)
            is_index = true;
        } else {
            is_index = false;
        }
    }

    for (;;) {
        switch (state) {
            case S_IDLE:
                status &= ~WDS_BUSY;
                rqs = INTRQ;
                return 1;

            case S_WAIT:
                if (time < next) {
                    return 1;
                }

                state = state2;
                is_index = (seldrive->rawdata && seldrive->motor && (((time + tshift) % TACTS_PER_ROTATE(1)) < TACTS_PER_INDEX));
                break;

            case S_DELAY_BEFORE_CMD:
                if (!wd93_nodelay && (cmd & CMD_DELAY)) {
                    next += DELAY_IN_MS(15);
                }

                status = (status | WDS_BUSY) & ~(WDS_DRQ | WDS_LOST | WDS_NOTFOUND | WDS_RECORDT | WDS_WRITEP);
                state2 = S_CMD_RW;
                state = S_WAIT;
                break;

            case S_CMD_RW:
                if (((cmd & 0xE0) == 0xA0 || (cmd & 0xF0) == 0xF0) && fdd[drive].is_wprotected()) {
                    status |= WDS_WRITEP;
                    state = S_IDLE;
                    break;
                }

                if ((cmd & 0xC0) == 0x80 || (cmd & 0xF8) == 0xC0) {
                    // read/write sectors or read am - find next AM
                    end_waiting_am = next + TACTS_PER_ROTATE(5); // max wait disk 5 turns
                    find_marker();
                    break;
                }

                if ((cmd & 0xF8) == 0xF0) {
                    // write track
                    rqs = DRQ;
                    status |= WDS_DRQ;
                    next += 3 * trkcache.ts_byte;
                    state2 = S_WRTRACK;
                    state = S_WAIT;
                    break;
                }

                if ((cmd & 0xF8) == 0xE0) {
                    // read track
                    load();
                    state2 = S_READ;
                    getindex();
                    read_track = true;
                    break;
                }

                // else unknown command
                state = S_IDLE;
                break;

            case S_FOUND_NEXT_ID:
                if (!seldrive->rawdata) {
                    // no disk - wait again
                    end_waiting_am = next + TACTS_PER_ROTATE(5);
                    find_marker();
                    break;
                }

                if (next >= end_waiting_am || (foundid == ~(unsigned)0)) {
                    status |= WDS_NOTFOUND;
                    state = S_IDLE;
                    break;
                }

                status &= ~WDS_CRCERR;
                load();

                if (!(cmd & 0x80)) {
                    // verify after seek
                    if (trkcache.hdr[foundid].c != track) {
                        find_marker();
                        break;
                    }

                    if (!trkcache.hdr[foundid].c1) {
                        status |= WDS_CRCERR;
                        find_marker();
                        break;
                    }

                    state = S_IDLE;
                    break;
                }

                if ((cmd & 0xF0) == 0xC0) {
                    // read AM
                    rwptr = trkcache.hdr[foundid].id - trkcache.trkd;
                    rwlen = 6;

read_first_byte:    data = trkcache.trkd[rwptr++];
                    rwlen--;
                    rqs = DRQ;
                    status |= WDS_DRQ;

                    next += trkcache.ts_byte;
                    state = S_WAIT;
                    state2 = S_READ;
                    read_track = false;
                    break;
                }

                // else R/W sector(s)

                if (trkcache.hdr[foundid].c != track || trkcache.hdr[foundid].n != sector) {
                    find_marker();
                    break;
                }

                if ((cmd & CMD_SIDE_CMP_FLAG) && (((cmd >> CMD_SIDE_SHIFT) ^ trkcache.hdr[foundid].s) & 1)) {
                    find_marker();
                    break;
                }

                if (!trkcache.hdr[foundid].c1) {
                    status |= WDS_CRCERR;
                    find_marker();
                    break;
                }

                if (cmd & 0x20) {
                    // write sector(s)
                    rqs = DRQ;
                    status |= WDS_DRQ;
                    next += trkcache.ts_byte * 9;
                    state = S_WAIT;
                    state2 = S_WRSEC;
                    break;
                }

                // read sector(s)

                if (!trkcache.hdr[foundid].data) {
                    find_marker();
                    break;
                }

                if (trkcache.hdr[foundid].data[-1] == 0xF8) {
                    status |= WDS_RECORDT;
                } else {
                    status &= ~WDS_RECORDT;
                }

                rwptr = trkcache.hdr[foundid].data - trkcache.trkd;
                rwlen = (128 << trkcache.hdr[foundid].l);
                goto read_first_byte;

            case S_READ:
                if (notready()) {
                    break;
                }

                load();

                if (rwlen) {
                    if (rqs & DRQ) {
                        status |= WDS_LOST;
                    }

                    // существует ненулевая вероятность, что из-за этого фикса что-нибудь перестанет работать
                    if (!wd93_nodelay && read_track && is_index && (((time + tshift) % TACTS_PER_ROTATE(1)) > TACTS_PER_INDEX_HACK)) {
                        status |= WDS_INDEX;
                        state = S_IDLE;
                        break;
                    }

                    data = trkcache.trkd[rwptr++];
                    rwlen--;
                    rqs = DRQ;
                    status |= WDS_DRQ;

                    if (!wd93_nodelay) {
                        next += trkcache.ts_byte;
                    }

                    state = S_WAIT;
                    state2 = S_READ;
                } else {
                    if ((cmd & 0xE0) == 0x80) {
                        // read sector
                        if (!trkcache.hdr[foundid].c2) {
                            status |= WDS_CRCERR;
                        }

                        if (cmd & CMD_MULTIPLE) {
                            sector++;
                            state = S_CMD_RW;
                            break;
                        }
                    }

                    if ((cmd & 0xF0) == 0xC0) { // read address
                        if (!trkcache.hdr[foundid].c1) {
                            status |= WDS_CRCERR;
                        }
                    }

                    state = S_IDLE;
                }

                break;

            case S_WRSEC:
                load();

                if (rqs & DRQ) {
                    status |= WDS_LOST;
                    state = S_IDLE;
                    break;
                }

                seldrive->optype |= 1;
                rwptr = trkcache.hdr[foundid].id + 6 + 11 + 11 - trkcache.trkd;

                for (rwlen = 0; rwlen < 12; rwlen++) {
                    trkcache.write(rwptr++, 0, 0);
                }

                for (rwlen = 0; rwlen < 3; rwlen++) {
                    trkcache.write(rwptr++, 0xA1, 1);
                }

                trkcache.write(rwptr++, (cmd & CMD_WRITE_DEL) ? 0xF8 : 0xFB, 0);
                rwlen = (128 << trkcache.hdr[foundid].l);
                state = S_WRITE;
                break;

            case S_WRITE:
                if (notready()) {
                    break;
                }

                if (rqs & DRQ) {
                    status |= WDS_LOST;
                    data = 0;
                }

                trkcache.write(rwptr++, data, 0);
                rwlen--;

                if (rwptr >= trkcache.trklen) {
                    rwptr = 0;
                }

                trkcache.sf = JUST_SEEK; // invalidate sectors

                if (rwlen) {
                    if (!wd93_nodelay) {
                        next += trkcache.ts_byte;
                    }

                    state = S_WAIT;
                    state2 = S_WRITE;
                    rqs = DRQ;
                    status |= WDS_DRQ;
                } else {
                    unsigned len = (128 << trkcache.hdr[foundid].l) + 1;
                    uint8_t sc[2056];

                    if (rwptr < len) {
                        memcpy(sc, trkcache.trkd + trkcache.trklen - rwptr, rwptr), memcpy(sc + rwptr, trkcache.trkd, len - rwptr);
                    } else {
                        memcpy(sc, trkcache.trkd + rwptr - len, len);
                    }

                    unsigned crc = wd1793_crc(sc, len);
                    trkcache.write(rwptr++, (uint8_t)(crc & 0xFF), 0);
                    trkcache.write(rwptr++, (uint8_t)(crc >> 8), 0);
                    trkcache.write(rwptr, 0xFF, 0);

                    if (cmd & CMD_MULTIPLE) {
                        sector++;
                        state = S_CMD_RW;
                        break;
                    }

                    state = S_IDLE;
                }

                break;

            case S_WRTRACK:
                if (rqs & DRQ) {
                    status |= WDS_LOST;
                    state = S_IDLE;
                    break;
                }

                seldrive->optype |= 2;
                state2 = S_WR_TRACK_DATA;
                getindex();
                end_waiting_am = next + TACTS_PER_ROTATE(5);
                break;

            case S_WR_TRACK_DATA: {
                if (notready()) {
                    break;
                }

                if (rqs & DRQ) {
                    status |= WDS_LOST;
                    data = 0;
                }

                // существует ненулевая вероятность, что из-за этого фикса что-нибудь перестанет работать
                if (!wd93_nodelay && is_index && (((time + tshift) % TACTS_PER_ROTATE(1)) > TACTS_PER_INDEX_HACK)) {
                    status |= WDS_INDEX;
                    state = S_IDLE;
                    break;
                }

                trkcache.seek(seldrive, seldrive->track, side, JUST_SEEK);
                trkcache.sf = JUST_SEEK; // invalidate sectors

                if (!trkcache.trkd) {
                    state = S_IDLE;
                    break;
                }

                uint8_t marker = 0;
                uint8_t byte = data;
                unsigned crc = 0;

                if (data == 0xF5) {
                    byte = 0xA1;
                    marker = 1;
                    start_crc = rwptr + 1;
                } else if (data == 0xF6) {
                    byte = 0xC2;
                    marker = 1;
                } else if (data == 0xF7) {
                    crc = wd1793_crc(trkcache.trkd + start_crc, rwptr - start_crc);
                    byte = crc & 0xFF;
                }

                trkcache.write(rwptr++, byte, marker);
                rwlen--;

                if (data == 0xF7) {
                    trkcache.write(rwptr++, (uint8_t)(crc >> 8), 0);
                    rwlen--; // second byte of CRC16
                }

                if ((int)rwlen > 0) {
                    if (!wd93_nodelay) {
                        next += trkcache.ts_byte;
                    }

                    state2 = S_WR_TRACK_DATA;
                    state = S_WAIT;
                    rqs = DRQ;
                    status |= WDS_DRQ;
                    break;
                }

                state = S_IDLE;
                break;
            }

            case S_TYPE1_CMD:
                status = (status | WDS_BUSY) & ~(WDS_DRQ | WDS_CRCERR | WDS_SEEKERR | WDS_WRITEP);

                if (is_index) {
                    status |= WDS_INDEX;
                }

                rqs = 0;

                if (fdd[drive].is_wprotected()) {
                    status |= WDS_WRITEP;
                }

                seldrive->motor = next + DELAY_IN_MS(2000);
                state2 = S_SEEKSTART; // default is seek/restore

                if (cmd & 0xE0) {
                    // single step
                    if (cmd & 0x40) {
                        stepdirection = (cmd & CMD_SEEK_DIR) ? -1 : 1;
                    }

                    state2 = S_STEP;
                }

                if (!wd93_nodelay) {
                    next += DELAY_IN_MS(1) / 8;
                }

                state = S_WAIT;
                break;

            case S_STEP: {
                // TRK00 sampled only in RESTORE command
                if (!seldrive->track && !(cmd & 0xF0)) {
                    track = 0;
                    state = S_VERIFY;
                    break;
                }

                last_track = track;
                last_drvTrack = seldrive->track;
                last_stepTime = next;

                if (!(cmd & 0xE0) || (cmd & CMD_SEEK_TRKUPD)) {
                    track += stepdirection;
                }

                seldrive->track += stepdirection;

                if (seldrive->track == (uint8_t) -1) {
                    seldrive->track = 0;
                }

                if (seldrive->track >= MAX_PHYS_CYL) {
                    seldrive->track = MAX_PHYS_CYL - 1;
                }

                trkcache.clear();
                static const unsigned steps[] = { 6, 12, 20, 30 };

                if (!wd93_nodelay) {
                    next += DELAY_IN_MS(steps[cmd & CMD_SEEK_RATE]);
                }

                // TODO: fdd noise
                // if (!wd93_nodelay && conf.fdd_noise) { Beep((stepdirection > 0)? 600 : 800, 2); }

                state2 = ((cmd & 0xE0) ? S_VERIFY : S_SEEK);
                state = S_WAIT;
                break;
            }

            case S_SEEKSTART:
                if (!(cmd & 0x10)) {
                    track = 0xFF;
                    data = 0;
                }

                // fallthrough

            case S_SEEK:
                if (data == track) {
                    state = S_VERIFY;
                    break;
                }

                stepdirection = (data < track) ? -1 : 1;
                state = S_STEP;
                break;

            case S_VERIFY:
                if (!(cmd & CMD_SEEK_VERIFY)) {
                    state = S_IDLE;
                    break;
                }

                end_waiting_am = next + TACTS_PER_ROTATE(6); // max wait disk 6 turns
                find_marker();
                break;

            case S_RESET:
                /*
                // seek to trk0, but don't be busy
                if (!seldrive->track) {
                    state = S_IDLE;
                } else {
                    seldrive->track--;
                    trkcache.clear();
                }

                // if (!seldrive->track) { track = 0; }
                next += DELAY_IN_MS(6);
                */

                state = S_IDLE;

                if ((next - last_stepTime) < DELAY_IN_MS(10)) { // для BestView
                    track = last_track;
                    seldrive->track = last_drvTrack;
                    trkcache.clear();
                }

                break;

            default:
                return 0;
        }
    }
}

void C_Wd1793::find_marker() {
    if (wd93_nodelay && seldrive->track != track) {
        seldrive->track = track;
    }

    load();

    foundid = ~0;
    unsigned wait = TACTS_PER_ROTATE(10);

    if (seldrive->motor && seldrive->rawdata) {
        unsigned div = trkcache.trklen * trkcache.ts_byte;
        unsigned i = (unsigned)((next + tshift) % div) / trkcache.ts_byte;
        wait = ~(unsigned)0;

        for (unsigned is = 0; is < trkcache.s; is++) {
            unsigned pos = trkcache.hdr[is].id - trkcache.trkd;
            unsigned dist = ((pos > i) ? (pos - i) : (trkcache.trklen + pos - i));

            if (dist < wait) {
                wait = dist;
                foundid = is;
            }
        }

        if (foundid != ~(unsigned)0) {
            wait *= trkcache.ts_byte;
        } else {
            wait = TACTS_PER_ROTATE(10);
        }

        if (wd93_nodelay && (foundid != ~(unsigned)0)) {
            // adjust tshift, that id appares right under head
            unsigned pos = trkcache.hdr[foundid].id - trkcache.trkd + 2;
            tshift = (unsigned)(((pos * trkcache.ts_byte) - (next % div) + div) % div);
            wait = 100; // delay=0 causes fdc to search infinitely, when no matched id on track
        }
    }

    // else no index pulses - infinite wait, but now wait 10spins, and re-test if disk inserted
    next += wait;

    if (seldrive->rawdata && next > end_waiting_am) {
        next = end_waiting_am;
        foundid = ~0;
    }

    state = S_WAIT;
    state2 = S_FOUND_NEXT_ID;
}

char C_Wd1793::notready() {
    // fdc is may be too fast, so wait until cpu handles DRQ, but not more 'end_waiting_am'
    if (!(rqs & DRQ) || next > end_waiting_am) {
        return 0;
    }

    state2 = state;
    state = S_WAIT;
    next += trkcache.ts_byte;

    return 1;
}

void C_Wd1793::getindex() {
    // unsigned trlen = trkcache.trklen * trkcache.ts_byte;
    // unsigned ticks = (unsigned)((next + tshift) % trlen);

    if (!wd93_nodelay) {
        // next += (trlen - ticks);
        unsigned tmp = (next + tshift) % TACTS_PER_ROTATE(1);

        if (tmp) {
            tmp = TACTS_PER_ROTATE(1) - tmp;
        }

        next += tmp + DELAY_IN_MS(4);
    }

    rwptr = 0;
    rwlen = trkcache.trklen;
    state = S_WAIT;
}

void C_Wd1793::load() {
    trkcache.seek(seldrive, seldrive->track, side, LOAD_SECTORS);
}

uint8_t C_Wd1793::in(uint8_t port, int64_t ttime, int* err) {
    time = ttime;
    *err = 0;

    if (wdDebug) {
        AddLog("%08X %08X: %02X", (unsigned)(time >> 32), (unsigned)(time & (int64_t)(0xFFFFFFFF)), port);
    }

    if (!process()) {
        *err = 1;
        return 0;
    }

    if (port & 0x80) {
        return (rqs | 0x3F);
    }

    if (port == 0x1F) {
        rqs &= ~INTRQ;
        return status;
    }

    if (port == 0x3F) {
        return track;
    }

    if (port == 0x5F) {
        return sector;
    }

    if (port == 0x7F) {
        status &= ~WDS_DRQ;
        rqs &= ~DRQ;
        return data;
    }

    return 0xFF;
}

void C_Wd1793::out(uint8_t port, uint8_t val, int64_t ttime, int* err) {
    time = ttime;
    *err = 0;

    if (!process()) {
        *err = 1;
        return;
    }

    // cmd
    if (port == 0x1F) {
        // force interrupt
        if ((val & 0xF0) == 0xD0) {
            state = S_IDLE;
            rqs = INTRQ;
            status &= ~WDS_BUSY;
            return;
        }

        if (status & WDS_BUSY) {
            if (wdDebug) {
                AddLog("BUSY");
            }

            return;
        }

        cmd = val;
        next = ttime;
        status |= WDS_BUSY;
        rqs = 0;

        // read/write command
        if (cmd & 0x80) {
            // abort if no disk
            if (status & WDS_NOTRDY) {
                state = S_IDLE;
                rqs = INTRQ;
                return;
            }

            // continue disk spinning
            if (seldrive->motor || wd93_nodelay) {
                seldrive->motor = next + DELAY_IN_MS(2000);
            }

            state = S_DELAY_BEFORE_CMD;
            return;
        }

        // else seek/step command
        state = S_TYPE1_CMD;
        return;
    }

    if (port == 0x3F) {
        track = val;
        return;
    }

    if (port == 0x5F) {
        sector = val;
        return;
    }

    if (port == 0x7F) {
        data = val;
        rqs &= ~DRQ;
        status &= ~WDS_DRQ;
        return;
    }

    // system
    if (port & 0x80) {
        system = val;
        drive = val & 3;
        side = (1 & ~(val >> 4));
        seldrive = fdd + drive;
        trkcache.clear();

        // reset
        // TODO: возможно reset долнен происходить по перепаду сигнала (0/1)
        if (!(val & 0x04)) {
            status = WDS_NOTRDY;
            rqs = INTRQ;
            seldrive->motor = 0;
            state = S_RESET;

            // move head to trk00
            // steptime = DELAY_IN_MS(6); // 6ms
            // next += DELAY_IN_MS(1); // 1ms before command
            // seldrive->track = 0;
        }
    }
}
