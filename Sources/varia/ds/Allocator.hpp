#pragma once

#include "varia/Vcommon.hpp"

#define vds_allocator_malloc(ALLOC_PTR, TYPE, COUNT) static_cast<TYPE *>( vds_allocator_allocate_aligned((ALLOC_PTR), sizeof(TYPE) * (COUNT), alignof(TYPE)))
#define vds_allocator_malloc_aligned(ALLOC_PTR, TYPE, COUNT, ALIGN) static_cast<TYPE *>( vds_allocator_allocate_aligned((ALLOC_PTR), sizeof(TYPE) * (COUNT), (ALIGN)))

struct vds_allocator_t
{
	void * data;
	size_t current_offset;
	size_t buffer_len;
};

void vds_allocator_initialize(VSELF vds_allocator_t * self, void * memory, size_t buffer_size);
void * vds_allocator_allocate_aligned(VSELF vds_allocator_t * self, size_t size, size_t alignment);
void * vds_allocator_allocate_aligned_count(VSELF vds_allocator_t * self, size_t elem_size, size_t count, size_t alignment);
void vds_allocator_reset(VSELF vds_allocator_t * self);
void vds_allocator_zero(VSELF vds_allocator_t * self);

void vds_allocator_copy_to(VSELF vds_allocator_t * destination, vds_allocator_t * source);
