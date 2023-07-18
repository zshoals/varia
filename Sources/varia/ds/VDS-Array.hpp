#pragma once

#include "VDS-Types.hpp"
#include "VDS-Util.hpp"

template <typename T, Integer_64 SIZE>
struct VDS_Array
{
    T data[SIZE];
};

template <typename T, Integer_64 SIZE>
void vds_internal_array_bounds_check(VDS_Array<T, SIZE> const * arr, Integer_64 index)
{
    VDS_ASSERT(index >= 0 && index < SIZE, "Index out of range");
}

template <typename T, Integer_64 SIZE>
void vds_array_init(VDS_Array<T, SIZE> * arr)
{
    T element = {};
    for (Integer_64 i = 0; i < SIZE; i += 1)
    {
        arr->data[i] = element;
    }
}

template <typename T, Integer_64 SIZE>
void vds_array_set(VDS_Array<T, SIZE> * arr, Integer_64 index, T const & element)
{
    vds_internal_array_bounds_check(arr, index);

    arr->data[index] = element;
}

template <typename T, Integer_64 SIZE>
T * vds_array_address_of(VDS_Array<T, SIZE> * arr, Integer_64 index)
{
    vds_internal_array_bounds_check(arr, index);

    T * element = &(arr->data[index]);
    return element;
}

template <typename T, Integer_64 SIZE>
T const * vds_array_const_address_of(VDS_Array<T, SIZE> const * arr, Integer_64 index)
{
    vds_internal_array_bounds_check(arr, index);

    T const * const element = &(arr->data[index]);
    return element;
}

template <typename T, Integer_64 SIZE>
T vds_array_copy_of(VDS_Array<T, SIZE> const * arr, Integer_64 index)
{
    vds_internal_array_bounds_check(arr, index);

    return arr->data[index];
}

template <typename T, Integer_64 SIZE, typename FUNC>
void vds_array_iterate(VDS_Array<T, SIZE> * arr, FUNC f)
{
    for (Integer_64 i = 0; i < SIZE; i += 1)
    {
        T * const element = &(arr->data[i]);
        f(element);
    }
}

template <typename T, Integer_64 SIZE, typename FUNC>
void vds_array_iterate_range(VDS_Array<T, SIZE> * arr, Integer_64 start, Integer_64 end, FUNC f)
{
    //TODO(<zshoals> 07-18-2023): There might need to be another check here for...something that I don't see yet?
    VDS_ASSERT( (start >= 0) && (end <= SIZE), "Start or end index out of iterable range" );
    VDS_ASSERT( (start < end), "Start range exceeded end range" );

    for (Integer_64 i = START; i < END; i += 1)
    {
        T * const element = &(arr->data[i]);
        f(element);
    }
}