#include "varia/Engine.hpp"

int kickstart(int argc, char** argv) 
{
	//=======RUN ALL TESTS======

	#ifndef VARIA_DISABLE_TESTS
		//we can return right here if we really want to
		//return 0;
	#endif

	varia_engine_initialize();

	return 0;
}





//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||       This stuff can be useful later on                             ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
	// char const * config_title = "Varia Project";
	// int config_window_width = 800;
	// int config_window_height = 600;
	// bool config_vsync = true;

	// //Initialize window and framebuffer options here with struct inits
	// //No crazy construction functions, just do it manually
	// //Eventually load settings from config file somewhere
	// int features = 
	// 	KINC_WINDOW_FEATURE_MINIMIZABLE | 
	// 	KINC_WINDOW_FEATURE_MAXIMIZABLE | 
	// 	KINC_WINDOW_FEATURE_RESIZEABLE;  


	// kinc_framebuffer_options fbo = {};
	// {
	// 	fbo.color_bits = 32;
	// 	fbo.depth_bits = 16;
	// 	fbo.frequency = 60; //Only useful in exclusive fullscreen mode, sets maximum refresh rate?
	// 	fbo.samples_per_pixel = 1;
	// 	fbo.stencil_bits = 8;
	// 	fbo.vertical_sync = config_vsync;
	// }

	// kinc_window_options_t wo = {};
	// {
	// 	wo.display_index = kinc_primary_display();
	// 	wo.width = config_window_width;
	// 	wo.height = config_window_height;
	// 	wo.title = config_title;
	// 	wo.visible = true;
	// 	wo.window_features = features;
	// 	wo.x = -1; //Default window position (centered)
	// 	wo.y = -1; //Default window position (centered)
	// 	wo.mode = KINC_WINDOW_MODE_WINDOW;
	// }

