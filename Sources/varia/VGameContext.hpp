#pragma once

#include "varia/VGameloop.hpp"
#include "varia/graphics/VRenderer.hpp"
#include "varia/VSystemEvents.hpp"
#include "varia/ds/VDS-ResetQueue.hpp"

struct Game_Context
{
    Input_Virtual_Action_State input;

    #define SYSTEM_EVENT_QUEUE_MAX_SIZE 128
    VDS_Reset_Queue_Storage<E_System_Event_Type, SYSTEM_EVENT_QUEUE_MAX_SIZE> system_events;

    Gamestate logic_world;
    Gamestate visual_world; //Exclusively for the renderer

    Graphics_Renderer gfx;
    Graphics_Intermediate_Representation ir_storage;
};