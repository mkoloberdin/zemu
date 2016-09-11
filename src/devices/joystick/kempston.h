// [boo_boo]

#ifndef _KEMPSTON_H_INCLUDED_
#define _KEMPSTON_H_INCLUDED_ 1

#include <cstdint>
#include "../../zemu.h"
#include "../device.h"

class C_KempstonStick : public C_Device
{
public:

  static uint8_t joy_kbd;

  void Init(void);
  void Close(void);

  static bool OnKeyDown(SDL_Event &event);
  static bool OnKeyUp(SDL_Event &event);

  static bool InputByteCheckPort(uint16_t port);
  static bool OnInputByte(uint16_t port, uint8_t &retval);
  static void SetJoyNum(int num);

private:

  static int joy_num;
};

#endif
