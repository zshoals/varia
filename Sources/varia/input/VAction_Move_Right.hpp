#pragma once

#include "varia/VShared.hpp"
#include "kinc/log.h"

//#include "event.hpp"

/*
struct Event
{
    tag
    union of data
    {

    }
}
*/

struct Action_Move_Right_Data
{
    Float_64 movement_multiplier;
};

void v_action_move_right_keydown(Action_Move_Right_Data * data);
void v_action_move_right_keyup(Action_Move_Right_Data * data);