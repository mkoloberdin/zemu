#ifndef _WD1793_CONST_H_INCLUDED_
#define _WD1793_CONST_H_INCLUDED_

#include "../params.h"

const int Z80FQ = MAX_FRAME_TACTS * 50;
const int FDD_RPS = 6; // rotation speed

const int MAX_TRACK_LEN = 6400;
const int MAX_CYLS = 86;            // don't load images with so many tracks
const int MAX_PHYS_CYL = 86;        // don't seek over it
const int MAX_SEC = 256;

#define SNBUF_LEN (2*1048576)

#endif
