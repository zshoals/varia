#pragma once

#include "varia/VGameloop.hpp"
#include "varia/graphics/VRenderer.hpp"
#include "varia/VSystemEventQueue.hpp"

struct Game_Context
{
    System_Event_Queue system_events;
    Gamestate logic_world;
    Gamestate visual_world; //Exclusively for the renderer
    Graphics_Renderer gfx;
    Graphics_Intermediate_Representation ir_storage;
};