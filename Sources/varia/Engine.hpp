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

struct varia_kinc_configuration_t
{
	kinc_framebuffer_options_t framebuffer;
	kinc_window_options_t window;
};

struct varia_environment_t
{
	float window_width;
	float window_height;
};

struct varia_engine_context_t
{
	varia_environment_t environment;
	varia_loop_context_t loop_context;
	varia_kinc_configuration_t kinc_context;
};

void varia_engine_set_fixed_timestep(varia_engine_context_t * engine, double timestep);
void varia_engine_set_framebuffer_options(varia_engine_context_t * engine, kinc_framebuffer_options_t framebuffer_options);
void varia_engine_set_window_options(varia_engine_context_t * engine, kinc_window_options_t window_options);

float varia_engine_get_window_width(varia_engine_context_t * engine);
float varia_engine_get_window_height(varia_engine_context_t * engine);

void varia_engine_fixed_update(void * engine_context); 
void varia_engine_render(varia_engine_context_t * engine, double dt);
void varia_engine_initialize(varia_engine_context_t * engine);
