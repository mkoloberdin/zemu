ZHW_Event

// ----

int mx;
int my;
int bt = ZHW_Moyse_GetRelativeState(&mx, &my);

if (bt & ZHW_MOUSE_LMASK) { ... }
if (bt & ZHW_MOUSE_RMASK) { ... }
if (bt & ZHW_MOUSE_MMASK) { ... }

// ----

ZHW_EVENT_MOUSEWHEEL
ZHW_EVENT_WHEELDIRECTION(event) == ZHW_MOUSE_WHEELUP
ZHW_EVENT_WHEELDIRECTION(event) == ZHW_MOUSE_WHEELDOWN

// ----

ZHW_EVENT_KEYDOWN
ZHW_EVENT_KEYUP
ZHW_EVENT_OKKEY(window, event)
event.key.keysym.sym
