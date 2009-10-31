#ifndef _WD1793_CRC_UTILS_H_INCLUDED_
#define _WD1793_CRC_UTILS_H_INCLUDED_

extern unsigned wd1793_crc(unsigned char *ptr, unsigned size);
extern unsigned short wd1793_crc16(unsigned char *buf, unsigned size);
extern void wd1793_crc32(int &crc, unsigned char *buf, unsigned len);

#endif
