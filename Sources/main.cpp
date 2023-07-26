#include "kinc/window.h"
#include "kinc/system.h"
#include "kinc/display.h"
#include "kinc/log.h"

#include "varia/VShared.hpp"
#include "varia/VGameloop.hpp"
#include "varia/ds/VDS-Array.hpp"

#include "varia/VInput.hpp"

void mr_keydown(Virtual_Move_Right_Data * data)
{
	kinc_log(KINC_LOG_LEVEL_INFO, "Hello! I was pressed! %f\n", data->movement_multiplier);
}

void v_initialize_input(Input_Virtual_Action_State * state)
{
		Virtual_Move_Right_Data mrdata = { 69.0f };
		state->move_right_action.bound_key = KINC_KEY_E;
		state->move_right_action.on_keydown = &mr_keydown;
		state->move_right_action.on_keyup = &mr_keydown;
		state->move_right_action.data = mrdata;

		v_input_register_processing_functions(state);
}

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
		game.logic_world.fixed_timestep_interval = 1.0 / 480.0;
		game.logic_world.max_frametime = 1.0 / 16.0;
		game.logic_world.max_frametime_overrun_threshold = 10;

		//[Gamestate Defaults]
		game.logic_world.logic_dt = game.logic_world.fixed_timestep_interval;
		game.logic_world.logic_timescale = 1.0;

		//[Gameloop Configuration]
		game.logic_world.enable_excessive_frametime_exit = true;
		game.logic_world.enable_framerate_limit = true;
		game.logic_world.fps_limit = 1.0 / 144.0;

		//[Kinc Settings]
		game.logic_world.window = wo;
		game.logic_world.framebuffer = fbo;

	}
	
	kinc_init(config_title, config_window_width, config_window_height, &wo, &fbo);
	v_initialize_input(address_of(game.logic_world.input));
	kinc_set_update_callback(&v_gameloop_entrypoint, &game);
	kinc_start();

	return 0;
}
