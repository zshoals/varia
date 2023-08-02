#pragma once


#include "VDS-Types.hpp"
#include "VDS-Util.hpp"

#include <string.h>

template <Integer_64 SIZE>
struct VDS_Arena_Storage
{
    Integer_64 write_head;
    char data[SIZE];
};

struct VDS_Arena
{
    Integer_64 * write_head;
    char * data;
    Integer_64 capacity;
};

template <Integer_64 SIZE>
VDS_Arena vds_arena_make_interface(VDS_Arena_Storage<SIZE> * arena)
{
    VDS_Arena interface;
    {
        interface.write_head = &(arena->write_head);
        interface.data = &(arena->data[0]);
        interface.capacity = SIZE;
    }

    return interface;
}

static inline void * vds_arena_allocate(VDS_Arena * arena, Integer_64 requested_size)
{
    Integer_64 head_address = (Integer_64)address_of(arena->data[*(arena->write_head)]);
    //NOTE(<zshoals> 08-02-2023): This overallocates by 64 if the address is already
    //  a multiple of 64, but who cares I guess
    Integer_64 offset_from_64 = 64 - (head_address % 64);

    Integer_64 aligned_head = *(arena->write_head) + offset_from_64;
    //NOTE(<zshoals> 08-01-2023): Overshoot the allocation by 4 so the next allocation is forced to be
    //  on an unused memory position and we have some null terms so we can read this stuff as a string safely
    Integer_64 end_head = aligned_head + requested_size + 4;

    Boolean allocation_in_range = end_head < arena->capacity;

    VDS_ASSERT(allocation_in_range, "Arena overrun");

    if (allocation_in_range)
    {
        void * raw_memory = static_cast<void *>(address_of(arena->data[aligned_head]));
        *(arena->write_head) = end_head;

        return raw_memory;
    }
    else
    {
        return nullptr;
    }

};

static inline void vds_arena_clear(VDS_Arena * arena)
{
    memset(&(arena->data[0]), 0, arena->capacity);
    *(arena->write_head) = 0;
}