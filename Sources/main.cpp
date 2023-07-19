#include "kinc/window.h"
#include "kinc/system.h"
#include "kinc/display.h"
#include "kinc/log.h"

#include "varia/VCommon.hpp"
#include "varia/VGameloop.hpp"
#include "varia/ds/VDS-Array.hpp"

int kickstart(int argc, char** argv) 
{
	//=======RUN ALL TESTS======

	#ifndef VARIA_DISABLE_TESTS
		//we can return right here if we really want to
		//return 0;
	
	
	#endif

	//========TESTS END=========

	//TODO(zshoals): THIS NEEDS TO BE IN THE RIGHT LOCATION, NOT HERE REALLY
	//Although it's not a big deal to be here either I guess, but I think it should be in our engine layer
	//Also, it works completely fine, looking good so far.

	kinc_display_init();

	char const * config_title = "Varia Project";
	int config_window_width = 800;
	int config_window_height = 600;
	bool config_vsync = false;

	//Initialize window and framebuffer options here with struct inits
	//No crazy construction functions, just do it manually
	//Eventually load settings from config file somewhere
	int features = 
		KINC_WINDOW_FEATURE_MINIMIZABLE | 
		KINC_WINDOW_FEATURE_MAXIMIZABLE | 
		KINC_WINDOW_FEATURE_RESIZEABLE;  


	kinc_framebuffer_options fbo = ZERO_INIT();
	{
		fbo.color_bits = 32;
		fbo.depth_bits = 16;
		fbo.frequency = 60; //Only useful in exclusive fullscreen mode, sets maximum refresh rate?
		fbo.samples_per_pixel = 1;
		fbo.stencil_bits = 8;
		fbo.vertical_sync = config_vsync;
	}

	kinc_window_options_t wo = ZERO_INIT();
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

	static Game_Context game = ZERO_INIT();
	{
		//[Timing Defaults]
		game.timing.fixed_timestep_interval = 1.0 / 120.0;
		game.timing.max_frametime = 1.0 / 16.0;
		game.timing.max_frametime_overrun_threshold = 10;

		//[Gamestate Defaults]
		game.gamestate.logic_dt = game.timing.fixed_timestep_interval;
		game.gamestate.timescale = 1.0;

		//[Gameloop Configuration]
		game.gamestate.loop_config.enable_excessive_frametime_exit = true;
		game.gamestate.loop_config.enable_framerate_limit = true;
		game.gamestate.loop_config.render_frames_per_second_limit = 1.0 / 300.0;

		//[Kinc Settings]
		game.gamestate.kinc.window = wo;
		game.gamestate.kinc.framebuffer = fbo;
	}
	
	kinc_init(config_title, config_window_width, config_window_height, &wo, &fbo);
	kinc_set_update_callback(&v_gameloop_entrypoint, &game);
	kinc_start();

	return 0;
}
