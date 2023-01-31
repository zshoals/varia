#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"

#include "StaticRingbuf.hpp"
#include "Result.hpp"




template<typename T, int Size>
struct vds_queue_t //Note(zshoals 01-31-2023):> Hey QT, winkyface ;)
{
	vds_ringbuf_t<T, Size> ring;
};





template<typename T, int Size>
void vds_queue_initialize(vds_queue_t<T, Size> * self)
{
	VARIA_ZERO_INIT(self);
}

//Tries to push a new value onto the queue. 
//If there was room to add the value, returns true, otherwise false
template<typename T, int Size>
bool vds_queue_enqueue(vds_queue_t<T, Size> * self, T value)
{
	bool has_room = vds_ringbuf_is_not_full(&self->ring);

	if (has_room)
	{
		vds_ringbuf_push_back(&self->ring, value);
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T, int Size>
vds_result_t<T> vds_queue_dequeue(vds_queue_t<T, Size> * self)
{
	vds_result_t<T> result;
	bool has_elements = vds_ringbuf_is_populated(&self->ring);

	if (has_elements)
	{
		result.valid = VDS_RESULT_STATUS_SUCCESS_E;
		result.value = vds_ringbuf_pop_front(&self->ring);

		return result;
	}
	else
	{
		result.valid = VDS_RESULT_STATUS_FAILURE_E;

		return result;
	}
}
