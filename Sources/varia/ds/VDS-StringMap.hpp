#pragma once

#include "VDS-Array.hpp"
#include <string.h>

//TODO(<zshoals> 08-03-2023): This isn't really a map; just an abstraction over
//  an array find

struct VDS_Stringmap_Key
{
    Integer_64 key;
};

//NOTE(<zshoals> 08-03-2023): We assume anything using stringmap has a char const * field called
//"name"
template <typename T, Integer_64 SIZE>
struct VDS_Stringmap
{
    VDS_Array<char const *, SIZE> keys;
    VDS_Array<T, SIZE> data;
    T stub;
};

template <Integer_64 SIZE>
VDS_Stringmap_Key vds_internal_stringmap_search(VDS_Array<char const *, SIZE> * storage, char const * search_key)
{
    VDS_Stringmap_Key key;
    key.key = vds_array_index_of(storage, [&search_key](char const * const * stored_key)
    {
        return ( strncmp(*(stored_key), search_key, 10000) == 0 );
    });

    return key;
}

template <typename T, Integer_64 SIZE>
void vds_stringmap_assign(VDS_Stringmap<T, SIZE> * map, char const * name, T element)
{
    VDS_Array<T, SIZE> * data_storage = &(map->data);
    VDS_Array<char const *, SIZE> * key_storage = &(map->keys);
    VDS_Stringmap_Key key = vds_internal_stringmap_search(key_storage, name);

    if (key.key == -1)
    {
        if (vds_array_can_push(key_storage))
        {
            vds_array_push(key_storage, name);
            vds_array_push(data_storage, element);
        }
        else
        {
            VDS_ASSERT(0, "Stringmap depleted available slots.");
        }
    }
    else
    {
        T * found_item = vds_array_location_of(data_storage, key.key);
        *(found_item) = element;
    }
}

template <typename T, Integer_64 SIZE>
T * vds_stringmap_construct_assign(VDS_Stringmap<T, SIZE> * map, char const * name)
{
    VDS_Array<T, SIZE> * data_storage = &(map->data);
    VDS_Array<char const *, SIZE> * key_storage = &(map->keys);

    if (vds_array_can_push(key_storage))
    {
        vds_array_push(key_storage, name);
        T * out = vds_array_construct_push(data_storage);

        return out;
    }
    else
    {
        VDS_ASSERT(0, "Stringmap depleted available slots.");
        return &(map->stub);
    }
}

template <typename T, Integer_64 SIZE>
T * vds_stringmap_retrieve(VDS_Stringmap<T, SIZE> * map, char const * string)
{
    VDS_Array<T, SIZE> * data_storage = &(map->data);
    VDS_Array<char const *, SIZE> * key_storage = &(map->keys);
    VDS_Stringmap_Key key = vds_internal_stringmap_search(key_storage, string);

    if (key.key == -1)
    {
        return &(map->stub);
    }
    else
    {
        return vds_array_location_of(data_storage, key.key);
    }
}

template <typename T, Integer_64 SIZE>
void vds_stringmap_clear(VDS_Stringmap<T, SIZE> * map)
{
    VDS_Array<char const *, SIZE> * key_storage = &(map->keys);
    VDS_Array<T, SIZE> * data_storage = &(map->data);
    vds_array_clear(key_storage);
    vds_array_clear(data_storage);
}