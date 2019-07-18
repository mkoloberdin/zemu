#ifndef _JOYSTICK_MANAGER_H_
#define _JOYSTICK_MANAGER_H_

#include "zemu.h"

#define MAX_JOYSTICKS 5
#define DEF_JOY_AXIS_TRESHOLD 3200 // default axis movement treshold (useful for fine-tunung analog sticks)

typedef struct {
    int axis_treshold; // for fine-tunung analog sticks
    bool init_ok;
    bool up;
    bool down;
    bool left;
    bool right;
    bool fire;
} JoystickState;

class C_JoystickManager {
public:

    static C_JoystickManager* Instance();

    void Init();
    bool EnableProcessing();
    bool DisableProcessing();

    // initialize joystick [joy_num] and mark it as managed, return true if joystick was added succesfully
    bool AddJoystick(int joy_num, int axis_treshold = DEF_JOY_AXIS_TRESHOLD);

    bool ProcessJoystickEvent(ZHW_Event& event);
    static bool OnJoystickEvent(ZHW_Event& event);
    JoystickState* GetJoystickState(int joy_num);

protected:

    JoystickState jstate[MAX_JOYSTICKS];

    C_JoystickManager();

private:

    bool is_enabled;
    int num_joysticks;
    static C_JoystickManager* _instance;
};

#endif
