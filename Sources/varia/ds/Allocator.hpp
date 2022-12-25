#pragma once

namespace vds
{

#define allocator_malloc(ALLOC_PTR, TYPE, COUNT) ALLOC_PTR->allocate_aligned(sizeof(TYPE) * (COUNT), alignof(TYPE))

struct Allocator
{
	void * data;
	size_t current_offset;
	const size_t buffer_len;
	const size_t element_size;

	Allocator(void * memory, size_t buffer_size, size_t element_size);
	void * allocate_aligned(size_t size, size_t alignment);
	void * free_all(void);
	void zero(void);
};

}