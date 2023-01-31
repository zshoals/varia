#include "Allocator.hpp"
#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include <inttypes.h>
#include <string.h>

// vds::Allocator::Allocator(void * memory, size_t buffer_size) : 
// 	data{memory}, buffer_len{buffer_size}, current_offset{0}{}

void vds_allocator_initialize(vds_allocator_t * self, void * memory, size_t buffer_size)
{
	self->data = memory;
	self->buffer_len = buffer_size;
	self->current_offset = 0;

	memset(self->data, 255, self->buffer_len);
}

void * vds_allocator_allocate_aligned(vds_allocator_t * self, size_t size, size_t alignment)
{
	DEBUG_ENSURE_UINT_GT_ZERO(size, "Tried to make an empty memory allocation.");

	uintptr_t base_addr = reinterpret_cast<uintptr_t>(self->data); 
	base_addr += self->current_offset;

	uintptr_t misalign = base_addr % alignment;
	uintptr_t target_address = base_addr + (alignment - misalign);
	size_t offset_advance_amount = misalign + size;

	DEBUG_ENSURE_UINT_LT(self->current_offset + offset_advance_amount, self->buffer_len, "Allocator OOM");

	self->current_offset += offset_advance_amount;

	return reinterpret_cast<void *>(target_address);
}

void * vds_allocator_allocate_aligned_count(vds_allocator_t * self, size_t elem_size, size_t count, size_t alignment)
{
	return vds_allocator_allocate_aligned(self, (elem_size * count), alignment);
}

void vds_allocator_free_all(vds_allocator_t * self)
{
	self->current_offset = 0;
}

void vds_allocator_zero(vds_allocator_t * self)
{
	memset(self->data, 0, self->buffer_len);
}

void vds_allocator_copy_to(vds_allocator_t * destination, vds_allocator_t * source)
{
	DEBUG_ENSURE_UINT_EQUALS(source->buffer_len, destination->buffer_len, "Allocator tried to copy data but sizes were mismatched.");
	memcpy(destination->data, source->data, destination->buffer_len);

	destination->current_offset = source->current_offset;
}