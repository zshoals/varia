#include "VGameloop.hpp"

#include "VShared.hpp"
#include "VGamestateQuery.hpp"
#include "kinc/system.h"
#include "kinc/log.h"
#include "kinc/graphics4/graphics.h"
#include <stdio.h>


//Game System Imports

static Float_64 timeout = 0.0;

static void v_print_timing_info(Game_Context * gctx)
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
        gctx->gamestate.frame_cycles,
        gctx->gamestate.rendered_frames,
        gctx->gamestate.logic_dt,
        1.0 / gctx->gamestate.logic_dt,
        gctx->gamestate.render_dt,
        gctx->timing.previous_rendertime,
        gctx->gamestate.logic_gameclock,
        gctx->gamestate.render_gameclock,
        gctx->time_perf.total_realtime_fixed_update_time,
        kinc_time()
    );
}

static void v_gameloop_tick(Gamestate * gs)
{
    gs->gamedata.player.x += (float)(100.0 * v_gamestate_logic_adjusted_delta(gs));
}

static void v_gameloop_render(Game_Context * gctx)
{
    //[BEGIN] Rendering
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Gamestate * gs = address_of(gctx->gamestate);

    kinc_g4_begin(0);
    kinc_g4_end(0);

    kinc_g4_swap_buffers();
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    v_print_timing_info(gctx);
}

void v_gameloop_entrypoint(void * data)
{
    Game_Context * context = static_cast<Game_Context *>(data);
    Gameloop_Timing * timing = address_of(context->timing);

    //Process Input?
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //Nothing yet
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Process important commands (Kinc state configuration)??
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //Nothing yet
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    Float_64 realtime = kinc_time();
    Float_64 realtime_advance = kinc_time() - timing->previous_realtime;
    timing->previous_realtime = realtime;

    //Logic Step
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        timing->logic_accumulator += realtime_advance;

        //[BEGIN] Max frametime checks / program exit if we get into a nasty death spiral situation
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        if 
        (
            context->gamestate.loop_config.enable_excessive_frametime_exit &&
            timing->recent_frametime_overrun_count >= timing->max_frametime_overrun_threshold
        )
        {
            VARIA_UNREACHABLE("Repeatedly exceeding the maximum permissible frametime;"
                "likely in a logic death spiral. Aborting program.");

            kinc_log(KINC_LOG_LEVEL_ERROR, "Repeatedly exceeding the maximum permissible frametime;"
                "likely in a logic death spiral. Aborting program.");
            kinc_stop();
        }
        else if (timing->logic_accumulator > timing->max_frametime)
        {
            timing->recent_frametime_overrun_count += 1;
            timing->logic_accumulator = timing->max_frametime;
        }
        else
        {
            timing->recent_frametime_overrun_count = 0;
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        


        //[BEGIN] Primary fixed update loop
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        context->time_perf.total_realtime_fixed_update_time = kinc_time();
        while (timing->logic_accumulator >= timing->fixed_timestep_interval)
        {
            timing->logic_accumulator -= timing->fixed_timestep_interval;

            Gamestate * gs = address_of(context->gamestate);
            gs->logic_gameclock += context->timing.fixed_timestep_interval;

            v_gameloop_tick(gs);

            timing->previous_logictime = kinc_time();
        }
        context->time_perf.total_realtime_fixed_update_time = 
            kinc_time() - context->time_perf.total_realtime_fixed_update_time;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    }
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::



    //Render Step
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        timing->render_accumulator += realtime_advance;

        //NOTE(<zshoals> 07-19-2023): Extrapolated time, how far ahead the render time is
        //  from the last recent world step
        context->gamestate.render_dt = kinc_time() - timing->previous_logictime;

        //Frame limiting check
        if //vertical sync off, framelimiter on, haven't reached the accumulator threshold yet
        ( 
            !(context->gamestate.kinc.framebuffer.vertical_sync) &&
            context->gamestate.loop_config.enable_framerate_limit &&
            context->timing.render_accumulator <= context->gamestate.loop_config.render_frames_per_second_limit
        )
        {
            VARIA_NO_OPERATION();
        }
        else
        {
            v_gameloop_render(context);

            context->gamestate.render_gameclock += kinc_time() - timing->previous_rendertime;
            timing->previous_rendertime = kinc_time();

            context->timing.render_accumulator = 0.0;

            context->gamestate.rendered_frames += 1;
        }
    }
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    context->gamestate.frame_cycles += 1;
}
