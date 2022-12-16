#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include "varia/ds/qsort.h"
#include "varia/ds/SearchResult.hpp"

#include <string.h>

namespace vds
{

template <typename T, int Size>
struct StaticArray
{

//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
//        Iterator
//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

	struct StaticArrayIterator
	{
		private: 
			T * ptr;

		public: 
			StaticArrayIterator(T * ptr) { this->ptr = ptr; }
			StaticArrayIterator operator++() { ++this->ptr; return *this; }
			bool operator!=(StaticArrayIterator const & other) const { return this->ptr != other.ptr; }
			T & operator*() const { return *this->ptr; }
			// T & operator*() { return *this->ptr; }

	};
//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
//        End Iterator
//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV


	T data[Size];
	size_t push_idx;

	StaticArray(void)
	{
		VARIA_ZERO_INIT(this);
	}

	//TODO(zshoals): Figure out what const after the function parameters means, this doesn't with them present however
	//examples seem to use the const version without any issues
	StaticArrayIterator begin(void) /*const*/ { return StaticArrayIterator( &this->data[0] ); }
	StaticArrayIterator end(void) /*const*/ { return StaticArrayIterator( &this->data[this->push_idx] ); }

	void push(T value)
	{
		DEBUG_ENSURE_UINT_LT(push_idx, Size, "Attempted element push of full StaticArray");

		this->data[this->push_idx] = value;
		++this->push_idx;
	}

	T pop(void)
	{
		ENSURE_UINT_GTE(push_idx, 1, "Attempted element pop of empty StaticArray.");

		--this->push_idx;
		return this->data[this->push_idx];
	}

	void set(size_t index, T value)
	{
		ENSURE_UINT_LT(index, Size, "Attempted to set an element out of range in StaticArray.");
		this->data[index] = value;
	}

	void set_unsafe(size_t index, T value)
	{
		DEBUG_ENSURE_UINT_LT(index, Size, "(Debug) Attempted to set an element out of range in StaticArray.");
		this->data[index] = value;
	}

	void set_all(T value)
	{
		for_range_var(i, Size)
		{
			this->data[i] = value;
		}
	}

	T const * get(size_t index)
	{
		ENSURE_UINT_LT(index, Size, "Attempted to get an element out of range in StaticArray.");
		return &this->data[index];
	}

	T const * get_unsafe(size_t index)
	{
		DEBUG_ENSURE_UINT_LT(index, Size, "Attempted to get an element out of range in StaticArray.");
		return &this->data[index];
	}

	T * get_mut(size_t index)
	{
		ENSURE_UINT_LT(index, Size, "Attempted to get an element out of range in StaticArray.");
		return &this->data[index];
	}

	T * get_mut_unsafe(size_t index)
	{
		DEBUG_ENSURE_UINT_LT(index, Size, "Attempted to get an element out of range in StaticArray.");
		return &this->data[index];
	}

	vds::SearchResult<T> find_get(T const * value)
	{
		vds::SearchResult<T> result;

		for_range_var(i, this->push_idx)
		{
			T const * target = this->get(i);

			if (*value == *target)
			{
				result.value = target;
				result.was_found = vds::SearchResultStatus_e::Found;
				return result;
			}
		}

		result.value = nullptr;
		result.was_found = vds::SearchResultStatus_e::Missing;
		return result;
	}

	vds::SearchResultMut<T> find_get_mut(T const * value)
	{
		vds::SearchResultMut<T> result;

		for_range_var(i, this->push_idx)
		{
			T * target = this->get_mut(i);

			if (*value == *target)
			{
				result.value = target;
				result.was_found = vds::SearchResultStatus_e::Found;
				return result;
			}
		}

		result.value = nullptr;
		result.was_found = vds::SearchResultStatus_e::Missing;
		return result;
	}

	void swap(size_t index_a, size_t index_b)
	{
		T temp = this->data[index_a];
		this->data[index_a] = this->data[index_b];
		this->data[index_b] = temp;
	}

	void sort_stackmode(void)
	{
		T temp;

		#define Q_LESS(i, j) this->data[i] < this->data[j]
		#define Q_SWAP(i, j) temp = this->data[i], this->data[i] = this->data[j], this->data[j] = temp

		QSORT(this->push_idx, Q_LESS, Q_SWAP);

		#undef Q_LESS
		#undef Q_SWAP
	}

	void sort_all(void)
	{
		T temp;

		#define Q_LESS(i, j) this->data[i] < this->data[j]
		#define Q_SWAP(i, j) temp = this->data[i], this->data[i] = this->data[j], this->data[j] = temp

		QSORT(Size, Q_LESS, Q_SWAP);

		#undef Q_LESS
		#undef Q_SWAP
	}

};

};
