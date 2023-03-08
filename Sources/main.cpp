#include "kinc/window.h"
#include "kinc/system.h"
#include "kinc/display.h"

#include "varia/vcommon.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/util/Memory.hpp"
#include "varia/logging.hpp"
#include "varia/ds/StringView.hpp"
#include "varia/ds/StaticArray.hpp"

#include <stdlib.h>

#include "varia/io/File.hpp"
#include "varia/util/Elapsed.hpp"

#include "dread/Dread.hpp"
#include "varia/tests/Test_Everything.hpp"

#include "varia/simd/f32q.hpp"
#include "varia/math/Vec2q.hpp"

#include "varia/log.hpp"


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

	char const * config_title = "Tectonic Project";
	int config_window_width = 800;
	int config_window_height = 600;
	bool config_vsync = true;

	//Initialize window and framebuffer options here with struct inits
	//No crazy construction functions, just do it manually
	//Eventually load settings from config file somewhere
	int features = 
		KINC_WINDOW_FEATURE_MINIMIZABLE | 
		KINC_WINDOW_FEATURE_MAXIMIZABLE | 
		KINC_WINDOW_FEATURE_RESIZEABLE;  


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


	kinc_init("Varia", 800, 600, NULL, NULL);


	vds_allocator_t mem;
	void * buffer = calloc(1, varia_memory_kilobytes_to_bytes(1));
	vds_allocator_initialize(&mem, buffer, varia_memory_kilobytes_to_bytes(1));

	vds_array_t<f32, 1024> float_arr;
	vds_array_initialize(&float_arr);
	vds_array_for_each_with_index(&float_arr, [](f32 * elem, size_t i)
	{
		*elem = i;
	});


	test_add_every_test_to_dread();
	dread_run_tests(dread_verbosity_e::Quiet);




	Glog_initialize();

	Glog_string("Hello there, my name is Jackson");
	Glog_newline();
	Glog_string("This is the end of the world, and you're not invited.");
	Glog_newline();
	Glog_string("Countdown: "); 
	Glog_time();
	Glog_print();
	Glog_clear_buffer();

	Glog_string("We couldn't believe that you had done this....");
	Glog_newline();
	Glog_string("This was "); Glog_time(); Glog_string(" seconds ago, and you hurt us. Badly.");
	Glog_print();
	Glog_clear_buffer();

	f32q_Gprint_info(f32q_set_all(0.44634f), "This is a Float32x4");
	Glog_print();

	// f32q bbo = f32q_create(1.0f, 500.f, 300.f, 33209.f);

	// f32q_print(bbo);
	// f32q_print(bbo);
	// f32q_print(bbo);
	// f32q_print(bbo);

	kinc_start();

	return 0;
}
