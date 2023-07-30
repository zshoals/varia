#include "varia/VGameContext.hpp"

#include "varia/VShared.hpp"
#include "varia/utility/VMemcpy.hpp"

void v_gamecontext_save_logic_world(Game_Context * context, E_Game_Context_Save_Slot slot)
{
    Enum_Index idx = enum_as_index(slot);
    Gamestate * destination = address_of(context->saved_worlds[idx.i]);
    Gamestate const * source = const_address_of(context->logic_world);
    size_t write_size = sizeof(context->saved_worlds[idx.i]);

    memcpy(destination, source, write_size);
}

void v_gamecontext_load_saved_world(Game_Context * context, E_Game_Context_Save_Slot slot)
{
    Enum_Index idx = enum_as_index(slot);

    Gamestate * destination = address_of(context->logic_world);
    Gamestate const * source = const_address_of(context->saved_worlds[idx.i]);
    size_t write_size = sizeof(context->logic_world);

    memcpy(destination, source, write_size);
}