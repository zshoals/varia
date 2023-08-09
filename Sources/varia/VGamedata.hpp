#pragma once

#include "varia/ds/VDS-Array.hpp"
#include "varia/ds/VDS-String.hpp"

//NOTE(<zshoals> 08-09-2023): This struct houses the ACTUAL gameplay stuff, things like
//  player and enemy behaviors, not info that's more meta than that. Meta info is in "Gamestate"
struct Gamedata
{
    VDS_Array<VDS_Short_String, 32> enemy_texture;
    VDS_Array<float, 32> x_pos;
    VDS_Array<float, 32> y_pos;
    VDS_Array<float, 32> width;
    VDS_Array<float, 32> height;
    VDS_Array<float, 32> layer;
};