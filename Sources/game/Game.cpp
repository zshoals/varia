#include "Game.hpp"
#include "varia/Engine.hpp"

#include "kinc/system.h"
#include "varia/Log.hpp"

static double accum = 0.0;
static double lasttime = 0.0;

void game_render(varia_engine_context_t * engine, double dt)
{
		
}


void game_simulate(varia_engine_context_t * engine, double dt)
{
	// double now = kinc_time();
	// accum += now - lasttime;
	// lasttime = now;

	// Glog_string("Sim: "); Glog_float(now);
	// Glog_newline();

	// if (accum >= 5.0)
	// {
	// 	accum = 0.0;

	// 	Glog_print();
	// 	Glog_clear_buffer();
	// }
}
