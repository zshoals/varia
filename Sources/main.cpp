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

#include "varia/ds/StaticRingbuf.hpp"
#include "varia/util/Memory.hpp"

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

	varia_memory_initialize_allocators(varia_memory_megabytes_to_bytes(128), varia_memory_megabytes_to_bytes(64));

	Glog_initialize();
	varia_profiler_initialize(varia_memory_scratch_allocator());

	vds_ringbuf_t<double> times;
	vds_ringbuf_initialize(&times, varia_memory_scratch_allocator(), 4);

	vds_ringbuf_push(&times, 946334363.342);
	vds_ringbuf_push(&times, 1.0463);
	vds_ringbuf_push(&times, 99999.0);
	vds_ringbuf_push(&times, 99999.0);
	vds_ringbuf_push(&times, 99999.0);
	vds_ringbuf_push(&times, (double)0x8000);
	vds_ringbuf_pop(&times);
	vds_ringbuf_pop(&times);
	vds_ringbuf_pop(&times);
	vds_ringbuf_pop(&times);
	vds_ringbuf_pop(&times);
	vds_ringbuf_pop(&times);
	vds_ringbuf_pop(&times);
	vds_ringbuf_push(&times, 1.0463);
	vds_ringbuf_push(&times, 1.0463);
	// vds_ringbuf_push(&times, 13333337.0);
	// vds_ringbuf_push(&times, 666666666666.0);

	vds_ringbuf_iterate(&times, [](double * elem, i64 i)
	{
		Glog_double(*elem);
		Glog_newline();
	});

	Glog_print();

	test_add_every_test_to_dread();
	dread_run_tests(dread_verbosity_e::Quiet);

	kinc_start();

	return 0;
}

