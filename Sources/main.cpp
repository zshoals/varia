#include "kinc/window.h"
#include "kinc/system.h"
#include "kinc/display.h"

#include "varia/Vcommon.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/util/Memory.hpp"
#include "varia/ds/StaticArray.hpp"

#include <stdlib.h>

#include "dread/Dread.hpp"
#include "varia/tests/Test_Everything.hpp"

#include "varia/simd/f32q.hpp"
#include "varia/math/Vec2q.hpp"

#include "varia/Log.hpp"
#include "varia/util/Profiler.hpp"


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
	void * buffer = calloc(1, varia_memory_megabytes_to_bytes(16));
	vds_allocator_initialize(&mem, buffer, varia_memory_megabytes_to_bytes(16));

	Glog_initialize();
	varia_profiler_initialize(&mem);
	// vds_array_t<f32, 1024> float_arr;
	// vds_array_initialize(&float_arr);
	// vds_array_for_each_with_index(&float_arr, [](f32 * elem, size_t i)
	// {
	// 	*elem = i;
	// });

	vds_array_t<int> ints;
	vds_array_initialize(&ints, &mem, 128);


	for_range_var(i, 155)
	{
		vds_array_push(&ints, i);	
	}

	vds_array_iterate(&ints, [](int * elem, i64 i)
	{
		Glog_int(*elem);
		Glog_newline();
	});

	varia_profiler_print();
	varia_profiler_reset();

	int * result = vds_array_find_get(&ints, [](int * elem)
	{
		return (*elem == 124);
	});



	test_add_every_test_to_dread();
	dread_run_tests(dread_verbosity_e::Quiet);

	kinc_start();

	return 0;
}

