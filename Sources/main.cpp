#include "varia/Engine.hpp"

#include "varia/util/Memory.hpp"
#include "varia/util/Profiler.hpp"
#include "varia/Log.hpp"

#include "kinc/system.h"
#include "kinc/display.h"

#include "varia/ds/StaticStringMap2.hpp"
#include "varia/ds/String.hpp"

static varia_engine_context_t engine;

int kickstart(int argc, char** argv) 
{
	//=======RUN ALL TESTS======

	#ifndef VARIA_DISABLE_TESTS
		//we can return right here if we really want to
		//return 0;
	#endif

	//Note(zshoals 03-13-2023):> Varia global subsystems >>> NOT <<< dependent on Kinc
	varia_memory_initialize_allocators
	(
		varia_memory_megabytes_to_bytes(128), //Perm 
		varia_memory_megabytes_to_bytes(64), //Scratch
		varia_memory_megabytes_to_bytes(64) //Image loader
	);
	Glog_initialize();
	varia_profiler_initialize(varia_memory_get_scratch_allocator());



	//TODO(zshoals 03-21-2023):> We want to split every arg
	//And we want to collect any arg meeting any criteria...
	vds_string_t jack = vstr("-jack -nowindow -bob -nologo -res 1920 1080 - (error here)", perm_allocator());
	vds_string_t split = vstr(" ", perm_allocator());
	vds_array_t<vds_string_t> splits = vds_string_split_all(jack, split, perm_allocator());

	vds_array_iterate_cf(&splits, [](vds_string_t * elem, int64_t i, vds_controlflow_e * flow)
	{
		Glog_string(vds_string_raw(*elem));
		Glog_newline();

		vds_string_t nologo = vstr("-bob", perm_allocator());
		if (vds_string_equals(*elem, nologo)) { *flow = vds_controlflow_e::Break; }
	});

	Glog_print();
	Glog_clear_buffer();



	//Note(zshoals 03-18-2023):> Kinc init
	kinc_display_init();
	kinc_init("Varia", 800, 600, NULL, NULL);

	//Note(zshoals 03-18-2023):> Primary engine core init + gameloop start
	varia_engine_initialize(&engine);

	return 0;
}




