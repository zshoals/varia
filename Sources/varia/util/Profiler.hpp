#pragma once

#include "varia/Vcommon.hpp"

struct vds_allocator_t;

#define VARIA_PROFILER_MAX_COLLECTIONS 25000

struct varia_profiler_data_t
{
	double function_time;
	char const * function_name;
};

struct varia_profiler_t
{
	double function_time_begin;
	char const * function_name;
	varia_profiler_t(char const * func_name);
	~varia_profiler_t();
};

#define VARIA_PROFILE_FUNCTION() varia_profiler_t VARIA_MACRO_VAR(profiler) = { VARIA_FUNCTION_NAME }

void varia_profiler_initialize(vds_allocator_t * alloc);
void varia_profiler_print(void);
void varia_profiler_reset(void);