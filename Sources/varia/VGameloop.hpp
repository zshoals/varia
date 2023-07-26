#pragma once

#include "varia/VShared.hpp"
#include "kinc/window.h"

enum class E_Simulation_Mode
{
    Fixed_Step,
    Extrapolate
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

//Gamestate configuration that can be modified at runtime
struct Gamestate
{
    //Kinc configuration options
    kinc_window_options_t window;
    kinc_framebuffer_options_t framebuffer;

    //Logic and Render timing data
    Float_64 logic_timescale;
    Float_64 fixed_timestep_interval;
    Float_64 logic_dt;
    Float_64 logic_accumulator;
    Float_64 previous_logictime;
    Float_64 previous_realtime;
    Float_64 render_dt;
    Float_64 render_accumulator;
    Float_64 previous_rendertime;

    //Rendering safety limits
    Float_64 max_frametime;
    Integer_32 recent_frametime_overrun_count;
    Integer_32 max_frametime_overrun_threshold;

    //Loop configuration
    Boolean enable_framerate_limit;
    Float_64 fps_limit;
    Boolean enable_excessive_frametime_exit;
    Float_64 total_realtime_fixed_update_time;

    //Performance tracking
    Integer_64 frame_cycles; //Total update + render combos 
    Integer_64 rendered_frames;
    Float_64 logic_gameclock;
    Float_64 render_gameclock;

    //Actual gamedata
    Gamedata gamedata;
};

struct Game_Context
{
    Gamestate logic_world;
    Gamestate visual_world; //Exclusively for the renderer
};
 
void v_gameloop_entrypoint(void * data);