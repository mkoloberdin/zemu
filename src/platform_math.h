#ifndef _PLATFORM_MATH_H_INCLUDED_
#define _PLATFORM_MATH_H_INCLUDED_

// + Fix for FreeBSD (by breeze)

#include <math.h>

#ifndef log2
	#define log2(x) (log(x) / M_LN2)
#endif

// - Fix for FreeBSD (by breeze)

#endif
