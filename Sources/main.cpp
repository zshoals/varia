#include "kinc/window.h"
#include "kinc/system.h"
#include "kinc/display.h"
#include "kinc/log.h"
#include "varia/ds/StaticArray.hpp"
#include "varia/ds/StaticRingbuf.hpp"
#include "varia/ds/StaticQueue.hpp"
#include "varia/ds/Bits.hpp"
#include "varia/ds/Bitset.hpp"
#include "varia/math/Math.hpp"
#include "varia/util/Elapsed.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/util/Memory.hpp"

#include "varia/exd/Component.hpp"
#include "varia/exd/World.hpp"
#include "varia/exd/Entity.hpp"

#include "varia/comps/Position.hpp"

#include "varia/logging.hpp"
#include "varia/ds/Bits.hpp"
#include "varia/comps/Position.hpp"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

using namespace Varia;

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
		fbo.frequency = 60; //Only useful in fullscreen mode, sets maximum refresh rate?
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
	// kinc_set_update_callback(&mainloop);


	using exd::Component;
	using exd::World;
	using exd::Entity;
	using vds::Allocator;

	size_t mem_size = Varia::Memory::megabytes_to_bytes(128);
	void * mem = static_cast<void *>(malloc(mem_size));
	Allocator arena;
	arena.initialize(mem, mem_size);

	World * w = allocator_malloc(&arena, World, 1);
	w->initialize(&arena);
	w->comp_register(sizeof(Position));

	vds::StaticArray<Entity, 4000> arr;
	arr.initialize();

	for_range_var(i, 4000)
	{
		Entity ent = w->ent_create();
		arr.push(ent);
		Component * positions = &w->components[0];
		positions->entity_add(ent);

		Position * p = static_cast<Position *>(positions->get_untyped_mutable(ent));

		p->x = i;
		p->y = 4000 + i;
	}

	for(Entity const & ent : arr)
	{
		Component * positions = &w->components[0];

		Position const  * p = static_cast<Position const *>(positions->get_untyped(ent));

		if (positions->has(ent))
		{
			VARIA_LOG_INT(p->x);
			VARIA_LOG_INT(p->y);
		}
	}


	kinc_start();

	return 0;
}


