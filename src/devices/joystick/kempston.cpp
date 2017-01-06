// [boo_boo]

// FIXME:
#include "platform/sdl/platform.h"

#include "kempston.h"
#include "../../joystick_manager.h"

int C_KempstonStick::joy_num = -1;
uint8_t C_KempstonStick::joy_kbd = 0;

void C_KempstonStick::Init(void)
{
  AttachSDLHandler(SDL_KEYDOWN, OnKeyDown);
  AttachSDLHandler(SDL_KEYUP, OnKeyUp);

  AttachZ80InputHandler(InputByteCheckPort, OnInputByte);
}

void C_KempstonStick::Close(void)
{
}

void C_KempstonStick::SetJoyNum(int num)
{
  joy_num = num;
}

bool C_KempstonStick::OnKeyDown(SDL_Event &event)
{
  if (!joyOnKeyb) return false;

  switch (event.key.keysym.sym)
  {
  case SDLK_UP:
    joy_kbd |= 0x08;
    break;

  case SDLK_DOWN:
    joy_kbd |= 0x04;
    break;

  case SDLK_LEFT:
    joy_kbd |= 0x02;
    break;

  case SDLK_RIGHT:
    joy_kbd |= 0x01;
    break;

  case SDLK_RCTRL:
    joy_kbd |= 0x10;
    break;

  default:
    break;
  }

  return false;
}

bool C_KempstonStick::OnKeyUp(SDL_Event &event)
{
  if (!joyOnKeyb) return false;

  switch (event.key.keysym.sym)
  {
  case SDLK_UP:
    joy_kbd &= ~0x08;
    break;

  case SDLK_DOWN:
    joy_kbd &= ~0x04;
    break;

  case SDLK_LEFT:
    joy_kbd &= ~0x02;
    break;

  case SDLK_RIGHT:
    joy_kbd &= ~0x01;
    break;

  case SDLK_RCTRL:
    joy_kbd &= ~0x10;
    break;

  default:
    break;
  }

  return false;
}

bool C_KempstonStick::InputByteCheckPort(uint16_t port)
{
  return (!(port & 0x20));
}

bool C_KempstonStick::OnInputByte(uint16_t port, uint8_t &retval)
{
  retval = 0;

  if (joyOnKeyb)
  {
    retval = joy_kbd;
  }
  else if (joy_num >= 0)
  {
    JoystickState *jstate = C_JoystickManager::Instance()->GetJoystickState(joy_num);

    if (jstate->right) retval |= 0x01;
    if (jstate->left) retval |= 0x02;
    if (jstate->down) retval |= 0x04;
    if (jstate->up) retval |= 0x08;
    if (jstate->fire) retval |= 0x10;
  }

  return true;
}
