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
struct VDS_Stringmap_Storage
{
    VDS_Array_Storage<T, SIZE> data;
    T stub;

    VDS_Stringmap_Storage()
    {
        memset(&(this->stub), 0, sizeof(this->stub));
        stub.name = "STRINGMAP_ERROR: NON-EXISTENT ENTRY; THIS IS A STUB.";
    }
};

template <typename T>
struct VDS_Stringmap
{
    VDS_Array data;
    T * stub;
};

template <typename T, Integer_64 SIZE>
VDS_Stringmap<T> vds_stringmap_make_interface(VDS_Stringmap_Storage<T, SIZE> * map)
{
    VDS_Stringmap<T> interface;
    {
        interface.data = vds_array_make_interface(&(map->data));
        interface.stub = &(map->stub);
    }

    return interface;
}

template <typename T>
VDS_Stringmap_Key vds_internal_stringmap_search(VDS_Array<T> * storage, char const * string)
{
    VDS_Stringmap_Key key = vds_array_index_of(storage, [&string](T const * element)
    {
        return strncmp(element->name, string, 10000);
    });

    return key;
}

template <typename T>
void vds_stringmap_assign(VDS_Stringmap<T> * map, T element)
{
    VDS_Array<T> * storage = &(map->data);
    VDS_Stringmap_Key key = vds_internal_stringmap_search(storage, element->name);

    if (key.key == -1)
    {
        if (vds_array_can_push(storage))
        {
            vds_array_push(storage, element);
        }
    }
    else
    {
        T * found_item = vds_array_location_of(storage, key.key);
        found_item = element;
    }
}

template <typename T>
T * vds_stringmap_retrieve(VDS_Stringmap<T> * map, char const * string)
{
    VDS_Array<T> * storage = &(map->data);
    VDS_Stringmap_Key key = vds_internal_stringmap_search(storage, string);

    if (key.key == -1)
    {
        return map->stub;
    }
    else
    {
        return vds_array_location_of(storage, key.key);
    }
}

template <typename T>
void vds_stringmap_clear(VDS_Stringmap<T> * map)
{
    VDS_Array<T> * storage = &(map->data);
    vds_array_clear(storage);
}