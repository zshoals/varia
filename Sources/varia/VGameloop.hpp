#pragma once

#include "varia/VCommon.hpp"

struct Gameloop_Timing
{
    Float_64 accumulator;

    Float_64 previous_frametime;

    Float_64 fixed_timestep_interval;
    Float_64 max_frametime;

    Integer_32 recent_frametime_overrun_count;
    Integer_32 max_frametime_overrun_threshold;
};

struct Player
{
    Float_32 x;
    Float_32 y;
};

struct Gamestate
{
    Float_64 dt;
    Float_64 timescale;
    Player player;
};

struct Game_Context
{
    Gameloop_Timing timing;
    Gamestate gamestate;
};
 
void v_gameloop_entrypoint(void * data);
Float_64 v_gamestate_adjusted_time(Gamestate const * gs);