#pragma once

#include "varia/VShared.hpp"
#include "varia/VGameloop.hpp"
#include "varia/graphics/VRenderer.hpp"
#include "varia/VSystemEvents.hpp"
#include "varia/ds/VDS-ResetQueue.hpp"
#include "varia/ds/VDS-Buffer.hpp"

enum class E_Game_Context_Save_Slot
{
    Slot_1,
    Slot_2,
    Slot_3,

    MAX_COUNT
};

struct Game_Context
{
    Input_Virtual_Action_State input;

    #define SYSTEM_EVENT_QUEUE_MAX_SIZE 128
    VDS_Reset_Queue_Storage<E_System_Event_Type, SYSTEM_EVENT_QUEUE_MAX_SIZE> system_events;

    Gamestate logic_world;
    Gamestate visual_world; //Exclusively for the renderer
    Gamestate saved_worlds[(Integer_64)(E_Game_Context_Save_Slot::MAX_COUNT)]; //Gamestate snapshots

    Graphics_Renderer gfx;
    Graphics_Intermediate_Representation ir_storage;

    VDS_Buffer<68 * 1000 * 1000> image_loading_buffer; //68mb
    VDS_Buffer<20 * 1000 * 1000> file_loading_buffer; //20mb
};

void v_gamecontext_save_logic_world(Game_Context * context, E_Game_Context_Save_Slot slot);
void v_gamecontext_load_saved_world(Game_Context * context, E_Game_Context_Save_Slot slot);