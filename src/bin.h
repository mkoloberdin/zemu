#ifndef __BIN_RULEZ__
#define __BIN_RULEZ__ 1

#define BIN(N) ( ((N & 0x10000000) >> 21) | ((N & 0x1000000) >> 18) | ((N & 0x100000) >> 15) | ((N & 0x10000) >> 12) | ((N & 0x1000) >> 9) | ((N & 0x100) >> 6) | ((N & 0x10) >> 3) | (N & 1) )

#ifdef MAKEWORD
  #undef MAKEWORD
#endif

#define MAKEWORD(H,L) ( ((H)<<8) | (L) )

#endif
