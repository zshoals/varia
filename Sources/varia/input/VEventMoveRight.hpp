#pragma once

#include "varia/VShared.hpp"
#include "varia/input/VVirtualKey.hpp"

struct Event_Move_Right_Pressed_Data
{
    Float_64 state;
};

struct Event_Move_Right_Released_Data
{
    Float_64 state;
};

struct Event_Move_Right
{
    Virtual_Key keybind;
    Event_Move_Right_Pressed_Data pressed_data;
    Event_Move_Right_Released_Data released_data;
};