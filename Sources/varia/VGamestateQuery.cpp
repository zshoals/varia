#include "varia/VGamestateQuery.hpp"

Float_64 v_gamestate_logic_adjusted_delta(Gamestate const * gs)
{
    return gs->logic_dt * gs->logic_timescale;
}

Float_64 v_gamestate_logic_raw_delta(Gamestate const * gs)
{
    return gs->logic_dt;
}

Float_64 v_gamestate_render_adjusted_delta(Gamestate const * gs)
{
    //TODO(<zshoals> 07-26-2023): Maybe not use the logic timescale here?
    return gs->render_dt * gs->logic_timescale;
}

Float_64 v_gamestate_render_raw_delta(Gamestate const * gs)
{
    return gs->render_dt;
}