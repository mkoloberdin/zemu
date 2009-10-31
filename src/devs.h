#ifndef _DEVS_H_INCLUDED_
#define _DEVS_H_INCLUDED_ 1

#include "devices/device.h"
#include "devices/border/border.h"
#include "devices/extport/extport.h"
#include "devices/keyboard/keyboard.h"
#include "devices/trdos/trdos.h"
#include "devices/mmanager/mmanager.h"
#include "devices/tsfm/tsfm.h"
#include "devices/mouse/mouse.h"
#include "devices/covox/covox.h"
#include "devices/joystick/kempston.h"		// [boo_boo]

extern C_Border dev_border;
extern C_ExtPort dev_extport;
extern C_Keyboard dev_keyboard;
extern C_TrDos dev_trdos;
extern C_MemoryManager dev_mman;
extern C_TsFm dev_tsfm;
extern C_Mouse dev_mouse;
extern C_Covox dev_covox;
extern C_KempstonStick dev_kempston;	// [boo_boo]

#endif
