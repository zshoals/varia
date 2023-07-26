#include "varia/VInput.hpp"

#include "varia/VShared.hpp"

#include "kinc/input/keyboard.h"
#include "kinc/input/mouse.h"


void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/)
{
    Input_Virtual_Action_State * state = static_cast<Input_Virtual_Action_State *>(data);

    v_input_try_virtual_action_keydown(key, address_of(state->move_right_action));
    v_input_try_virtual_action_keydown(key, address_of(state->move_left_action));
}

void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/)
{
    Input_Virtual_Action_State * state = static_cast<Input_Virtual_Action_State *>(data);

    v_input_try_virtual_action_keyup(key, address_of(state->move_right_action));
    v_input_try_virtual_action_keyup(key, address_of(state->move_left_action));
}

void v_input_register_processing_functions(Input_Virtual_Action_State * input)
{
    kinc_keyboard_set_key_down_callback(&v_input_keydown_callback, input);
    kinc_keyboard_set_key_up_callback(&v_input_keydown_callback, input);
    //Mouse callbacks?
}