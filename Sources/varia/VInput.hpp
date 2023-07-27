#pragma once

#include "varia/VShared.hpp"
#include "kinc/input/keyboard.h"


typedef int Kinc_Keycode;

template <typename DATATYPE>
struct Virtual_Action
{
    Kinc_Keycode bound_key;
    Boolean requires_shift;
    Boolean requires_control;
    Boolean requires_alt;
    void (*on_keydown)(DATATYPE * data);
    void (*on_keyup)(DATATYPE * data);
    DATATYPE data;
};

struct Input_Modifier_State
{
    Boolean control_down;
    Boolean shift_down;
    Boolean alt_down;
};

template <typename DATATYPE>
void v_input_try_virtual_action_keydown(Kinc_Keycode key, Input_Modifier_State const * modifiers, Virtual_Action<DATATYPE> * action)
{
    Boolean has_shift = (action->requires_shift) ? modifiers->shift_down : true;
    Boolean has_control = (action->requires_control) ? modifiers->control_down : true;
    Boolean has_alt = (action->requires_alt) ? modifiers->alt_down : true;

    Boolean has_modifiers = (has_shift && has_control && has_alt);

    if (action->bound_key == key && has_modifiers)
    {
        action->on_keydown(address_of(action->data));
    }
}

template <typename DATATYPE>
void v_input_try_virtual_action_keyup(Kinc_Keycode key, Input_Modifier_State const * modifiers, Virtual_Action<DATATYPE> * action)
{
    //NOTE(<zshoals> 07-27-2023): These Boolean checks might not be needed on the keyup portion

    Boolean has_shift = (action->requires_shift) ? modifiers->shift_down : true;
    Boolean has_control = (action->requires_control) ? modifiers->control_down : true;
    Boolean has_alt = (action->requires_alt) ? modifiers->alt_down : true;

    Boolean has_modifiers = (has_shift && has_control && has_alt);

    if (action->bound_key == key && has_modifiers)
    {
        action->on_keyup(address_of(action->data));
    }
}

struct Action_Move_Right_Data
{
    Float_64 movement_multiplier;
    Boolean shift_toggled;
};

struct Action_Move_Left_Data
{
    Float_64 movement_multiplier;
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

struct Input_Virtual_Action_State
{
    Virtual_Action<Action_Move_Right_Data> move_right_action;
    Virtual_Action<Action_Move_Left_Data> move_left_action;
    Input_Modifier_State modifiers;
};

void v_input_register_processing_functions(Input_Virtual_Action_State * input);
void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_State * state*/);
void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_State * state*/);