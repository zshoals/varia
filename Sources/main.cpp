#include "kinc/system.h"

#include "varia/Engine.hpp"
#include "varia/util/Memory.hpp"
#include "varia/util/Profiler.hpp"
#include "varia/Log.hpp"

static varia_engine_context_t engine;

int kickstart(int argc, char** argv) 
{
	//=======RUN ALL TESTS======

	#ifndef VARIA_DISABLE_TESTS
		//we can return right here if we really want to
		//return 0;
	#endif

	//Note(zshoals 03-13-2023):> Varia subsystems >>> NOT <<< dependent on Kinc
	varia_memory_initialize_allocators
	(
		varia_memory_megabytes_to_bytes(128), //Perm
		varia_memory_megabytes_to_bytes(64), //Scratch
		varia_memory_megabytes_to_bytes(64) //Image loader
	);
	Glog_initialize();
	varia_profiler_initialize(varia_memory_get_scratch_allocator());

	//Note(zshoals 03-18-2023):> Kinc init
	kinc_display_init();
	kinc_init("Varia", 800, 600, NULL, NULL);

	//Note(zshoals 03-18-2023):> Primary engine core init + gameloop start
	varia_engine_initialize(&engine);

	return 0;
}




