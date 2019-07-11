#ifndef _WD1793_CRC_UTILS_H_INCLUDED_
#define _WD1793_CRC_UTILS_H_INCLUDED_

#include "wd1793_defs.h"

extern uint16_t wd1793_crc(uint8_t* ptr, unsigned size);
extern uint16_t wd1793_crc16(uint8_t* buf, unsigned size);
extern int32_t wd1793_crc32(int32_t crc, uint8_t* buf, unsigned len);

#endif
