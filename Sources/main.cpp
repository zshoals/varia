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

	vec2q sins = vec2q_unit_vector_from_angle(f32q_deg2rad(f32q_set_all(355.0f)));
	f32q in_ang = vec2q_atan2(sins);
	in_ang = f32q_rad2deg(in_ang);
	// vec2q sins = { f32q_set_all(1.0f), f32q_set_all(3.0f) };
	vec2q impact = vec2q_unit_vector_from_angle(f32q_PI());
	f32q impact_ang = vec2q_atan2(impact);
	impact_ang = f32q_rad2deg(impact_ang);

	VARIA_QLOG("Before");
	VARIA_LOG_FLOAT(f32q_get(vec2q_atan2(sins), 0));

	sins = vec2q_reflect(sins, impact);
	f32q ang = vec2q_atan2(sins);
	ang = f32q_rad2deg(ang);

	VARIA_QLOG("After");
	VARIA_LOG_FLOAT(f32q_get(ang, 0));
	VARIA_LOG_FLOAT(f32q_get(in_ang, 0));
	VARIA_LOG_FLOAT(f32q_get(impact_ang, 0));

	// VARIA_LOG_FLOAT(kinc_float32x4_get(sins_out, 0));
	// VARIA_LOG_FLOAT(kinc_float32x4_get(sins_out, 1));
	// VARIA_LOG_FLOAT(kinc_float32x4_get(sins_out, 2));
	// VARIA_LOG_FLOAT(kinc_float32x4_get(sins_out, 3));

	test_add_every_test_to_dread();
	dread_run_tests(dread_verbosity_e::Quiet);

	kinc_start();

	return 0;
}
