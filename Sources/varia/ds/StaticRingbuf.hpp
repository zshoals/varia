#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include "kinc/log.h"

namespace vds
{

template<typename T, int Size>
struct StaticRingbuf
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

	void initialize(void)
	{
		VARIA_ZERO_INIT(this);
	}

	StaticRingbufIterator begin(void)
	{
		return StaticRingbufIterator(&this->data[0], rear, count);
	} 

	StaticRingbufIterator end(void) 
	{
		//Note(zshoals): Only the count matters in this instance
		return StaticRingbufIterator(&this->data[rear], rear, 0);
	}

	void push_back(T value)
	{
		size_t target_idx = (count + rear) % Size;
		data[target_idx] = value;

		DEBUG_ENSURE_UINT_LTE(count, Size, "Ringbuf design error; count exceeded Size, which shouldn't occur.");
		bool overfilled = (count >= Size);
		rear = (rear + overfilled) % Size;
		count += overfilled ^ (1ULL << 0); //Invert overfilled;
	}

	T pop_back(void)
	{
		ENSURE_UINT_GTE(count, 1, "Attempted element removal of empty StaticRingbuf.");

		--count;
		size_t target_idx = (count + rear) % Size;
		return data[target_idx];
	}

	T pop_back_unsafe(void)
	{
		DEBUG_ENSURE_UINT_GTE(count, 1, "Attempted element removal of empty StaticRingbuf.");

		--count;
		size_t target_idx = (count + rear) % Size;
		return data[target_idx];
	}

	T pop_front(void)
	{
		ENSURE_UINT_GTE(count, 1, "Attempted element removal of empty StaticRingbuf.");

		size_t target_idx = rear % Size;
		--count;
		++rear;

		return data[target_idx];
	}

	T pop_front_unsafe(void)
	{
		DEBUG_ENSURE_UINT_GTE(count, 1, "Attempted element removal of empty StaticRingbuf.");

		size_t target_idx = rear % Size;
		--count;
		++rear;

		return data[target_idx];
	}

	void clear(void)
	{
		this->rear = 0;
		this->count = 0;
	}

	bool is_populated(void)
	{
		return (count > 0);
	}

	bool is_empty(void)
	{
		return !is_populated();
	}

	bool is_full(void)
	{
		DEBUG_ENSURE_UINT_LTE(count, Size, "Ringbuf design error; count exceeded Size, which shouldn't occur.");
		return (count >= Size);
	}

	bool is_not_full(void)
	{
		return !this->is_full();
	}

};

}