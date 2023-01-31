#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include "kinc/log.h"

template<typename T, int Size>
struct vds_ringbuf_t
{

//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
//        Iterator
//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	struct StaticRingbufIterator
	{
		private:
			T * base_ptr;
			size_t rear;
			size_t count;

		public:
			StaticRingbufIterator(T * ptr, size_t rear, size_t count) : base_ptr{ptr}, rear{rear}, count{count} {}
			StaticRingbufIterator operator++() 
			{ 
				++rear;
				rear %= Size;
				--count;

				return StaticRingbufIterator(base_ptr, rear, count);
			}
			bool operator!=(StaticRingbufIterator const & other) const { return this->count != 0; }
			T & operator*() const { return this->base_ptr[rear]; }
	};
//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
//        End Iterator
//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

	T data[Size];
	size_t rear;
	size_t count;

	// StaticRingbuf(void)
	// {
	// 	this->initialize();
	// }

	StaticRingbufIterator begin(void)
	{
		return StaticRingbufIterator(&this->data[0], rear, count);
	} 

	StaticRingbufIterator end(void) 
	{
		//Note(zshoals): Only the count matters in this instance
		return StaticRingbufIterator(&this->data[rear], rear, 0);
	}
};





template<typename T, int Size>
void vds_ringbuf_initialize(vds_ringbuf_t<T, Size> * self)
{
	VARIA_ZERO_INIT(self);
}

template<typename T, int Size>
void vds_ringbuf_push_back(vds_ringbuf_t<T, Size> * self, T value)
{
	size_t target_idx = (self->count + self->rear) % Size;
	self->data[target_idx] = value;

	DEBUG_ENSURE_UINT_LTE(self->count, Size, "Ringbuf design error; count exceeded Size, which shouldn't occur.");
	bool overfilled = (self->count >= Size);
	self->rear = (self->rear + overfilled) % Size;
	self->count += overfilled ^ (1ULL << 0); //Invert overfilled;
}

template<typename T, int Size>
T vds_ringbuf_pop_back(vds_ringbuf_t<T, Size> * self)
{
	ENSURE_UINT_GTE(self->count, 1, "Attempted element removal of empty StaticRingbuf.");

	--self->count;
	size_t target_idx = (self->count + self->rear) % Size;
	return self->data[target_idx];
}

template<typename T, int Size>
T vds_ringbuf_pop_back_unsafe(vds_ringbuf_t<T, Size> * self)
{
	DEBUG_ENSURE_UINT_GTE(self->count, 1, "Attempted element removal of empty StaticRingbuf.");

	--self->count;
	size_t target_idx = (self->count + self->rear) % Size;
	return self->data[target_idx];
}

template<typename T, int Size>
T vds_ringbuf_pop_front(vds_ringbuf_t<T, Size> * self)
{
	ENSURE_UINT_GTE(self->count, 1, "Attempted element removal of empty StaticRingbuf.");

	size_t target_idx = self->rear % Size;
	--self->count;
	++self->rear;

	return self->data[target_idx];
}

template<typename T, int Size>
T vds_ringbuf_pop_front_unsafe(vds_ringbuf_t<T, Size> * self)
{
	DEBUG_ENSURE_UINT_GTE(self->count, 1, "Attempted element removal of empty StaticRingbuf.");

	size_t target_idx = self->rear % Size;
	--self->count;
	++self->rear;

	return self->data[target_idx];
}

template<typename T, int Size>
void vds_ringbuf_clear(vds_ringbuf_t<T, Size> * self)
{
	self->rear = 0;
	self->count = 0;
}

template<typename T, int Size>
bool vds_ringbuf_is_populated(vds_ringbuf_t<T, Size> * self)
{
	return (self->count > 0);
}

template<typename T, int Size>
bool vds_ringbuf_is_empty(vds_ringbuf_t<T, Size> * self)
{
	return !vds_ringbuf_is_populated(self);
}

template<typename T, int Size>
bool vds_ringbuf_is_full(vds_ringbuf_t<T, Size> * self)
{
	DEBUG_ENSURE_UINT_LTE(self->count, Size, "Ringbuf design error; count exceeded Size, which shouldn't occur.");
	return (self->count >= Size);
}

template<typename T, int Size>
bool vds_ringbuf_is_almost_full(vds_ringbuf_t<T, Size> * self)
{
	return (self->count + 1 == Size);
}

template<typename T, int Size>
bool vds_ringbuf_is_not_full(vds_ringbuf_t<T, Size> * self)
{
	return !vds_ringbuf_is_full(self);
}