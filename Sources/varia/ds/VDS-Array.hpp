#pragma once

#include "VDS-Types.hpp"
#include "VDS-Util.hpp"

template <typename T, Integer_64 SIZE>
struct VDS_Array_Storage
{
    Integer_64 push_head;
    T data[SIZE];
};

template <typename T>
struct VDS_Array
{
    Integer_64 * push_head;
    Integer_64 capacity;
    T * data;
};

template <typename T, Integer_64 SIZE>
VDS_Array<T> vds_array_make_interface(VDS_Array_Storage<T, SIZE> * storage)
{
    VDS_Array<T> interface;
    {
        interface.push_head = &(storage->push_head);
        interface.capacity = SIZE;
        interface.data = &(storage->data[0]);
    }
}

template <typename T>
Boolean vds_array_can_push(VDS_Array<T> const * array)
{
    return ( *(array->push_head) < array->capacity );
}

template <typename T>
void vds_array_push(VDS_Array<T> * array, T element)
{
    VDS_ASSERT( *(array->push_head) + 1 <= array->capacity, "Push would overrun array");

    array->data[ *(array->push_head) ] = element;
    *(array->push_head) += 1;
}

template <typename T>
void vds_array_can_pop(VDS_Array<T> const * array)
{
    return ( *(array->push_head) > 0);
}

template <typename T>
T vds_array_pop(VDS_Array<T> * array)
{
    VDS_ASSERT( *(array->push_head) - 1 >= 0, "Pop would underrun array");

    *(array->push_head) -= 1;
    return *(array->data[ *(array->push_head) ]);
}

template <typename T>
T vds_array_copy_of(VDS_Array<T> const * array, Integer_64 index)
{
    return array->data[index];
}

template <typename T>
T * vds_array_location_of(VDS_Array<T> * array, Integer_64 index)
{
    return &(array->data[index]);
}

template <typename T>
T const * vds_array_const_location_of(VDS_Array<T> const * array, Integer_64 index)
{
    return &(array->data[index]);
}

template <typename T, typename FUNC>
Integer_64 vds_array_index_of(VDS_Array<T> * array, FUNC search_function)
{
    //NOTE(<zshoals> 07-31-2023): Push head is max, NOT capacity
    //  good or bad call?
    for (Integer_64 i = 0, end = *(array->push_head); i < end; i += 1)
    {
        if ( search_function( &(array->data[i]) ) )
        {
            return i;
        }
    }

    return -1;
}

template <typename T, typename FUNC>
void vds_array_iterate(VDS_Array<T> * array, FUNC f)
{
    //NOTE(<zshoals> 07-31-2023): Push head is max, NOT capacity
    //  good or bad call?
    for (Integer_64 i = 0; i < *(array->push_head); i += 1)
    {
        T * element = vds_array_location_of(array, i);
        f(element, i);
    }
}

































































































// template <typename T, Integer_64 SIZE>
// void vds_internal_array_bounds_check(VDS_Array<T, SIZE> const * arr, Integer_64 index)
// {
//     VDS_ASSERT(index >= 0 && index < SIZE, "Index out of range");
// }

// //NOTE(<zshoals> 07-18-2023): Force this value to be comptime
// #define vds_array_capacity(ARRPTR) VDS_COUNT_OF(ARRPTR)

// template <typename T, Integer_64 SIZE>
// void vds_array_init(VDS_Array<T, SIZE> * arr)
// {
//     T element = {};
//     for (Integer_64 i = 0; i < SIZE; i += 1)
//     {
//         arr->data[i] = element;
//     }
// }

// template <typename T, Integer_64 SIZE>
// void vds_array_set(VDS_Array<T, SIZE> * arr, Integer_64 index, T const & element)
// {
//     vds_internal_array_bounds_check(arr, index);

//     arr->data[index] = element;
// }

// template <typename T, Integer_64 SIZE>
// T * vds_array_address_of(VDS_Array<T, SIZE> * arr, Integer_64 index)
// {
//     vds_internal_array_bounds_check(arr, index);

//     T * element = &(arr->data[index]);
//     return element;
// }

// template <typename T, Integer_64 SIZE>
// T const * vds_array_const_address_of(VDS_Array<T, SIZE> const * arr, Integer_64 index)
// {
//     vds_internal_array_bounds_check(arr, index);

//     T const * const element = &(arr->data[index]);
//     return element;
// }

// template <typename T, Integer_64 SIZE>
// T vds_array_copy_of(VDS_Array<T, SIZE> const * arr, Integer_64 index)
// {
//     vds_internal_array_bounds_check(arr, index);

//     return arr->data[index];
// }

// template <typename T, Integer_64 SIZE, typename FUNC>
// void vds_array_iterate(VDS_Array<T, SIZE> * arr, FUNC f)
// {
//     for (Integer_64 i = 0; i < SIZE; i += 1)
//     {
//         T * const element = &(arr->data[i]);
//         f(element);
//     }
// }

// template <typename T, Integer_64 SIZE, typename FUNC>
// void vds_array_iterate_range(VDS_Array<T, SIZE> * arr, Integer_64 start, Integer_64 end, FUNC f)
// {
//     //TODO(<zshoals> 07-18-2023): There might need to be another check here for...something that I don't see yet?
//     VDS_ASSERT( (start >= 0) && (end <= SIZE), "Start or end index out of iterable range" );
//     VDS_ASSERT( (start < end), "Start range exceeded end range" );

//     for (Integer_64 i = START; i < END; i += 1)
//     {
//         T * const element = &(arr->data[i]);
//         f(element);
//     }
// }
