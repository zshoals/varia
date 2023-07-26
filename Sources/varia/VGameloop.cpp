#include "VGameloop.hpp"

#include "VShared.hpp"
#include "VGamestateQuery.hpp"
#include "kinc/system.h"
#include "kinc/log.h"
#include "kinc/graphics4/graphics.h"
#include <string.h> //Memcpy
#include <stdio.h>


//Game System Imports

static void v_print_timing_info(Gamestate * gamestate)
{
    // kinc_log(KINC_LOG_LEVEL_INFO, "Player X:    %f", gs->gamedata.player.x);

    kinc_log(KINC_LOG_LEVEL_INFO, 
        ":::::::::::::::::::\n"
        "Cycles:      %d\n"
        "RenderFrms:  %d\n"
        "FixDelta:    %f\n"
        "FixRecip:    %f\n"
        "RenderDelta  %f\n"
        "LastRender:  %f\n"
        "Gametime:    %f\n"
        "VarGametime: %f\n"
        "Update Time: %f\n"
        "Real Time:   %f\n"
        ":::::::::::::::::::\n",
        gamestate->frame_cycles,
        gamestate->rendered_frames,
        gamestate->logic_dt,
        1.0 / gamestate->logic_dt,
        gamestate->render_dt,
        gamestate->previous_rendertime,
        gamestate->logic_gameclock,
        gamestate->render_gameclock,
        gamestate->total_realtime_fixed_update_time,
        kinc_time()
    );
}

static void v_gameloop_simulate(Gamestate * gs, E_Simulation_Mode mode)
{
    gs->gamedata.player.x += (float)(100.0 * v_gamestate_logic_adjusted_delta(gs));
}

static void v_gameloop_render(Gamestate * gamestate)
{
    //[BEGIN] Rendering
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    kinc_g4_begin(0);
    kinc_g4_end(0);

    kinc_g4_swap_buffers();
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    v_print_timing_info(gamestate);
}

void v_gameloop_entrypoint(void * data)
{
    Game_Context * context = static_cast<Game_Context *>(data);
    Gamestate * logic_world = address_of(context->logic_world);

    //Process Input?
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //        Nothing yet
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Process important commands (Kinc state configuration)??
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //        Nothing yet
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    Float_64 realtime = kinc_time();
    Float_64 realtime_advance = kinc_time() - logic_world->previous_realtime;
    logic_world->previous_realtime = realtime;

    //Logic Step
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        logic_world->logic_accumulator += realtime_advance;

        //[BEGIN] Max frametime checks / program exit if we get into a nasty death spiral situation
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        if 
        (
            logic_world->enable_excessive_frametime_exit &&
            logic_world->recent_frametime_overrun_count >= logic_world->max_frametime_overrun_threshold
        )
        {
            VARIA_UNREACHABLE("Repeatedly exceeding the maximum permissible frametime;"
                "likely in a logic death spiral. Aborting program.");

            kinc_log(KINC_LOG_LEVEL_ERROR, "Repeatedly exceeding the maximum permissible frametime;"
                "likely in a logic death spiral. Aborting program.");
            kinc_stop();
        }
        else if (logic_world->logic_accumulator > logic_world->max_frametime)
        {
            logic_world->recent_frametime_overrun_count += 1;
            logic_world->logic_accumulator = logic_world->max_frametime;
        }
        else
        {
            logic_world->recent_frametime_overrun_count = 0;
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        


        //[BEGIN] Primary fixed update loop
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        logic_world->total_realtime_fixed_update_time = kinc_time();
        while (logic_world->logic_accumulator >= logic_world->fixed_timestep_interval)
        {
            logic_world->logic_accumulator -= logic_world->fixed_timestep_interval;
            logic_world->logic_gameclock += logic_world->fixed_timestep_interval;
            logic_world->previous_logictime = kinc_time();

            v_gameloop_simulate(logic_world, E_Simulation_Mode::Fixed_Step);
        }
        logic_world->total_realtime_fixed_update_time = 
            kinc_time() - logic_world->total_realtime_fixed_update_time;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    }
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::



    //Render Step
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        logic_world->render_accumulator += realtime_advance;

        //NOTE(<zshoals> 07-19-2023): Extrapolated time, how far ahead the render time is
        //  from the last recent world step
        logic_world->render_dt = kinc_time() - logic_world->previous_logictime;

        //Frame limiting check
        if //vertical sync off, framelimiter on, haven't reached the accumulator threshold yet
        ( 
            !(logic_world->framebuffer.vertical_sync) &&
            logic_world->enable_framerate_limit &&
            logic_world->render_accumulator <= logic_world->fps_limit
        )
        {
            VARIA_NO_OPERATION();
        }
        else
        {
            Gamestate * visual_world = address_of(context->visual_world);
            //Create an "on demand" gamestate which will not be part of the fixed-step
            //  logic chain, this gamestate is for visual purposes only
            memcpy(visual_world, logic_world, sizeof(*logic_world));

            v_gameloop_simulate(visual_world, E_Simulation_Mode::Extrapolate);
            v_gameloop_render(visual_world);

            logic_world->render_gameclock += kinc_time() - logic_world->previous_rendertime;
            logic_world->previous_rendertime = kinc_time();

            logic_world->render_accumulator = 0.0;

            logic_world->rendered_frames += 1;
        }
    }
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    logic_world->frame_cycles += 1;
}
