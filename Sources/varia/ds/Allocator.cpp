#include "Allocator.hpp"
#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include <inttypes.h>
#include <string.h>

vds::Allocator::Allocator(void * memory, size_t buffer_size) : 
	data{memory}, buffer_len{buffer_size}, current_offset{0}{}

void * vds::Allocator::allocate_aligned(size_t size, size_t alignment)
{
	DEBUG_ENSURE_UINT_GT_ZERO(size, "Tried to make an empty memory allocation.");

	uintptr_t base_addr = reinterpret_cast<uintptr_t>(data);  
	uintptr_t misalign = base_addr % alignment;
	uintptr_t target_address = base_addr + (alignment - misalign);
	size_t offset_advance_amount = misalign + size;

	DEBUG_ENSURE_UINT_LT(this->current_offset + offset_advance_amount, this->buffer_len, "Allocator OOM");

	this->current_offset += offset_advance_amount;

	return reinterpret_cast<void *>(target_address);
}

void * vds::Allocator::allocate_aligned_count(size_t elem_size, size_t count, size_t alignment)
{
	return allocate_aligned(elem_size * count, alignment);
}

void vds::Allocator::free_all(void)
{
	current_offset = 0;
}

void vds::Allocator::zero(void)
{
	memset(this->data, 0, this->buffer_len);
}