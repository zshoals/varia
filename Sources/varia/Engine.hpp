#pragma once

#include "kinc/system.h"
#include "kinc/window.h"
#include "kinc/display.h"

struct varia_loop_context_t
{
	double fixed_timestep;
	double max_frametime;
	double accumulator;
	double previous_time;

	double render_frametime_target;
	double render_accumulator;
	double render_previous_time;
	bool render_limiter_on;
};

struct varia_engine_configuration_t
{
	kinc_framebuffer_options_t framebuffer;
	kinc_window_options_t window;
};

void varia_engine_set_fixed_timestep(double timestep);
void varia_engine_set_framebuffer_options(kinc_framebuffer_options_t framebuffer_options);
void varia_engine_set_window_options(kinc_window_options_t window_options);

void varia_engine_fixed_update(void * UNUSED);
void varia_engine_render(double dt);
void varia_engine_initialize(void);
