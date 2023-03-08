#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>

template <typename T, int Size>
struct alignas(64) vds_dumbbuf_t 
{
	T buffer[Size];
	int64_t current_offset;
};

template <typename T, int Size>
void vds_dumbbuf_initialize(vds_dumbbuf_t<T, Size> * buf)
{
	memset(buf->buffer[0], 0, sizeof(*buf));
}


template <typename T, int Size>
int64_t vds_dumbbuf_reset(vds_dumbbuf_t<T, Size> * buf)
{
	buf->current_offset = 0;
}

template <typename T, int Size>
int64_t vds_dumbbuf_capacity(vds_dumbbuf_t<T, Size> * buf)
{
	return Size;
}

template <typename T, int Size>
int64_t vds_dumbbuf_length(vds_dumbbuf_t<T, Size> * buf)
{
	return buf->current_offset;
}

template <typename T, int Size>
T * vds_dumbbuf_data(vds_dumbbuf_t<T, Size> * buf)
{
	return &buf->buffer[0];
}

template <typename T, int Size>
T * vds_dumbbuf_data_at_offset(vds_dumbbuf_t<T, Size> * buf)
{
	return &buf->buffer[buf->current_offset];
}

template <typename T, int Size>
bool vds_dumbbuf_can_push_amount(vds_dumbbuf_t<T, Size> * buf, int64_t count)
{
	return (buf->current_offset + count) < Size;
}

//Attempts to push a value
//Nothing gets pushed if pushing would overrun the buffer.
template <typename T, int Size>
bool vds_dumbbuf_try_push(vds_dumbbuf_t<T, Size> * buf, T element)
{
	if (vds_dumbbuf_can_push_amount(buf, 1))
	{
		buf->buffer[buf->current_offset] = element;
		++buf->current_offset;

		return true;
	}

	return false;
}

//Verifies if --count-- values would fit inside the buffer.
//If --count-- values fit, executes lambda F, passing along the current location in the buffer
//Takes a dumbbuf, an element count, and a lambda that returns the actual amount of items written,
//which should be equal to or less than --count--.
//Returns true if the blit was successful, otherwise false.
template <typename T, typename FUNC, int Size>
bool vds_dumbbuf_try_blit(vds_dumbbuf_t<T, Size> * buf, int64_t count, FUNC f)
{
	if (vds_dumbbuf_can_push_amount(buf, count))
	{
		int64_t written = f(&buf->buffer[buf->current_offset]);

		assert(written <= count);

		buf->current_offset += written;

		return true;
	}

	return false;
}