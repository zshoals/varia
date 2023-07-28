#include "varia/input/VInput.hpp"

#include "varia/VShared.hpp"

#include "kinc/input/keyboard.h"
#include "kinc/input/mouse.h"

void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_Event_Emitter * state*/)
{
    Input_Event_Emitter * emitter = static_cast<Input_Event_Emitter *>(data);
    Input_Virtual_Action_State * state = emitter->input;
    System_Event_Queue * events = emitter->events;

    //NOTE(<zshoals> 07-27-2023): Special casing modifier keys because it's way easier this way...
    if (key == KINC_KEY_SHIFT) { state->modifiers.shift_down = true; }
    if (key == KINC_KEY_CONTROL) { state->modifiers.control_down = true; }
    if (key == KINC_KEY_ALT) { state->modifiers.alt_down = true; }

    //Gameplay::Move Right PRESSED
    if(v_input_virtual_key_matches_keypress(key, state->modifiers, state->move_right_action))
    {
        System_Event e = ZERO_INIT();
        e.tag = E_System_Event_Type::Gameplay_Move_Right_Pressed;

        v_system_event_queue_push(events, e);
    }
    //if....other keybinds...

}

void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_Event_Emitter * state*/)
{
    Input_Event_Emitter * emitter = static_cast<Input_Event_Emitter *>(data);
    Input_Virtual_Action_State * state = emitter->input;
    System_Event_Queue * events = emitter->events;

    //NOTE(<zshoals> 07-27-2023): Special casing modifier keys because it's way easier this way...
    if (key == KINC_KEY_SHIFT) { state->modifiers.shift_down = false; }
    if (key == KINC_KEY_CONTROL) { state->modifiers.control_down = false; }
    if (key == KINC_KEY_ALT) { state->modifiers.alt_down = false; }

    //Gameplay::Move Right RELEASED
    if(v_input_virtual_key_matches_keypress(key, state->modifiers, state->move_right_action))
    {
        System_Event e = ZERO_INIT();
        e.tag = E_System_Event_Type::Gameplay_Move_Right_Released;

        v_system_event_queue_push(events, e);
    }
    //if....other keybinds...
}


void v_input_trigger_all_keyup_actions(Input_Event_Emitter * emitter)
{
    Input_Virtual_Action_State const * state = emitter->input;

    System_Event e[32] = ZERO_INIT();

    //NOTE(<zshoals> 07-28-2023): Yes...we're just going to manually build this list...
    e[0].tag = E_System_Event_Type::Gameplay_Move_Right_Released;

    Integer_64 constexpr max_items = 1;
    for_range_var(i, max_items)
    {
        v_system_event_queue_push(emitter->events, e[i]);
    }
}