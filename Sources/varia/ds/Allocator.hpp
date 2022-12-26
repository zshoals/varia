#pragma once

namespace vds
{

#define allocator_malloc(ALLOC_PTR, TYPE, COUNT) static_cast<TYPE *>(ALLOC_PTR->allocate_aligned(sizeof(TYPE) * (COUNT), alignof(TYPE)))

struct Allocator
{
	void * data;
	size_t current_offset;
	size_t buffer_len;

	// Allocator(void * memory, size_t buffer_size);
	void initialize(void * memory, size_t buffer_size);
	void * allocate_aligned(size_t size, size_t alignment);
	void * allocate_aligned_count(size_t elem_size, size_t count, size_t alignment);
	void free_all(void);
	void zero(void);
};

}