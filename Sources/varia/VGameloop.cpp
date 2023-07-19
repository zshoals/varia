#include "VGameloop.hpp"

#include "VCommon.hpp"
#include "VGamestateQuery.hpp"
#include "kinc/system.h"
#include "kinc/log.h"
#include "kinc/graphics4/graphics.h"


//Game System Imports


static void v_gameloop_tick(Gamestate * gs)
{
    gs->gamedata.player.x += (float)(100.0 * v_gamestate_logic_adjusted_delta(gs));
}

static void v_gameloop_render(Game_Context * gctx)
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
        return;
    }

    //We are finally rendering a frame, reset the render accumulator
    gctx->timing.render_accumulator = 0.0;

    //[BEGIN] Rendering
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    Gamestate * gs = address_of(gctx->gamestate);

    // kinc_log(KINC_LOG_LEVEL_INFO, "Player X:    %f", gs->gamedata.player.x);
    // kinc_log(KINC_LOG_LEVEL_INFO, "Update time: %f", gctx->time_perf.total_realtime_fixed_update_time);
    kinc_log(KINC_LOG_LEVEL_INFO, "Frametime:   %f", gctx->time_perf.previous_frametime_differential);
    kinc_log(KINC_LOG_LEVEL_INFO, "Gametime:    %f", gctx->gamestate.logic_gameclock);
    kinc_log(KINC_LOG_LEVEL_INFO, "VarGametime: %f", gctx->gamestate.render_gameclock);

    kinc_g4_begin(0);
    kinc_g4_end(0);

    kinc_g4_swap_buffers();
}

static void v_gameloop_fixed_update(Game_Context * gctx)
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

    gctx->time_perf.total_realtime_fixed_update_time = kinc_time();
    while (timing->logic_accumulator >= timing->fixed_timestep_interval)
    {
        timing->logic_accumulator -= timing->fixed_timestep_interval;

        Gamestate * gs = address_of(gctx->gamestate);
        gs->logic_gameclock += gctx->timing.fixed_timestep_interval;

        v_gameloop_tick(gs);
    }
    gctx->time_perf.total_realtime_fixed_update_time = kinc_time() - gctx->time_perf.total_realtime_fixed_update_time;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}

void v_gameloop_entrypoint(void * data)
{
    Game_Context * context = static_cast<Game_Context *>(data);
    Gameloop_Timing * timing = address_of(context->timing);

    Float_64 ktime = kinc_time();
    Float_64 time_since_last_update = ktime - timing->previous_frametime;
    context->time_perf.previous_frametime_differential = time_since_last_update;

    //Logic Step
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        timing->logic_accumulator += time_since_last_update;

        v_gameloop_fixed_update(context);
    }
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Render Step
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        //NOTE(<zshoals> 07-19-2023): Acquire a more accurate last update time, as some realtime has passed while updating the gamestate
        //  however, don't actually use it for performance tracking...probably unnecessary
        time_since_last_update = kinc_time() - timing->previous_frametime;
        timing->render_accumulator += time_since_last_update;

        context->gamestate.render_dt = time_since_last_update;
        context->gamestate.render_gameclock += time_since_last_update;

        v_gameloop_render(context);
    }
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Finally record the previous frametime which was taken BEFORE updating and rendering
    timing->previous_frametime = ktime;
}
