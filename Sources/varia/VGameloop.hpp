#pragma once

#include "varia/VShared.hpp"
#include "varia/input/VInput.hpp"
#include "kinc/window.h"

enum class E_Simulation_Mode
{
    Fixed_Step,
    Extrapolate
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
    Float_64 logic_cumulative_gameclock;
    Float_64 render_cumulative_gameclock;

    //Input
    //TODO(<zshoals> 07-27-2023): This might actually belong in the game context
    Input_Virtual_Action_State input;

    //Other stuff
    Float_64 display_time_multiplier;

    //Actual gamedata below...which we don't have any of yet :)
};

 
void v_gameloop_initialize(kinc_window_options_t wo, kinc_framebuffer_options_t fbo);