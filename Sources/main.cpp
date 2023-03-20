#include "varia/Engine.hpp"

#include "varia/util/Memory.hpp"
#include "varia/util/Profiler.hpp"
#include "varia/Log.hpp"

#include "kinc/system.h"
#include "kinc/display.h"

#include "varia/ds/StaticStringMap2.hpp"

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

	vds_string_map_t<int> bob;
	vds_string_map_initialize(&bob, varia_memory_get_permanent_allocator(), 4);
	vds_string_map_set(&bob, "Herro!", 1337);
	vds_string_map_set(&bob, "Herro!", 1337);
	vds_string_map_set(&bob, "Herro!", 1337);
	vds_string_map_set(&bob, "Herro!", 1337);
	vds_string_map_set(&bob, "Herro!", 1337);
	vds_string_map_set(&bob, "Herro!", 1337);
	vds_string_map_set(&bob, "Herro!", 1337);
	vds_string_map_set(&bob, "gjrggrdgr", 436);
	vds_string_map_set(&bob, "gGDDDDD", 43444444);
	vds_string_map_set(&bob, "pokemon gotta catch em all", 2222);
	vds_string_map_set(&bob, "FUUUUUU", 999999);

	vds_option_t<int *> item = bob["pokemon gotta catch em all"];
	if (item)
	{
		Glog_string("OMFG!!! FOUND IT!!!!");
		Glog_print();
	}
	else
	{
		Glog_string("OH NO!!!!!! FUCK!!!!!!!!!!!");
		Glog_print();
	}

	//Note(zshoals 03-18-2023):> Kinc init
	kinc_display_init();
	kinc_init("Varia", 800, 600, NULL, NULL);

	//Note(zshoals 03-18-2023):> Primary engine core init + gameloop start
	varia_engine_initialize(&engine);

	return 0;
}




