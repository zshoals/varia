#pragma once

#include "VDS-Types.hpp"
#include "VDS-Util.hpp"

template <typename T, Integer_64 SIZE>
struct VDS_Array
{
    Integer_64 push_head;
    T data[SIZE];
};

template <typename T, Integer_64 SIZE>
T vds_array_copy_of(VDS_Array<T, SIZE> const * array, Integer_64 index)
{
    //TODO(<zshoals> 08-06-2023): Where's the bounds checking?
    return array->data[index];
}

template <typename T, Integer_64 SIZE>
T * vds_array_location_of(VDS_Array<T, SIZE> * array, Integer_64 index)
{
    //TODO(<zshoals> 08-06-2023): Where's the bounds checking?
    return &(array->data[index]);
}

template <typename T, Integer_64 SIZE>
T const * vds_array_const_location_of(VDS_Array<T, SIZE> const * array, Integer_64 index)
{
    //TODO(<zshoals> 08-06-2023): Where's the bounds checking?
    return &(array->data[index]);
}

template <typename T, Integer_64 SIZE>
Boolean vds_array_can_push(VDS_Array<T, SIZE> const * array)
{
    return ( array->push_head < SIZE );
}

template <typename T, Integer_64 SIZE>
void vds_array_push(VDS_Array<T, SIZE> * array, T element)
{
    VDS_ASSERT( array->push_head + 1 <= SIZE, "Push would overrun array");

    array->data[ array->push_head ] = element;
    array->push_head += 1;
}

template <typename T, Integer_64 SIZE>
T * vds_array_construct_push(VDS_Array<T, SIZE> * array)
{
    VDS_ASSERT( array->push_head + 1 <= SIZE, "Push would overrun array");

    T * element = vds_array_location_of(array, array->push_head);
    array->push_head += 1;

    return element;
}

template <typename T, Integer_64 SIZE>
Boolean vds_array_can_pop(VDS_Array<T, SIZE> const * array)
{
    return ( array->push_head > 0);
}

template <typename T, Integer_64 SIZE>
void vds_array_pop(VDS_Array<T, SIZE> * array)
{
    VDS_ASSERT( array->push_head - 1 >= 0, "Pop would underrun array");

    array->push_head -= 1;
}

template <typename T, Integer_64 SIZE>
void vds_array_clear(VDS_Array<T, SIZE> * array)
{
    array->push_head = 0;
}

template <typename T, Integer_64 SIZE, typename FUNC>
Integer_64 vds_array_index_of(VDS_Array<T, SIZE> * array, FUNC search_function)
{
    //NOTE(<zshoals> 07-31-2023): Push head is max, NOT capacity
    //  good or bad call?
    for (Integer_64 i = 0, end = array->push_head; i < end; i += 1)
    {
        T const * element = &(array->data[i]);
        if ( search_function(element) )
        {
            return i;
        }
    }

    return -1;
}

template <typename T, Integer_64 SIZE, typename FUNC>
void vds_array_iterate(VDS_Array<T, SIZE> * array, FUNC f)
{
    //NOTE(<zshoals> 07-31-2023): Push head is max, NOT capacity
    //  good or bad call?
    for (Integer_64 i = 0; i < array->push_head; i += 1)
    {
        T * element = vds_array_location_of(array, i);
        f(element, i);
    }
}
