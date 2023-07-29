#pragma once

#include "varia/VShared.hpp"
#include "varia/ds/VDS-ResetQueue.hpp"

#include "kinc/input/keyboard.h"

typedef int Kinc_Keycode;

enum class E_Gameplay_Actions
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

#define GAMEPLAY_EVENT_QUEUE_MAX_SIZE 128
struct Input_Virtual_Action_State
{
    Input_Virtual_Key virtual_keys[E_Gameplay_Actions::MAX_COUNT];

    VDS_Reset_Queue_Storage<E_Gameplay_Event, GAMEPLAY_EVENT_QUEUE_MAX_SIZE> events;

    Input_Modifier_State modifiers;
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void v_input_trigger_all_keyup_actions(Input_Virtual_Action_State * state);
void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/);
void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_Virtual_Action_State * state*/);