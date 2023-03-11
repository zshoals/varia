#pragma once

#include "varia/Vcommon.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/ds/StaticArray.hpp"

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
	ring->head = 0;
	ring->_capacity = count;
	ring->_length = 0;
}

template <typename T>
void vds_ringbuf_next_head(vds_ringbuf_t<T> * ring)
{
	ring->_head = ( (ring->_head + 1) % ring->_capacity ); 
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
	ring->_length + (1 * (ring->_length < ring->_capacity));
	vds_ringbuf_next_head(ring);
}

template <typename T>
T const * vds_ringbuf_pop(vds_ringbuf_t<T> * ring)
{
	//TODO(zshoals 03-11-2023):> FIXME
}