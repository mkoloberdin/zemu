#ifndef __BIN_RULEZ__
#define __BIN_RULEZ__ 1

#ifdef MAKEWORD
  #undef MAKEWORD
#endif

#define MAKEWORD(H,L) ( ((H)<<8) | (L) )

#endif
