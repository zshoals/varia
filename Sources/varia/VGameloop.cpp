#include "VGameloop.hpp"

#include "VCommon.hpp"
#include "VGamestateQuery.hpp"
#include "kinc/system.h"
#include "kinc/log.h"
#include "kinc/graphics4/graphics.h"


//Game System Imports

static Float_64 timeout = 0.0;

static void v_print_timing_info(Game_Context * gctx)
{
    // kinc_log(KINC_LOG_LEVEL_INFO, "Player X:    %f", gs->gamedata.player.x);

    kinc_log(KINC_LOG_LEVEL_INFO, 
        ":::::::::::::::::::\n\
Cycles:      %d\n\
RenderFrms:  %d\n\
FixDelta:    %f\n\
RenderDelta  %f\n\
LastRender:  %f\n\
Gametime:    %f\n\
VarGametime: %f\n\
Update Time: %f\n\
:::::::::::::::::::\n",
        gctx->gamestate.frame_cycles,
        gctx->gamestate.rendered_frames,
        gctx->gamestate.logic_dt,
        gctx->gamestate.render_dt,
        gctx->timing.previous_rendertime,
        gctx->gamestate.logic_gameclock,
        gctx->gamestate.render_gameclock,
        gctx->time_perf.total_realtime_fixed_update_time
    );
}

static void v_gameloop_tick(Gamestate * gs)
{
    gs->gamedata.player.x += (float)(100.0 * v_gamestate_logic_adjusted_delta(gs));
}

static Boolean v_gameloop_render(Game_Context * gctx)
{
    //Frame limiting check
    if //vertical sync off, framelimiter on, haven't reached the accumulator threshold yet
    ( 
        !(gctx->gamestate.kinc.framebuffer.vertical_sync) &&
        gctx->gamestate.loop_config.enable_framerate_limit &&
        gctx->timing.render_accumulator <= gctx->gamestate.loop_config.render_frames_per_second_limit
    )
    {
        //Don't render a frame as we haven't exceeded the framerate limit yet
        return false;
    }


    //[BEGIN] Rendering
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Gamestate * gs = address_of(gctx->gamestate);

    kinc_g4_begin(0);
    kinc_g4_end(0);

    kinc_g4_swap_buffers();
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    v_print_timing_info(gctx);
    return true;
}

static Boolean v_gameloop_fixed_update(Game_Context * gctx)
{
    Gameloop_Timing * timing = address_of(gctx->timing);

    //[BEGIN] Max frametime checks / program exit if we get into a nasty death spiral situation
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    if 
    (
        gctx->gamestate.loop_config.enable_excessive_frametime_exit &&
        timing->recent_frametime_overrun_count >= timing->max_frametime_overrun_threshold
    )
    {
        VARIA_UNREACHABLE("Repeatedly exceeding the maximum permissible frametime; likely in a logic death spiral. Aborting program.");

        kinc_log(KINC_LOG_LEVEL_ERROR, "Repeatedly exceeding the maximum permissible frametime; likely in a logic death spiral. Aborting program.");
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

    Boolean actually_ticked_this_update = false;

    gctx->time_perf.total_realtime_fixed_update_time = kinc_time();
    while (timing->logic_accumulator >= timing->fixed_timestep_interval)
    {
        actually_ticked_this_update = true;
        timing->logic_accumulator -= timing->fixed_timestep_interval;

        Gamestate * gs = address_of(gctx->gamestate);
        gs->logic_gameclock += gctx->timing.fixed_timestep_interval;

        v_gameloop_tick(gs);
    }
    gctx->time_perf.total_realtime_fixed_update_time = kinc_time() - gctx->time_perf.total_realtime_fixed_update_time;

    return actually_ticked_this_update;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}

void v_gameloop_entrypoint(void * data)
{
    Game_Context * context = static_cast<Game_Context *>(data);
    Gameloop_Timing * timing = address_of(context->timing);

    Float_64 realtime = kinc_time();
    Float_64 realtime_advance = kinc_time() - timing->previous_realtime;

    //Logic Step
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        timing->logic_accumulator += realtime_advance;

        Boolean logic_was_updated = v_gameloop_fixed_update(context);
        if (logic_was_updated)
        {
            timing->previous_logictime = kinc_time();
        }
    }
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Render Step
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        timing->render_accumulator += realtime_advance;

        //NOTE(<zshoals> 07-19-2023): Extrapolated time, how far ahead the render time is from the last recent world step
        context->gamestate.render_dt = kinc_time() - timing->previous_logictime;

        Boolean frame_was_rendered = v_gameloop_render(context);
        if (frame_was_rendered)
        {
            context->gamestate.render_gameclock += kinc_time() - timing->previous_rendertime;
            kinc_log(KINC_LOG_LEVEL_INFO, "Accum: %f", context->timing.render_accumulator);
            timing->previous_rendertime = kinc_time();

            context->timing.render_accumulator = 0.0;

            context->gamestate.rendered_frames += 1;
        }
    }
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    timing->previous_realtime = realtime;
    context->gamestate.frame_cycles += 1;
}
