#include "VCommon.hpp"
#include "VGameloop.hpp"
#include "kinc/system.h"
#include "kinc/log.h"
#include "kinc/graphics4/graphics.h"


static void v_gameloop_tick(Gamestate * gs)
{
    gs->player.x += (float)(100.0 * v_gamestate_adjusted_time(gs));
}

static void v_gameloop_render(Game_Context * gctx)
{
    Gamestate * gs = address_of(gctx->gamestate);
    //NOTE(<zshoals> 07-18-2023): I LITERALLY PUT THIS IN
    //  TO ABORT THE PROGRAM AFTER 20 SECONDS!!!!!
    if (kinc_time() >= 20.0)
    {
        kinc_stop();
    }

    kinc_log(KINC_LOG_LEVEL_INFO, "Player X:    %f", gs->player.x);
    kinc_log(KINC_LOG_LEVEL_INFO, "Update time: %f", gctx->time_perf.total_realtime_fixed_update_time);
    kinc_log(KINC_LOG_LEVEL_INFO, "Frametime:   %f", gctx->time_perf.previous_frametime_differential);

    kinc_g4_begin(0);
    kinc_g4_end(0);

    kinc_g4_swap_buffers();
}

static void v_gameloop_fixed_update(Game_Context * gctx)
{
    Gameloop_Timing * timing = address_of(gctx->timing);
    double ktime = kinc_time();
    double time_since_last_update = ktime - timing->previous_frametime;
    gctx->time_perf.previous_frametime_differential = time_since_last_update;
    timing->previous_frametime = ktime;


    timing->accumulator += time_since_last_update;


    //[BEGIN] Max frametime checks / program exit if we get into a nasty death spiral situation
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    if (timing->accumulator > timing->max_frametime)
    {
        timing->recent_frametime_overrun_count += 1;
        timing->accumulator = timing->max_frametime;
    }
    else
    {
        timing->recent_frametime_overrun_count = 0;
    }

    if (timing->recent_frametime_overrun_count >= timing->max_frametime_overrun_threshold)
    {
        VARIA_UNREACHABLE("Repeatedly exceeding the maximum permissible frametime; likely in a logic death spiral. Aborting program.");
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    


    //[BEGIN] Primary fixed update loop
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    gctx->time_perf.total_realtime_fixed_update_time = kinc_time();
    while (timing->accumulator >= timing->fixed_timestep_interval)
    {
        timing->accumulator -= timing->fixed_timestep_interval;

        Gamestate * gs = address_of(gctx->gamestate);
        v_gameloop_tick(gs);
    }
    gctx->time_perf.total_realtime_fixed_update_time = kinc_time() - gctx->time_perf.total_realtime_fixed_update_time;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}

void v_gameloop_entrypoint(void * data)
{
    Game_Context * context = static_cast<Game_Context *>(data);
    v_gameloop_fixed_update(context);
    v_gameloop_render(context);
}

//[Gamestate Queries]
Float_64 v_gamestate_adjusted_time(Gamestate const * gs)
{
    return gs->dt * gs->timescale;
}