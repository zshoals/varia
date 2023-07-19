#pragma once

#include "varia/VCommon.hpp"
#include "varia/VGameloop.hpp"

//TODO(<zshoals> 07-18-2023): This is at the wrong abstraction level
//  we should have some sort of substruct instead of the ENTIRE gamestate xd
Float_64 v_gamestate_logic_adjusted_delta(Gamestate const * gs);
Float_64 v_gamestate_logic_raw_delta(Gamestate const * gs);
Float_64 v_gamestate_render_adjusted_delta(Gamestate const * gs);
Float_64 v_gamestate_render_raw_delta(Gamestate const * gs);