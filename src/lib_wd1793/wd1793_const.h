#ifndef _WD1793_CONST_H_INCLUDED_
#define _WD1793_CONST_H_INCLUDED_

#include "params.h"

const int64_t Z80FQ = MAX_FRAME_TACTS * 50;

#define TACTS_PER_ROTATE(n) ((n) * (Z80FQ) / 5)
#define DELAY_IN_MS(m) ((m) * (Z80FQ) / 1000)

const int TACTS_PER_INDEX = DELAY_IN_MS(4);
// const int TACTS_PER_INDEX_HACK = DELAY_IN_MS(5 / 2);
const int TACTS_PER_INDEX_HACK = (DELAY_IN_MS(1) / 10);

const int MAX_TRACK_LEN = 6400;
const int MAX_CYLS = 86; // don't load images with so many tracks
const int MAX_PHYS_CYL = 86; // don't seek over it
const int MAX_SEC = 256;

#define SNBUF_LEN (2 * 1048576)

#endif
