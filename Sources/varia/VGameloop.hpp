#pragma once

#include "varia/VCommon.hpp"
#include "kinc/window.h"

struct Gameloop_Performance
{
    Float_64 total_realtime_fixed_update_time;
    Float_64 previous_frametime_differential;
};

struct Gameloop_Timing
{
    Float_64 previous_frametime;

    //Logic
    Float_64 logic_accumulator;

    Float_64 fixed_timestep_interval;
    Float_64 max_frametime;

    Integer_32 recent_frametime_overrun_count;
    Integer_32 max_frametime_overrun_threshold;

    //Render
    Float_64 render_accumulator;
};

struct Gameloop_Config
{
    Float_64 render_frames_per_second_limit;
    Boolean enable_framerate_limit;

    Boolean enable_excessive_frametime_exit;
};

struct Kinc_Settings
{
    kinc_window_options_t window;
    kinc_framebuffer_options_t framebuffer;
};

struct Player
{
    Float_32 x;
    Float_32 y;
};

struct Gamedata
{
    Player player;
};

struct Gamestate
{
    //Gamestate configuration that can be modified at runtime
    Gameloop_Config loop_config;
    Kinc_Settings kinc;

    Float_64 logic_dt;
    Float_64 render_dt;
    Float_64 logic_gameclock;
    Float_64 render_gameclock;
    Float_64 timescale;

    //Actual gamedata
    Gamedata gamedata;
};

struct Game_Context
{
    Gameloop_Timing timing;
    Gameloop_Performance time_perf;

    Gamestate gamestate;
};
 
void v_gameloop_entrypoint(void * data);