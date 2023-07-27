#include "varia/input/VAction_Move_Right.hpp"
#include "varia/VShared.hpp"

void v_action_move_right_keydown(Action_Move_Right_Data * data)
{
    data->movement_multiplier = 1.0f;
}

void v_action_move_right_keyup(Action_Move_Right_Data * data)
{
    data->movement_multiplier = 0.0f;
}