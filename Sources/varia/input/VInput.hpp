#pragma once

#include "varia/VShared.hpp"
#include "varia/ds/VDS-ResetQueue.hpp"

#include "kinc/input/keyboard.h"

typedef int Kinc_Keycode;

enum class E_Gameplay_Action
{
    Move_Right,
    Move_Left,

    MAX_COUNT //The way we use this means it acts like 
};

enum class E_Gameplay_Event
{
    UNHANDLED_EVENT,

    No_Action,

    Move_Right_Pressed,
    Move_Right_Released,

    Move_Left_Pressed,
    Move_Left_Released,

};

struct Input_Virtual_Key
{
    Kinc_Keycode bound_key;
    Boolean requires_shift;
    Boolean requires_control;
    Boolean requires_alt;
    E_Gameplay_Event event_pressed;
    E_Gameplay_Event event_released;
};

struct Input_Modifier_State
{
    Boolean control_down;
    Boolean shift_down;
    Boolean alt_down;
};

using Input_Event_Queue = VDS_Reset_Queue<E_Gameplay_Event, 128>;
struct Input_Virtual_Action_State
{
    Input_Virtual_Key virtual_keys[E_Gameplay_Action::MAX_COUNT];

    Input_Event_Queue events;

    Input_Modifier_State _modifiers;
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void v_input_initialize(Input_Virtual_Action_State * input);

void v_input_configure_move_right_action(Input_Virtual_Action_State * state, Kinc_Keycode key, Boolean use_shift, Boolean use_control, Boolean use_alt);
void v_input_configure_move_left_action(Input_Virtual_Action_State * state, Kinc_Keycode key, Boolean use_shift, Boolean use_control, Boolean use_alt);

void v_input_trigger_all_keyup_actions(Input_Virtual_Action_State * state);
void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/);
void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/);

template <typename FUNC>
void v_input_process_events(Input_Virtual_Action_State * state, FUNC f)
{
    Input_Event_Queue * events_location = address_of(state->events);

    while (vds_reset_queue_has_elements(events_location))
    {
        E_Gameplay_Event e = vds_reset_queue_get_first_element(events_location);
        f(e);
        vds_reset_queue_pop(events_location);
    }

    vds_reset_queue_reset(events_location);
}