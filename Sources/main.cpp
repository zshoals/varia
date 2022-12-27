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

#include "varia/exd/View.hpp"

#include "varia/comps/Flammable.hpp"
#include "varia/comps/Position.hpp"
#include "varia/exd/ComponentTypes.hpp"

#include "varia/logging.hpp"
#include "varia/ds/Bits.hpp"
#include "varia/comps/Position.hpp"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

using namespace Varia;

inline void pos_system(exd::View * v, exd::Entity ent)
{
	Position * p = static_cast<Position *>(v->comp_get_mutable(ent, exd::ComponentTypeID::Position_e));
	Flammable * f = static_cast<Flammable *>(v->comp_get_mutable(ent, exd::ComponentTypeID::Flammable_e));
	p->x = ent.id_extract();
	p->y = ent.generation_extract();
}

inline void print_system(exd::View * v, exd::Entity ent)
{
	Position * p = static_cast<Position *>(v->comp_get_mutable(ent, exd::ComponentTypeID::Position_e));

	if (ent.id_extract() == 5000)
	{
		// VARIA_LOG_INT(p->x);
		// VARIA_LOG_INT(p->y);
		p->x += 1;
	}
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
	w->comp_register(sizeof(struct Position), exd::ComponentTypeID::Position_e);
	w->comp_register(sizeof(struct Flammable), exd::ComponentTypeID::Flammable_e);

	vds::StaticArray<Entity, 8192> entlist;
	entlist.initialize();

	#define COMP_SET(ENT, TYPE) w->comp_set((ENT), (exd::ComponentTypeID::VARIA_CONCAT(TYPE, _e)))

	{
		VARIA_LOG_STRING("Entity creation");
		Elapsed t;
		for_range(8000)
		{
			Entity ent = w->ent_create();
			COMP_SET(ent, Position);
			COMP_SET(ent, Flammable);
		}
	}

	exd::View v = {};
	{
		VARIA_LOG_STRING("View creation");
		Elapsed t;
		v.initialize();
		v.include(&w->components[0]);
		v.include(&w->components[1]);
		v.compile();
	}

	{
		VARIA_LOG_STRING("Position update");
		Elapsed t;
		ITERATE_VIEW(&v, pos_system);
	}

	{
		VARIA_LOG_STRING("Print out");
		Elapsed t;
		ITERATE_VIEW(&v, print_system);
	}





	kinc_start();

	return 0;
}


