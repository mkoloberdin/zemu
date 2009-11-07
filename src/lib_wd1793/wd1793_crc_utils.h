#ifndef _WD1793_CRC_UTILS_H_INCLUDED_
#define _WD1793_CRC_UTILS_H_INCLUDED_

#include "wd1793_defs.h"

extern unsigned wd1793_crc(uint8_t *ptr, unsigned size);
extern unsigned short wd1793_crc16(uint8_t *buf, unsigned size);
extern void wd1793_crc32(int &crc, uint8_t *buf, unsigned len);

#endif
