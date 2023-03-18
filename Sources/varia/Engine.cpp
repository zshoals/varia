#include "Engine.hpp"

#include "kinc/system.h"
#include "kinc/display.h"
#include "kinc/window.h"

#include "varia/util/Memory.hpp"
#include "varia/Log.hpp"
#include "varia/util/Profiler.hpp"

#include "dread/Dread.hpp"
#include "varia/tests/Test_Everything.hpp"

#include "game/Game.hpp"



void varia_engine_set_fixed_timestep(varia_engine_context_t * engine, double timestep)
{
	engine->loop_context.fixed_timestep = timestep;
}

void varia_engine_set_framebuffer_options(varia_engine_context_t * engine, kinc_framebuffer_options_t framebuffer_options)
{
	//TODO(zshoals 03-14-2023):> Set me up!
}

void varia_engine_set_window_options(varia_engine_context_t * engine, kinc_window_options_t window_options)
{
	//TODO(zshoals 03-14-2023):> Set me up!
}


int varia_engine_get_window_width(varia_engine_context_t * engine)
{
	return engine->environment.window_width;
}

int varia_engine_get_window_height(varia_engine_context_t * engine)
{
	return engine->environment.window_height;
}


void varia_engine_render(varia_engine_context_t * engine, double dt)
{
	game_render(engine, dt);
}

void varia_engine_fixed_update(void * engine_context)
{
	varia_engine_context_t * engine = (varia_engine_context_t *)engine_context;
	double now = kinc_time();
	engine->loop_context.accumulator += now - engine->loop_context.previous_time;
	engine->loop_context.previous_time = now;

	//Note(zshoals 03-13-2023):> Slowdown bailout
	if (engine->loop_context.accumulator > engine->loop_context.max_frametime) 
	{ 
		engine->loop_context.accumulator = engine->loop_context.max_frametime; 
	}

	//Note(zshoals 03-13-2023):> Fixed update 
	while (engine->loop_context.accumulator >= engine->loop_context.fixed_timestep)
	{
		game_simulate(engine, engine->loop_context.fixed_timestep);
		engine->loop_context.accumulator -= engine->loop_context.fixed_timestep;
	}



	//Note(zshoals 03-13-2023):> Render phase
	//Get as accurate of a timestamp as we can for this render attempt
	now = kinc_time();
	engine->loop_context.render_accumulator += now - engine->loop_context.render_previous_time;
	engine->loop_context.render_previous_time = now;

	if //Vsync off, Frame limiter on
	(
		engine->loop_context.render_limiter_on &&
		!engine->kinc_context.framebuffer.vertical_sync && 
		(engine->loop_context.render_accumulator >= engine->loop_context.render_frametime_target)
	)
	{
		varia_engine_render(engine, engine->loop_context.accumulator);
		engine->loop_context.render_accumulator = 0.0;
	}
	else //Any other rendering config (if vsync is on, we ignore the frame limiter)
	{
		varia_engine_render(engine, engine->loop_context.accumulator);
	}
}

void varia_engine_initialize(varia_engine_context_t * engine)
{

	//Note(zshoals 03-13-2023):> Engine configuration
	{
		char const * config_title = "Varia Project";
		int config_window_width = 800;
		int config_window_height = 600;
		bool config_vsync = true;

		// int features = 
		// 	KINC_WINDOW_FEATURE_MINIMIZABLE | 
		// 	KINC_WINDOW_FEATURE_MAXIMIZABLE | 
		// 	KINC_WINDOW_FEATURE_RESIZEABLE;  

		//No window features for now, not necessary
		int features = 0;


		kinc_framebuffer_options fbo = {};
		{
			fbo.color_bits = 32;
			fbo.depth_bits = 16;
			fbo.frequency = 60; //Only useful in exclusive fullscreen mode, sets maximum refresh rate?
			fbo.samples_per_pixel = 1;
			fbo.stencil_bits = 8;
			fbo.vertical_sync = config_vsync;
		}

		kinc_window_options_t wo = {};
		{
			wo.display_index = kinc_primary_display();
			wo.width = config_window_width;
			wo.height = config_window_height;
			wo.title = config_title;
			wo.visible = true;
			wo.window_features = features;
			wo.x = -1; //Default window position (centered)
			wo.y = -1; //Default window position (centered)
			wo.mode = KINC_WINDOW_MODE_WINDOW;
		}

		engine->loop_context.fixed_timestep = 1.0 / 240.0; //240hz
		engine->loop_context.max_frametime = 1.0 / 4.0;
		engine->loop_context.accumulator = 0.0;
		engine->loop_context.previous_time = 0.0;

		engine->loop_context.render_frametime_target = 1.0 / 30.0; //480hz
		engine->loop_context.render_accumulator = 0.0;
		engine->loop_context.render_previous_time = 0.0;
		engine->loop_context.render_limiter_on = true;

		engine->kinc_context.framebuffer = fbo;
		engine->kinc_context.window = wo;

		int primary = kinc_primary_display();
		kinc_window_resize(primary, wo.width, wo.height);
		kinc_window_change_features(primary, features);
		kinc_window_change_mode(primary, wo.mode);
		kinc_window_set_title(primary, wo.title);
		kinc_window_change_framebuffer(primary, &fbo);
	}

	//TODO(zshoals 03-17-2023):> These environment vars need to be updated on window resize
	engine->environment.window_width = kinc_width();
	engine->environment.window_height = kinc_height();

	//Note(zshoals 03-18-2023):> Graphics subsystem
	varia_graphics_initialize(&engine->graphics_context);

	//Note(zshoals 03-13-2023):> Dread testing system
	test_add_every_test_to_dread();
	dread_run_tests(dread_verbosity_e::Quiet);

	//Note(zshoals 03-18-2023):> Begin our game loop!
	kinc_set_update_callback(&varia_engine_fixed_update, engine);
	kinc_start();
}
