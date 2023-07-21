#pragma once

#include "varia/VShared.hpp"
#include "kinc/window.h"

struct Gameloop_Performance
{
    Float_64 total_realtime_fixed_update_time;
};

struct Gameloop_Timing
{
    Float_64 previous_realtime;

    //Logic
    Float_64 logic_accumulator;
    Float_64 previous_logictime;

    Float_64 fixed_timestep_interval;
    Float_64 max_frametime;

    Integer_32 recent_frametime_overrun_count;
    Integer_32 max_frametime_overrun_threshold;

    //Render
    Float_64 render_accumulator;
    Float_64 previous_rendertime;
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

    Integer_64 frame_cycles; //Total update + render combos 
    Integer_64 rendered_frames;
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
    Gamestate extrapolated_gamestate; //Exclusively for the renderer
};
 
void v_gameloop_entrypoint(void * data);