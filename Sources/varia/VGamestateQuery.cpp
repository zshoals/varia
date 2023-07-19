#include "varia/VGamestateQuery.hpp"

Float_64 v_gamestate_logic_adjusted_delta(Gamestate const * gs)
{
    return gs->logic_dt * gs->timescale;
}

Float_64 v_gamestate_logic_raw_delta(Gamestate const * gs)
{
    return gs->logic_dt;
}

Float_64 v_gamestate_render_adjusted_delta(Gamestate const * gs)
{
    return gs->render_dt * gs->timescale;
}

Float_64 v_gamestate_render_raw_delta(Gamestate const * gs)
{
    return gs->render_dt;
}