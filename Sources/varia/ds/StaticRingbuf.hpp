#pragma once

#include "varia/Vcommon.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/ds/StaticArray.hpp"

#include <math.h>

template <typename T>
struct vds_ringbuf_t
{
	vds_array_t<T> _data;
	i64 _capacity;
	i64 _length;
	i64 _head;
};

template <typename T>
void vds_ringbuf_initialize(vds_ringbuf_t<T> * ring, vds_allocator_t * alloc, i64 count)
{
	vds_array_initialize(&ring->_data, alloc, count);
	ring->_head = 0;
	ring->_capacity = count;
	ring->_length = 0;
}

template <typename T>
void vds_ringbuf_reset(vds_ringbuf_t<T> * ring)
{
	ring->_head = 0;
	ring->_length = 0;
}

template <typename T>
i64 vds_ringbuf_next_head(vds_ringbuf_t<T> * ring)
{
	i64 cur_head = ring->_head;
	i64 cur_cap = ring->_capacity;

	cur_head = (cur_head + 1) % cur_cap;

	return cur_head;
}

template <typename T>
i64 vds_ringbuf_previous_head(vds_ringbuf_t<T> * ring)
{
	i64 cur_head = ring->_head;
	i64 cur_cap = ring->_capacity;

	cur_head = cur_head < 0 ? cur_cap : cur_head;
	--cur_head;

	return cur_head;
}

template <typename T>
bool vds_ringbuf_is_full(vds_ringbuf_t<T> * ring)
{
	return ring->_length >= ring->_capacity;
}

template <typename T>
void vds_ringbuf_push(vds_ringbuf_t<T> * ring, T value)
{
	ring->_data._data[ring->_head] = value;
	ring->_length += (1 * (ring->_length < ring->_capacity));
	ring->_head = vds_ringbuf_next_head(ring);
}

template <typename T>
T const * vds_ringbuf_pop(vds_ringbuf_t<T> * ring)
{
	if (ring->_length > 0)
	{
		ring->_head = vds_ringbuf_previous_head(ring);
		--(ring->_length);
		return &ring->_data._data[ring->_head];
	}
	else
	{
		return ring->_data._error_object;
	}
}

template <typename T, typename FUNC>
void vds_ringbuf_iterate(vds_ringbuf_t<T> * ring, FUNC f)
{
	i64 const len = ring->_length;
	i64 iter = ring->_head;
/*
            0           8/0          8/0...
			|------------|------------|---......
*/
	iter = ((ring->_capacity - ring->_length) + iter) % ring->_capacity;

	for (i64 i = 0; i < len; ++i)
	{
		T * element = &ring->_data._data[iter];
		f(element, iter);
		iter = (iter + 1) % ring->_capacity;
	}
}

template <typename T, typename FUNC>
void vds_ringbuf_pop_all(vds_ringbuf_t<T> * ring, FUNC f)
{
	i64 const len = ring->_length;
	i64 iter;

	for (i64 i = 0; i < len; ++i)
	{
		iter = vds_ringbuf_previous_head(ring);
		T * element = &ring->_data._data[iter];
		f(element);

		--ring->_length;
		ring->_head = iter;
	}
}