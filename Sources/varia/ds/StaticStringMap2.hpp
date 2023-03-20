#pragma once

#include "varia/ds/StrHash.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/ds/Option.hpp"

#include <math.h>
#include <string.h>
#include <stdint.h>


struct vds_string_key_t
{
	char const * string;
};

template <typename T> struct vds_string_map_t;

//Note(zshoals 03-19-2023):> Warning: Only works with string literals, also no removals
template <typename T>
void vds_string_map_initialize(vds_string_map_t<T> * map, vds_allocator_t * alloc, int64_t capacity)
{
	map->_slot_status = (bool *)vds_allocator_allocate_aligned_count(alloc, sizeof(bool), capacity, alignof(bool));
	map->_keys = (vds_string_key_t *)vds_allocator_allocate_aligned_count(alloc, sizeof(vds_string_key_t), capacity, alignof(vds_string_key_t));
	map->_data = (T *)vds_allocator_allocate_aligned_count(alloc, sizeof(T), capacity, 64);
	map->_empty_string = "";
	map->_capacity = capacity;

	for (int64_t i = 0; i < map->_capacity; ++i)
	{
		map->_slot_status[i] = false;
		map->_keys[i].string = map->_empty_string;
	}
}

static constexpr int64_t vds_string_map_get_hashkey(char const * key, int64_t capacity)
{
	int64_t hashvalue = cstr_hash(key) & (~0x80000000);
	return hashvalue % capacity;
}

template <typename T>
int64_t vds_string_map_element_exists_at(vds_string_map_t<T> * map, char const * key, int64_t idx)
{

	int64_t count = map->_capacity - 1;

	//Note(zshoals 03-19-2023):> Find the appropriate element, verifying it's what we're really looking for
	//by linear searching from the target point until an empty block or we find the thing
	do 
	{
		if (!map->_slot_status[idx]) { return -1; }
		if (strcmp(key, map->_keys[idx].string) == 0) { return idx; }

		idx = (idx + 1) % map->_capacity;
		--count;
	} while (count != 0);

	return -1;
}

template <typename T>
void vds_string_map_set(vds_string_map_t<T> * map, char const * key, T value)
{
	DEBUG_ENSURE(strlen(key) > 0, "vds_string_map_t:> Must provide a string with substance.");

	int64_t hashkey = vds_string_map_get_hashkey(key, map->_capacity);
	int64_t elem_idx = vds_string_map_element_exists_at(map, key, hashkey);

	if (elem_idx != -1)
	{
		map->_data[elem_idx] = value;
	}
	else
	{
		int64_t end_idx = hashkey - 1;
		end_idx = (end_idx < 0) ? (map->_capacity - 1) : end_idx;

		while (hashkey != end_idx)
		{
			if (!map->_slot_status[hashkey])
			{
				map->_slot_status[hashkey] = true;
				map->_data[hashkey] = value;
				map->_keys[hashkey].string = key;

				return;
			}
			else
			{
				hashkey = (hashkey + 1) % map->_capacity;
			}
		}
	}

	//Note(zshoals 03-19-2023):> The map was full, and we couldn't add the item.
}

template <typename T>
vds_option_t<T *> vds_string_map_get(vds_string_map_t<T> * map, char const * key)
{
	DEBUG_ENSURE(strlen(key) > 0, "vds_string_map_t:> Must provide a string with substance.");

	int64_t hashkey = vds_string_map_get_hashkey(key, map->_capacity);
	int64_t elem_idx = vds_string_map_element_exists_at(map, key, hashkey);

	if (elem_idx != -1)
	{
		return vds_option_create_some(&map->_data[elem_idx]);
	}
	else
	{
		return vds_option_create_none<T *>();
	}
}

template <typename T>
struct vds_string_map_t
{
	bool * _slot_status; //Only good for finding an empty slot?
	vds_string_key_t * _keys;
	T * _data;
	int64_t _capacity;
	char const * _empty_string;

	vds_option_t<T *> operator[](char const * key)
	{
		return vds_string_map_get(this, key);
	}
};
