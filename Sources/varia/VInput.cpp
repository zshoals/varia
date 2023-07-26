#include "varia/VInput.hpp"

#include "varia/VShared.hpp"
#include "varia/VGameloop.hpp"

#include "kinc/input/keyboard.h"
#include "kinc/input/mouse.h"


Input_Virtual_Action v_input_create_virtual_action(Kinc_Keycode key, Input_Callback on_keydown, Input_Callback on_keyup)
{
    Input_Virtual_Action action = ZERO_INIT();
    {
        action.bound_key = key;
        action.on_keydown = on_keydown;
        action.on_keyup = on_keyup;
    }

    return action;
}

void v_input_set_move_right(Input_State * input, Input_Virtual_Action action)
{
    input->actions[(Integer_64)(E_Input_Virtual_Action::Move_Right)] = action;
}

void v_input_set_move_left(Input_State * input, Input_Virtual_Action action)
{
    input->actions[(Integer_64)(E_Input_Virtual_Action::Move_Left)] = action;
}

void v_input_set_move_up(Input_State * input, Input_Virtual_Action action)
{
    input->actions[(Integer_64)(E_Input_Virtual_Action::Move_Up)] = action;
}

void v_input_set_move_down(Input_State * input, Input_Virtual_Action action)
{
    input->actions[(Integer_64)(E_Input_Virtual_Action::Move_Down)] = action;
}

void v_input_set_jump(Input_State * input, Input_Virtual_Action action)
{
    input->actions[(Integer_64)(E_Input_Virtual_Action::Jump)] = action;
}

void v_input_set_shoot(Input_State * input, Input_Virtual_Action action)
{
    input->actions[(Integer_64)(E_Input_Virtual_Action::Shoot)] = action;
}



//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::




void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_State * state*/)
{
    Input_State * state = static_cast<Input_State *>(data);

    for_range( (Integer_64)(E_Input_Virtual_Action::MAX_COUNT) )
    {
        if (state->actions.bound_key == key)
        {
            state->actions.on_keydown(address_of(state->proxy));
        }
    }
}

void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_State * state*/)
{
    Input_State * state = static_cast<Input_State *>(data);

    for_range( (Integer_64)(E_Input_Virtual_Action::MAX_COUNT) )
    {
        if (state->actions.bound_key == key)
        {
            state->actions.on_keyup(address_of(state->proxy));
        }
    }
}

void v_input_register_processing_functions(Input_State * input)
{
    kinc_keyboard_set_keydown_callback(&v_input_keydown_callback, input);
    kinc_keyboard_set_keyup_callback(&v_input_keydown_callback, input);
    //Mouse callbacks?
}