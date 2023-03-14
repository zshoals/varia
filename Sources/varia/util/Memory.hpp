#pragma once

#include "varia/Vcommon.hpp"
#include "varia/ds/Allocator.hpp"
#include <string.h>

constexpr size_t varia_memory_kilobytes_to_bytes(size_t KB) { return KB * 1000; }
constexpr size_t varia_memory_megabytes_to_bytes(size_t MB) { return MB * 1000 * 1000; }
constexpr size_t varia_memory_gigabytes_to_bytes(size_t GB) { return GB * 1000 * 1000 * 1000; }

constexpr size_t VARIA_MEMORY_CONSTANTS_DEFAULT_ALIGNMENT = 16;

#define varia_bitcast(DESTINATION_PTR, SOURCE_PTR, SIZE)\
	memcpy((DESTINATION_PTR), (SOURCE_PTR), (SIZE))
	

vds_allocator_t * varia_memory_get_permanent_allocator(void);
vds_allocator_t * varia_memory_get_scratch_allocator(void);
vds_allocator_t * varia_memory_get_image_allocator(void);
void varia_memory_initialize_allocators(i64 permanent_size_bytes, i64 scratch_size_bytes, i64 image_size_bytes);
void varia_memory_reset_scratch_allocator(void);
void varia_memory_reset_image_allocator(void);