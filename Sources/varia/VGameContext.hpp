#pragma once

#include "varia/VGameloop.hpp"
#include "varia/graphics/VRenderer.hpp"

struct Game_Context
{
    Gamestate logic_world;
    Gamestate visual_world; //Exclusively for the renderer
    Graphics_Renderer gfx;
    Graphics_Intermediate_Representation ir_storage;
};