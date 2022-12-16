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

#include "varia/exd/World.hpp"
#include "varia/exd/Component.hpp"


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

	using exd::World;
	using exd::Component;
	using exd::Entity;

	World<8192> * w = new World<8192>();
	Entity ent = w->ent_create();
	Position * pos = w->positions0.comp_set(w, ent);
	pos->x = 100;
	pos->y = 200;

	Position const * pos_edit = w->positions0.comp_get(w, ent);
	VARIA_LOG_INT(pos_edit->x);
	VARIA_LOG_INT(pos_edit->y);

	vds::StaticArray<Entity, 4000> cont = {};

	Elapsed timer = {};
	timer.begin();
	for_range(4000)
	{
		cont.push(w->ent_create());
	}
	timer.end();
	VARIA_LOG_FLOAT(timer.dt());

	timer.reset();
	timer.begin();
	for_range_var(i, 4000)
	{
		Entity ent = *cont.get_mut(i);
		Position * pos = w->positions0.comp_set_unchecked(ent);
		pos->x = 100;
		pos->y = 99999;
	}
	timer.end();
	VARIA_LOG_FLOAT(timer.dt());

	timer.reset();
	timer.begin();
	for_range(4000)
	{
		w->ent_kill(cont.pop());
	}
	timer.end();
	VARIA_LOG_FLOAT(timer.dt());

	kinc_start();

	return 0;
}

