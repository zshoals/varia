#pragma once

#include "varia/input/VInput.hpp"
#include "varia/VGamedata.hpp"
#include "kinc/window.h"
#include "kinc/display.h"

enum class E_Simulating
{
    Simulate,
    Pause
};

enum class E_Simulation_Mode
{
    Fixed_Step,
    Extrapolate
};

//Gamestate configuration that can be modified at runtime
struct Gamestate
{
    //Simulation Control
    E_Simulating simulating;
    E_Simulation_Mode simulation_mode;

    //Kinc configuration options
    Boolean window_requires_reapplication;

    int kinc_primary_display_index;

    kinc_window_options_t window;
    kinc_framebuffer_options_t framebuffer;
    kinc_window_options_t pending_window_changes;
    kinc_framebuffer_options_t pending_framebuffer_changes;

    //Logic and Render timing data
    Float_64 logic_timescale;
    Float_64 fixed_timestep_interval;
    Float_64 logic_dt;
    Float_64 logic_adjusted_dt;
    Float_64 logic_accumulator;
    Float_64 previous_logictime;
    Float_64 previous_realtime;
    Float_64 render_dt;
    Float_64 render_adjusted_dt;
    Float_64 render_accumulator;
    Float_64 previous_rendertime;

    //Rendering safety limits
    Float_64 max_frametime;
    Integer_32 recent_frametime_overrun_count;
    Integer_32 max_frametime_overrun_threshold;

    //Loop configuration
    Boolean enable_sleep_in_loop; //Sleep the loop when alt-tabbed/other low performance requirement states
    Boolean enable_framerate_limit;
    Float_64 fps_limit;
    Boolean enable_excessive_frametime_exit;
    Float_64 total_realtime_fixed_update_time;

    //Performance tracking
    Integer_64 frame_cycles; //Total update + render combos 
    Integer_64 rendered_frames;
    Float_64 logic_cumulative_gameclock;
    Float_64 render_cumulative_gameclock;

    //Actual gamedata below...which we don't have any of yet :)
    Gamedata gamedata;
};
