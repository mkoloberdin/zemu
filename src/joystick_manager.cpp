// [boo_boo]

#include "joystick_manager.h"
#include "exceptions.h"
#include "devs.h"
#include "zemu_env.h"

C_JoystickManager *C_JoystickManager::_instance = 0;

C_JoystickManager *C_JoystickManager::Instance()
{
  if (!_instance) _instance = new C_JoystickManager;
  return _instance;
}

// [rst]
void C_JoystickManager::Init()
{
  params.kempstonEnable = env.getBool("kempstonjoystick", "enable", false);

  if (params.kempstonEnable)
  {
    params.kempstonOnStickNum = env.getInt("kempstonjoystick", "sysjoysticknum", -1);
    params.kempstonAxisTreshold = env.getInt("kempstonjoystick", "axisthreshold", DEF_JOY_AXIS_TRESHOLD);

    // если номер джойстика < 0, то кемпстоном можно будет управлять только с клавиатуры
    if (params.kempstonOnStickNum >= 0)
    {
      if (AddJoystick(params.kempstonOnStickNum, params.kempstonAxisTreshold))
      {
        dev_kempston.SetJoyNum(params.kempstonOnStickNum);
        EnableProcessing();
      }
      else
      {
        StrikeMessage("Error initializing kempston joystick");
      }
    }
  }
}
// [/rst]

C_JoystickManager::C_JoystickManager()
{
  if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) throw C_E(E_General, "cannot initialize SDL joystick subsystem");

  SDL_JoystickEventState(SDL_ENABLE);

  memset(jstate, 0, sizeof(JoystickState) * MAX_JOYSTICKS);
  num_joysticks = SDL_NumJoysticks();

  if (num_joysticks > MAX_JOYSTICKS) num_joysticks = MAX_JOYSTICKS;
  is_enabled = false;

  // printf("%d joysticks available\n",num_joysticks);
  AttachSDLHandler(SDL_JOYAXISMOTION, OnJoystickEvent);
  AttachSDLHandler(SDL_JOYBUTTONDOWN, OnJoystickEvent);
  AttachSDLHandler(SDL_JOYBUTTONUP, OnJoystickEvent);
}

bool C_JoystickManager::AddJoystick(int joy_num, int axis_treshold)
{
  SDL_Joystick *joy;

  if (joy_num >= num_joysticks || nullptr == (joy = SDL_JoystickOpen(joy_num))) return false;

  jstate[joy_num].init_ok = true;
  jstate[joy_num].axis_treshold = axis_treshold;

  return true;
}

bool C_JoystickManager::OnJoystickEvent(SDL_Event &event)
{
  return C_JoystickManager::Instance()->ProcessJoystickEvent(event);
}

bool C_JoystickManager::ProcessJoystickEvent(SDL_Event &event)
{
  int axis_state, joy_num;
  bool ev_processed = false;

  if (!is_enabled) return false;

  // printf("joystick event %d: ",event.type);
  switch (event.type)
  {
  case SDL_JOYAXISMOTION:
    // printf("joy %d axis %d moved by %d\n",event.jaxis.which,event.jaxis.axis,event.jaxis.value);
    joy_num = event.jaxis.which;
    if (joy_num >= num_joysticks || !jstate[joy_num].init_ok) break;

    axis_state = 0;

    if (event.jaxis.value < -jstate[joy_num].axis_treshold) axis_state = -1;
    else if (event.jaxis.value > jstate[joy_num].axis_treshold) axis_state = 1;

    switch (event.jaxis.axis)
    {
    case 0: // axis 0: left-right
      if (axis_state > 0) {
        jstate[joy_num].right = true;
        jstate[joy_num].left = false;
      }
      else if (axis_state < 0) {
        jstate[joy_num].left = true;
        jstate[joy_num].right = false;
      }
      else jstate[joy_num].left = jstate[joy_num].right = false;

      ev_processed = true;
      break;

    case 1: // axis 1: up-down
      if (axis_state > 0) {
        jstate[joy_num].down = true;
        jstate[joy_num].up = false;
      }
      else if (axis_state < 0) {
        jstate[joy_num].up = true;
        jstate[joy_num].down = false;
      }
      else jstate[joy_num].up = jstate[joy_num].down = false;

      ev_processed = true;
      break;
    }
    break;

  case SDL_JOYBUTTONDOWN:
    // printf("button down\n");
    joy_num = event.jbutton.which;
    jstate[joy_num].fire = true;
    ev_processed = true;
    break;

  case SDL_JOYBUTTONUP:
    // printf("button up\n");
    joy_num = event.jbutton.which;
    jstate[joy_num].fire = false;
    ev_processed = true;
    break;
  }

  return ev_processed;
}

JoystickState *C_JoystickManager::GetJoystickState(int joy_num)
{
  if (!is_enabled || joy_num >= num_joysticks || !jstate[joy_num].init_ok) throw C_E(E_General, "cannot read state for this joystick");
  return &jstate[joy_num];
}

bool C_JoystickManager::EnableProcessing()
{
  if (!num_joysticks) return false;
  return (is_enabled = true);
}

bool C_JoystickManager::DisableProcessing()
{
  if (!is_enabled) return false;
  return (is_enabled = false);
}
