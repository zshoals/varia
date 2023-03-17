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
#include "varia/graphics/Program.hpp"
#include "varia/graphics/Material.hpp"



static varia_engine_configuration_t engine_config;
static varia_loop_context_t loop_context;



void varia_engine_set_fixed_timestep(double timestep)
{
	loop_context.fixed_timestep = timestep;
}

void varia_engine_set_framebuffer_options(kinc_framebuffer_options_t framebuffer_options)
{
	//TODO(zshoals 03-14-2023):> Set me up!
}

void varia_engine_set_window_options(kinc_window_options_t window_options)
{
	//TODO(zshoals 03-14-2023):> Set me up!
}




void varia_engine_render(double dt)
{
	game_render(dt);
}

void varia_engine_fixed_update(void * UNUSED)
{
	double now = kinc_time();
	loop_context.accumulator += now - loop_context.previous_time;
	loop_context.previous_time = now;

	//Note(zshoals 03-13-2023):> Slowdown bailout
	if (loop_context.accumulator > loop_context.max_frametime) 
	{ 
		loop_context.accumulator = loop_context.max_frametime; 
	}

	//Note(zshoals 03-13-2023):> Fixed update 
	while (loop_context.accumulator >= loop_context.fixed_timestep)
	{
		game_simulate(loop_context.fixed_timestep);
		loop_context.accumulator -= loop_context.fixed_timestep;
	}



	//Note(zshoals 03-13-2023):> Render phase
	//Get as accurate of a timestamp as we can for this render attempt
	now = kinc_time();
	loop_context.render_accumulator += now - loop_context.render_previous_time;
	loop_context.render_previous_time = now;

	if //Vsync off, Frame limiter on
	(
		loop_context.render_limiter_on &&
		!engine_config.framebuffer.vertical_sync && 
		(loop_context.render_accumulator >= loop_context.render_frametime_target)
	)
	{
		varia_engine_render(loop_context.accumulator);
		loop_context.render_accumulator = 0.0;
	}
	else //Any other rendering config (if vsync is on, we ignore the frame limiter)
	{
		varia_engine_render(loop_context.accumulator);
	}
}

void varia_engine_initialize(void)
{
	//Note(zshoals 03-13-2023):> Kinc display subsystem (has to be called early)
	//	because window options in engine configuration needs to call a display function
	{
		kinc_display_init();
	}



	//Note(zshoals 03-13-2023):> Varia subsystems >>> NOT <<< dependent on Kinc
	{
		varia_memory_initialize_allocators
		(
			varia_memory_megabytes_to_bytes(128), //Perm
			varia_memory_megabytes_to_bytes(64), //Scratch
			varia_memory_megabytes_to_bytes(64) //Image loader
		);
		Glog_initialize();
		varia_profiler_initialize(varia_memory_get_scratch_allocator());

	}



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

		loop_context.fixed_timestep = 1.0 / 240.0; //240hz
		loop_context.max_frametime = 1.0 / 4.0;
		loop_context.accumulator = 0.0;
		loop_context.previous_time = 0.0;

		loop_context.render_frametime_target = 1.0 / 30.0; //480hz
		loop_context.render_accumulator = 0.0;
		loop_context.render_previous_time = 0.0;
		loop_context.render_limiter_on = true;

		engine_config.framebuffer = fbo;
		engine_config.window = wo;
	}





	//Note(zshoals 03-13-2023):> Kinc subsystems
	{
		kinc_init("Varia", 800, 600, &engine_config.window, &engine_config.framebuffer);
		kinc_set_update_callback(&varia_engine_fixed_update, NULL);
	}


	//Note(zshoals 03-14-2023):> Varia subsystems dependent on Kinc
	{
		//TODO(zshoals 03-14-2023):> This shouldn't be here, this should be in some graphics subsystem
		varia_graphics_program_initialize_defaults();
	}


	//Note(zshoals 03-13-2023):> Dread testing system
	{
		test_add_every_test_to_dread();
		dread_run_tests(dread_verbosity_e::Quiet);
	}



	kinc_start();

}
