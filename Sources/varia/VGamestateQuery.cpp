#include "varia/VGamestateQuery.hpp"

Float_64 v_gamestate_adjusted_delta(Gamestate const * gs)
{
    return gs->dt * gs->timescale;
}

Float_64 v_gamestate_raw_delta(Gamestate const * gs)
{
    return gs->dt;
}