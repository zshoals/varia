#include "Profiler.hpp"

#include "kinc/system.h"

#include "varia/Log.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/ds/StaticArray.hpp"

static vds_array_t<varia_profiler_data_t> _profiler_storage;
static bool initialized = false;

varia_profiler_t::varia_profiler_t(char const * func_name)
{
	if (initialized)
	{
		this->function_time_begin = kinc_time();
		this->function_name = func_name;
	}
}

varia_profiler_t::~varia_profiler_t()
{
	if (initialized)
	{
		varia_profiler_data_t data;
		data.function_name = this->function_name;
		data.function_time = kinc_time() - this->function_time_begin;

		vds_array_push(&_profiler_storage, data);
	}
}

void varia_profiler_initialize(vds_allocator_t * alloc)
{
	vds_array_initialize(&_profiler_storage, alloc, VARIA_PROFILER_MAX_COLLECTIONS);
	initialized = true;
}

void varia_profiler_print(void)
{
	vds_array_iterate(&_profiler_storage, [](varia_profiler_data_t * elem, i64 i)
	{
		Glog_string("Function time of "); Glog_string(elem->function_name); Glog_string(":");
		Glog_double(elem->function_time);
		Glog_newline();
	});

	Glog_print();
	Glog_clear_buffer();
}

void varia_profiler_reset(void)
{
	vds_array_reset(&_profiler_storage);
}
