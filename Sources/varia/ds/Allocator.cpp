#include "Allocator.hpp"
#include "varia/Vcommon.hpp"
#include "varia/Validation.hpp"
#include <inttypes.h>
#include <string.h>

// vds::Allocator::Allocator(void * memory, size_t buffer_size) : 
// 	data{memory}, buffer_len{buffer_size}, current_offset{0}{}

void vds_allocator_initialize(vds_allocator_t * self, void * memory, size_t buffer_size)
{
	ENSURE(memory != nullptr, "vds_allocator_t:> Provided memory location was null");

	self->data = memory;
	self->buffer_len = buffer_size;
	self->current_offset = 0;

	memset(self->data, 255, self->buffer_len);
}

void * vds_allocator_allocate_aligned(vds_allocator_t * self, size_t size, size_t alignment)
{
	DEBUG_ENSURE(size > 0, "Tried to make an empty memory allocation.");
	size_t _DEBUG_MAX_ALLOCATION_SIZE = 1 * 1000 * 1000 * 1000; //1 Gigabyte
	DEBUG_ENSURE(size < _DEBUG_MAX_ALLOCATION_SIZE, "Tried to make a very large allocation(1 GB+). Was this intended?");

	uintptr_t base_addr = reinterpret_cast<uintptr_t>(self->data); 
	base_addr += self->current_offset;

	uintptr_t misalign = base_addr % alignment;
	uintptr_t target_address = base_addr + (alignment - misalign);
	size_t offset_advance_amount = misalign + size;

	//Note(zshoals 02-08-2023):> Better to be safe than sorry; always check that the allocation size is ok
	ENSURE( ((self->current_offset + offset_advance_amount) < self->buffer_len), "Allocator OOM");


	self->current_offset += offset_advance_amount;

	return reinterpret_cast<void *>(target_address);
}

void * vds_allocator_allocate_aligned_count(vds_allocator_t * self, size_t elem_size, size_t count, size_t alignment)
{
	return vds_allocator_allocate_aligned(self, (elem_size * count), alignment);
}

void vds_allocator_reset(vds_allocator_t * self)
{
	self->current_offset = 0;
}

void vds_allocator_zero(vds_allocator_t * self)
{
	memset(self->data, 0, self->buffer_len);
}

void vds_allocator_copy_to(vds_allocator_t * destination, vds_allocator_t * source)
{
	DEBUG_ENSURE(source->buffer_len == destination->buffer_len, "Allocator tried to copy data but sizes were mismatched.");

	memcpy(destination->data, source->data, destination->buffer_len);

	destination->current_offset = source->current_offset;
}
