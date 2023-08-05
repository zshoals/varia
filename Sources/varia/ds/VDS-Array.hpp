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

    return interface;
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
T * vds_array_construct_push(VDS_Array<T> * array)
{
    VDS_ASSERT( *(array->push_head) + 1 <= array->capacity, "Push would overrun array");

    T * element = vds_array_location_of(array, *(array->push_head));
    *(array->push_head) += 1;

    return element;
}

template <typename T>
Boolean vds_array_can_pop(VDS_Array<T> const * array)
{
    return ( *(array->push_head) > 0);
}

template <typename T>
void vds_array_pop(VDS_Array<T> * array)
{
    VDS_ASSERT( *(array->push_head) - 1 >= 0, "Pop would underrun array");

    *(array->push_head) -= 1;
}

template <typename T>
void vds_array_clear(VDS_Array<T> * array)
{
    *(array->push_head) = 0;
}

template <typename T, typename FUNC>
Integer_64 vds_array_index_of(VDS_Array<T> * array, FUNC search_function)
{
    //NOTE(<zshoals> 07-31-2023): Push head is max, NOT capacity
    //  good or bad call?
    for (Integer_64 i = 0, end = *(array->push_head); i < end; i += 1)
    {
        T const * element = &(array->data[i]);
        if ( search_function(element) )
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
