#include "varia/input/VInput.hpp"

#include "varia/VShared.hpp"

#include "kinc/input/keyboard.h"
#include "kinc/input/mouse.h"

//Assign standard keybinds here?
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/)
{
    Input_Virtual_Action_State * state = static_cast<Input_Virtual_Action_State *>(data);

    //NOTE(<zshoals> 07-27-2023): Special casing modifier keys because it's way easier this way...
    if (key == KINC_KEY_SHIFT) { state->modifiers.shift_down = true; }
    if (key == KINC_KEY_CONTROL) { state->modifiers.control_down = true; }
    if (key == KINC_KEY_ALT) { state->modifiers.alt_down = true; }

    v_input_try_virtual_action_keydown(key, const_address_of(state->modifiers), address_of(state->move_right_action));
    v_input_try_virtual_action_keydown(key, const_address_of(state->modifiers), address_of(state->move_left_action));
}

void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/)
{
    Input_Virtual_Action_State * state = static_cast<Input_Virtual_Action_State *>(data);

    //NOTE(<zshoals> 07-27-2023): Special casing modifier keys because it's way easier this way...
    if (key == KINC_KEY_SHIFT) { state->modifiers.shift_down = false; }
    if (key == KINC_KEY_CONTROL) { state->modifiers.control_down = false; }
    if (key == KINC_KEY_ALT) { state->modifiers.alt_down = false; }

    v_input_try_virtual_action_keyup(key, const_address_of(state->modifiers), address_of(state->move_right_action));
    v_input_try_virtual_action_keyup(key, const_address_of(state->modifiers), address_of(state->move_left_action));
}

//State transfer operations
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Action_Move_Right_Data v_input_extract_move_right_data(Input_Virtual_Action_State const * input)
{
    return input->move_right_action.data;
}



void v_input_trigger_all_keyup_actions(Input_Virtual_Action_State * input)
{
    input->move_right_action.on_keyup(address_of(input->move_right_action.data));
    // input->move_left_action.on_keyup();
}