#pragma once

#include <stdint.h>
#include <string.h>

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

//Attempts to push --count-- values
//Nothing gets pushed if pushing would overrun the buffer.
template <typename T, int Size>
bool vds_dumbbuf_safe_blit(vds_dumbbuf_t<T, Size> * buf, T * elements, int64_t count)
{
	if (vds_dumbbuf_can_push_amount(count))
	{
		memcpy(buf->buffer[buf->current_offset], elements, sizeof(T) * count);
		buf->current_offset += count;

		return true;
	}

	return false;
}
