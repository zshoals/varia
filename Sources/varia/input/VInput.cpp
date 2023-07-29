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

void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/)
{
    Input_Virtual_Action_State * state = static_cast<Input_Virtual_Action_State *>(data);
    VDS_Reset_Queue<E_Gameplay_Event> events = vds_reset_queue_make_interface(address_of(state->events));
    VDS_Reset_Queue<E_Gameplay_Event> * events_location = address_of(events);

    //NOTE(<zshoals> 07-27-2023): Special casing modifier keys because it's way easier this way...
    if (key == KINC_KEY_SHIFT) { state->modifiers.shift_down = true; }
    if (key == KINC_KEY_CONTROL) { state->modifiers.control_down = true; }
    if (key == KINC_KEY_ALT) { state->modifiers.alt_down = true; }

    for_range_var(i, (Integer_64)(E_Gameplay_Actions::MAX_COUNT))
    {
        if (v_input_virtual_key_matches_keypress(key, state->modifiers, state->virtual_keys[i]))
        {
            vds_reset_queue_push(events_location, state->virtual_keys[i].event_pressed);
        }
    }

}

void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/)
{
    Input_Virtual_Action_State * state = static_cast<Input_Virtual_Action_State *>(data);
    VDS_Reset_Queue<E_Gameplay_Event> events = vds_reset_queue_make_interface(address_of(state->events));
    VDS_Reset_Queue<E_Gameplay_Event> * events_location = address_of(events);

    //NOTE(<zshoals> 07-27-2023): Special casing modifier keys because it's way easier this way...
    if (key == KINC_KEY_SHIFT) { state->modifiers.shift_down = false; }
    if (key == KINC_KEY_CONTROL) { state->modifiers.control_down = false; }
    if (key == KINC_KEY_ALT) { state->modifiers.alt_down = false; }

    for_range_var(i, (Integer_64)(E_Gameplay_Actions::MAX_COUNT))
    {
        if (v_input_virtual_key_matches_keypress(key, state->modifiers, state->virtual_keys[i]))
        {
            vds_reset_queue_push(events_location, state->virtual_keys[i].event_released);
        }
    }
}


void v_input_trigger_all_keyup_actions(Input_Virtual_Action_State * state)
{
    VDS_Reset_Queue<E_Gameplay_Event> events = vds_reset_queue_make_interface(address_of(state->events));
    VDS_Reset_Queue<E_Gameplay_Event> * events_location = address_of(events);

    for_range_var(i, (Integer_64)(E_Gameplay_Actions::MAX_COUNT))
    {
        vds_reset_queue_push(events_location, state->virtual_keys[i].event_released);
    }
}