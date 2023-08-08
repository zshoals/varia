#include "varia/input/VInput.hpp"

#include "varia/VShared.hpp"

#include "kinc/input/keyboard.h"
#include "kinc/input/mouse.h"

static inline Boolean v_input_virtual_key_matches_keypress(Kinc_Keycode keycode, Input_Modifier_State const & modifiers, Input_Virtual_Key const & virtual_key)
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


static inline void v_input_configure_virtual_key(Input_Virtual_Key * virtual_key, Kinc_Keycode key, Boolean use_shift, Boolean use_control, Boolean use_alt)
{
    virtual_key->bound_key = key;
    virtual_key->requires_shift = use_shift;
    virtual_key->requires_control = use_control;
    virtual_key->requires_alt = use_alt;
}

void v_input_initialize(Input_Virtual_Action_State * input)
{
    kinc_keyboard_set_key_down_callback(&v_input_keydown_callback, input);
    kinc_keyboard_set_key_up_callback(&v_input_keyup_callback, input);
}

void v_input_configure_move_right_action(Input_Virtual_Action_State * state, Kinc_Keycode key, Boolean use_shift, Boolean use_control, Boolean use_alt)
{
    Input_Virtual_Key * virtual_key = address_of(state->virtual_keys[(Integer_64)(E_Gameplay_Action::Move_Right)]);
    v_input_configure_virtual_key(virtual_key, key, use_shift, use_control, use_alt);
    virtual_key->event_pressed = E_Gameplay_Event::Move_Right_Pressed;
    virtual_key->event_released = E_Gameplay_Event::Move_Right_Released;
}

void v_input_configure_move_left_action(Input_Virtual_Action_State * state, Kinc_Keycode key, Boolean use_shift, Boolean use_control, Boolean use_alt)
{
    Input_Virtual_Key * virtual_key = address_of(state->virtual_keys[(Integer_64)(E_Gameplay_Action::Move_Left)]);
    v_input_configure_virtual_key(virtual_key, key, use_shift, use_control, use_alt);
    virtual_key->event_pressed = E_Gameplay_Event::Move_Left_Pressed;
    virtual_key->event_released = E_Gameplay_Event::Move_Left_Released;
}

void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/)
{
    Input_Virtual_Action_State * state = static_cast<Input_Virtual_Action_State *>(data);
    Input_Event_Queue * events_location = address_of(state->events);

    //NOTE(<zshoals> 07-27-2023): Special casing modifier keys because it's way easier this way...
    if (key == KINC_KEY_SHIFT) { state->_modifiers.shift_down = true; }
    if (key == KINC_KEY_CONTROL) { state->_modifiers.control_down = true; }
    if (key == KINC_KEY_ALT) { state->_modifiers.alt_down = true; }

    for_range_var(i, (Integer_64)(E_Gameplay_Action::MAX_COUNT))
    {
        if (v_input_virtual_key_matches_keypress(key, state->_modifiers, state->virtual_keys[i]))
        {
            vds_reset_queue_push(events_location, state->virtual_keys[i].event_pressed);
        }
    }

}

void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/)
{
    Input_Virtual_Action_State * state = static_cast<Input_Virtual_Action_State *>(data);
    Input_Event_Queue * events_location = address_of(state->events);

    //NOTE(<zshoals> 07-27-2023): Special casing modifier keys because it's way easier this way...
    if (key == KINC_KEY_SHIFT) { state->_modifiers.shift_down = false; }
    if (key == KINC_KEY_CONTROL) { state->_modifiers.control_down = false; }
    if (key == KINC_KEY_ALT) { state->_modifiers.alt_down = false; }

    for_range_var(i, (Integer_64)(E_Gameplay_Action::MAX_COUNT))
    {
        if (v_input_virtual_key_matches_keypress(key, state->_modifiers, state->virtual_keys[i]))
        {
            vds_reset_queue_push(events_location, state->virtual_keys[i].event_released);
        }
    }
}


void v_input_trigger_all_keyup_actions(Input_Virtual_Action_State * state)
{
    Input_Event_Queue * events_location = address_of(state->events);

    for_range_var(i, (Integer_64)(E_Gameplay_Action::MAX_COUNT))
    {
        vds_reset_queue_push(events_location, state->virtual_keys[i].event_released);
    }
}