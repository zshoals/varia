#pragma once

#include "varia/ds/Allocator.hpp"
#include "varia/Validation.hpp"
#include "varia/Log.hpp"
#include "varia/ds/Option.hpp"

#include <stdint.h>
#include <string.h>

struct vds_string_map_key_t
{
	char string[63];
};

template <typename T> struct vds_string_map_t;

template <typename T>
void vds_string_map_initialize(vds_string_map_t<T> * map, vds_allocator_t * alloc, int8_t capacity)
{
	DEBUG_ENSURE(capacity > 0, "vds_string_map_t:> Tried to allocate an empty string map");
	DEBUG_ENSURE(capacity <= 128, "vds_string_map_t:> Tried to allocate too many string values (stringmap is for short datasets only.");
	map->_keys = (vds_string_map_key_t *)vds_allocator_allocate_aligned_count(alloc, sizeof(vds_string_map_key_t), capacity, 64);
	map->_values = (T *)vds_allocator_allocate_aligned_count(alloc, sizeof(T), capacity, 64);
	map->_slot_states = (bool *)vds_allocator_allocate_aligned_count(alloc, sizeof(int8_t), capacity, alignof(int8_t));
	map->_capacity = capacity;;

	for(int64_t i = 0; i < map->_capacity; ++i)
	{
		map->_slot_states[i] = false;
	}
}

template <typename T>
int8_t vds_string_map_key_exists(vds_string_map_t<T> * map, char const * string)
{
	for (int64_t i = 0; i < map->_capacity; ++i)
	{
		if (!map->_slot_states[i]) continue;

		int res = strcmp(string, &(map->_keys[i].string[0]));
		if (res == 0) { return (int8_t)i; }
	}

	return -1;
}

template <typename T>
int8_t vds_string_map_first_free(vds_string_map_t<T> * map)
{
	for (int64_t i = 0; i < map->_capacity; ++i)
	{
		if (!map->_slot_states[i])
		{
			return (int8_t)i;
		}
	}

	return -1;
}

template <typename T>
void vds_string_map_set(vds_string_map_t<T> * map, char const * str, T data)
{
	DEBUG_ENSURE(strlen(str) < 62, "vds_string_map_t:> String is too long for this string map, 63 characters minus null term");

	//TODO(zshoals 03-19-2023):> This silently fails if there's no room left in the container
	//Is that the right course of action?
	int8_t location = vds_string_map_key_exists(map, str);
	if (location != -1)
	{
		map->_values[location] = data;
	}
	else
	{
		int8_t first_free = vds_string_map_first_free(map);
		if (first_free != -1)
		{
			map->_slot_states[first_free] = true;
			strncpy(&(map->_keys[first_free].string[0]), str, 62);
			map->_keys[first_free].string[62] = '\0';
			map->_values[first_free] = data;
		}
	}
}

template <typename T>
void vds_string_map_remove(vds_string_map_t<T> * map, char const * str)
{
	int location = vds_string_map_key_exists(map, str);
	if (location != -1)
	{
		map->_slot_states[location] = false;
	}
	else
	{
		ENSURE_UNREACHABLE("vds_string_map_t:> Tried to remove a non-existent entry!");
	}
}

template <typename T>
vds_option_t<T *> vds_string_map_get(vds_string_map_t<T> * map, char const * str)
{
	int8_t location = vds_string_map_key_exists(map, str);

	if (location != -1)
	{
		return vds_option_create_some(&map->_values[location]);
	}
	else
	{
		return vds_option_create_none<T *>();
	}
}

template <typename T>
struct vds_string_map_t
{
	int8_t _capacity;
	bool * _slot_states;
	vds_string_map_key_t * _keys;
	T * _values;

	vds_option_t<T *> operator[](char const * str)
	{
		return vds_string_map_get(this, str);
	}
};
