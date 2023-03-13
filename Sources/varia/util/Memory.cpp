#include "varia/util/Memory.hpp"
#include "varia/Validation.hpp"
#include <stdlib.h>

bool _initialized = false;
vds_allocator_t _permanent;
vds_allocator_t _scratch;

vds_allocator_t * varia_memory_get_permanent_allocator(void)
{
	ENSURE(_initialized, "The allocator subsystem was not initialized.");
	return &_permanent;
}

vds_allocator_t * varia_memory_get_scratch_allocator(void)
{
	ENSURE(_initialized, "The allocator subsystem was not initialized.");
	return &_scratch;
}

void varia_memory_initialize_allocators(i64 permanent_size_bytes, i64 scratch_size_bytes)
{
	ENSURE(!_initialized, "The allocator subsystem was already initialized.");
	vds_allocator_initialize(&_permanent, calloc(1, permanent_size_bytes), permanent_size_bytes);
	vds_allocator_initialize(&_scratch, calloc(1, scratch_size_bytes), scratch_size_bytes);

	_initialized = true;
}

void varia_memory_reset_scratch_allocator(void)
{
	ENSURE(_initialized, "The allocator subsystem was not initialized.");
	vds_allocator_reset(&_scratch);	
}
