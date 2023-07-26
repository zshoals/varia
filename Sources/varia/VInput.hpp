#pragma once

#include "varia/VShared.hpp"
#include "kinc/input/keyboard.h"


typedef int Kinc_Keycode;

template <typename DATATYPE>
struct Virtual_Action
{
    Kinc_Keycode bound_key;
    void (*on_keydown)(DATATYPE * data);
    void (*on_keyup)(DATATYPE * data);
    DATATYPE data;
};

template <typename DATATYPE>
void v_input_try_virtual_action_keydown(Kinc_Keycode key, Virtual_Action<DATATYPE> * action)
{
    if (action->bound_key == key)
    {
        action->on_keydown(address_of(action->data));
    }
}

template <typename DATATYPE>
void v_input_try_virtual_action_keyup(Kinc_Keycode key, Virtual_Action<DATATYPE> * action)
{
    if (action->bound_key == key)
    {
        action->on_keyup(address_of(action->data));
    }
}

struct Virtual_Move_Right_Data
{
    Float_64 movement_multiplier;
};

struct Virtual_Move_Left_Data
{
    Float_64 movement_multiplier;
};

struct Input_Virtual_Action_State
{
    // Virtual_Move_Right move_right_action;
    // Virtual_Move_Left move_left_action;
    Virtual_Action<Virtual_Move_Right_Data> move_right_action;
    Virtual_Action<Virtual_Move_Left_Data> move_left_action;
};

void v_input_register_processing_functions(Input_Virtual_Action_State * input);
void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_State * state*/);
void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_State * state*/);