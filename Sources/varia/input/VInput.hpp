#pragma once

#include "varia/VShared.hpp"
#include "varia/input/VVirtualKey.hpp"
#include "varia/input/VEventMoveRight.hpp"
#include "varia/VSystemEventQueue.hpp"

#include "kinc/input/keyboard.h"

struct Input_Modifier_State
{
    Boolean control_down;
    Boolean shift_down;
    Boolean alt_down;
};

struct Input_Virtual_Action_State
{
    Event_Move_Right move_right_action;
    //Action_Move_Left move_left_action;

    Input_Modifier_State modifiers;
};

struct Input_Event_Emitter
{
    Input_Virtual_Action_State * input;
    System_Event_Queue * events;
};

static inline Boolean v_input_virtual_key_matches_keypress(Kinc_Keycode keycode, Input_Modifier_State modifiers, Virtual_Key virtual_key)
{
    Boolean has_shift = (virtual_key.requires_shift) ? modifiers.shift_down : true;
    Boolean has_control = (virtual_key.requires_control) ? modifiers.control_down : true;
    Boolean has_alt = (virtual_key.requires_alt) ? modifiers.alt_down : true;

    Boolean has_modifiers = (has_shift & has_control & has_alt);

    if (virtual_key.bound_key == keycode && has_modifiers)
    {
        return true;
    }

    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void v_input_trigger_all_keyup_actions(Input_Event_Emitter * emitter);

void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_Event_Emitter* state*/);
void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_Event_Emitter * state*/);