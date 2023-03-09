#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "varia/ds/Allocator.hpp"

template <typename T>
struct vds_dumbbuf_t 
{
	T * buffer;
	int64_t _capacity;
	int64_t current_offset;
};

template <typename T>
void vds_dumbbuf_initialize(vds_dumbbuf_t<T> * buf, vds_allocator_t * alloc, size_t count)
{
	buf->_capacity = count;
	buf->current_offset = 0;
	buf->buffer = vds_allocator_malloc_aligned(alloc, T, count, 64);
	memset(&buf->buffer[0], 0, sizeof(T) * count)
}

template <typename T>
void vds_dumbbuf_initialize_direct(vds_dumbbuf_t<T> * buf, T * buffer, size_t count)
{
	buf->_capacity = count;
	buf->current_offset = 0;
	buf->buffer = buffer;
	memset(&buf->buffer[0], 0, sizeof(T) * count);
}

template <typename T>
void vds_dumbbuf_reset(vds_dumbbuf_t<T> * buf)
{
	buf->current_offset = 0;
	memset(buf->buffer, 0x69, sizeof(T) * buf->_capacity);
}

template <typename T>
int64_t vds_dumbbuf_capacity(vds_dumbbuf_t<T> * buf)
{
	return buf->_capacity;
}

template <typename T>
void vds_dumbbuf_advance(vds_dumbbuf_t<T> * buf, int64_t count)
{
	buf->current_offset += count;
}

template <typename T>
int64_t vds_dumbbuf_length(vds_dumbbuf_t<T> * buf)
{
	return buf->current_offset;
}

template <typename T>
T * vds_dumbbuf_data(vds_dumbbuf_t<T> * buf)
{
	return &buf->buffer[0];
}

template <typename T>
T * vds_dumbbuf_data_at_offset(vds_dumbbuf_t<T> * buf)
{
	assert(buf->current_offset < buf->_capacity);
	return &buf->buffer[buf->current_offset];
}

template <typename T>
bool vds_dumbbuf_can_push_amount(vds_dumbbuf_t<T> * buf, int64_t count)
{
	return (buf->current_offset + count) < buf->_capacity;
}

//Attempts to push a value
//Nothing gets pushed if pushing would overrun the buffer.
template <typename T>
bool vds_dumbbuf_try_push(vds_dumbbuf_t<T> * buf, T element)
{
	if (vds_dumbbuf_can_push_amount(buf, 1))
	{
		buf->buffer[buf->current_offset] = element;
		++buf->current_offset;

		return true;
	}

	return false;
}
