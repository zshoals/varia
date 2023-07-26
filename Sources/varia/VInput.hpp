#pragma once

#include "varia/VShared.hpp"


typedef int Kinc_Keycode;
typedef void (*Input_Callback)(Input_Gamestate_Data_Proxy * proxy);

struct Input_Gamestate_Data_Proxy
{
    //Insert any gamestate stuff into this struct, 
    //  copy it into here
    //  copy back out

    Float_32 movement_right_multiplier;
    Float_32 movement_left_multiplier;
    Float_32 movement_up_multiplier;
    Float_32 movement_down_multiplier;

    Boolean player_is_shooting;
    Boolean player_jumped;
}

enum class E_Input_Virtual_Action
{
    Move_Right,
    Move_Left,
    Move_Up,
    Move_Down,
    Jump,
    Shoot,

    MAX_COUNT
};

struct Input_Virtual_Action
{
    Input_Callback on_keydown;
    Input_Callback on_keyup;
    Kinc_Keycode bound_key;
};

struct Input_State
{
    Input_Virtual_Action actions[(Integer_64)(E_Input_Virtual_Action::MAX_COUNT)];
    Input_Gamestate_Data_Proxy proxy;
};

Input_Virtual_Action v_input_create_virtual_action(Kinc_Keycode key, Input_Callback on_keydown, Input_Callback on_keyup);

void v_input_set_move_right(Input_State * input, Input_Virtual_Action action);
void v_input_set_move_left(Input_State * input, Input_Virtual_Action action);
void v_input_set_move_up(Input_State * input, Input_Virtual_Action action);
void v_input_set_move_down(Input_State * input, Input_Virtual_Action action);
void v_input_set_jump(Input_State * input, Input_Virtual_Action action);
void v_input_set_shoot(Input_State * input, Input_Virtual_Action action);

void v_input_register_processing_functions(Input_State * input);

void v_input_keydown_callback(Kinc_Keycode key, void * data /*Input_State * state*/);
void v_input_keyup_callback(Kinc_Keycode key, void * data /*Input_State * state*/);