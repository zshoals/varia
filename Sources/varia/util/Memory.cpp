#include "varia/util/Memory.hpp"
#include "varia/Validation.hpp"
#include <stdlib.h>

bool _initialized = false;
vds_allocator_t _permanent;
vds_allocator_t _scratch;
vds_allocator_t _image;

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

vds_allocator_t * varia_memory_get_image_allocator(void)
{
	ENSURE(_initialized, "The allocator subsystem was not initialized.");
	return &_image;	
}

vds_allocator_t * perm_allocator(void)
{
	return varia_memory_get_permanent_allocator();
}

vds_allocator_t * scratch_allocator(void)
{
	return varia_memory_get_scratch_allocator();
}

vds_allocator_t * image_allocator(void)
{
	return varia_memory_get_image_allocator();
}


void varia_memory_initialize_allocators(i64 permanent_size_bytes, i64 scratch_size_bytes, i64 image_size_bytes)
{
	ENSURE(!_initialized, "The allocator subsystem was already initialized.");

	vds_allocator_initialize(&_permanent, calloc(1, permanent_size_bytes), permanent_size_bytes);
	vds_allocator_initialize(&_scratch, calloc(1, scratch_size_bytes), scratch_size_bytes);
	vds_allocator_initialize(&_image, calloc(1, image_size_bytes), image_size_bytes);

	_initialized = true;
}

void varia_memory_reset_scratch_allocator(void)
{
	ENSURE(_initialized, "The allocator subsystem was not initialized.");
	vds_allocator_reset(&_scratch);	
}

void varia_memory_reset_image_allocator(void)
{
	ENSURE(_initialized, "The allocator subsystem was not initialized.");
	vds_allocator_reset(&_image);	
}
