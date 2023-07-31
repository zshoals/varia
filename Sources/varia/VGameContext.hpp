#pragma once

#include "varia/VShared.hpp"
#include "varia/VGameloop.hpp"
#include "varia/graphics/VRenderer.hpp"
#include "varia/VSystemEvents.hpp"
#include "varia/VAssets.hpp"

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
    System_Events system_events;

    Gamestate logic_world;
    Gamestate visual_world; //Exclusively for the renderer
    Gamestate saved_worlds[(Integer_64)(E_Game_Context_Save_Slot::MAX_COUNT)]; //Gamestate snapshots

    Assets assets;

    Graphics_Renderer gfx;
    Graphics_Intermediate_Representation ir_storage;
};

void v_gamecontext_save_logic_world(Game_Context * context, E_Game_Context_Save_Slot slot);
void v_gamecontext_load_saved_world(Game_Context * context, E_Game_Context_Save_Slot slot);