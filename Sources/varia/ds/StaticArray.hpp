#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include "varia/ds/qsort.h"
#include "varia/ds/SearchResult.hpp"

#include <string.h>



//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
//        Iterator
//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
//        End Iterator
//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
template <typename T, int Size>
struct vds_array_t
{
	T data[Size];
	size_t push_idx;

	struct vds_iterators_array_iter
	{
		private: 
			T * ptr;

		public: 
			vds_iterators_array_iter(T * ptr) { this->ptr = ptr; }
			vds_iterators_array_iter operator++() { ++this->ptr; return *this; }
			bool operator!=(vds_iterators_array_iter const & other) const { return this->ptr != other.ptr; }
			T & operator*() const { return *this->ptr; }
			// T & operator*() { return *this->ptr; }

	};

	vds_iterators_array_iter begin(void) /*const*/ { return vds_iterators_array_iter( &this->data[0] ); }
	vds_iterators_array_iter end(void) /*const*/ { return vds_iterators_array_iter( &this->data[this->push_idx] ); }
};


//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                   Free functions                                    ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

template <typename T, int Size>
void vds_array_initialize(vds_array_t<T, Size> * arr)
{
	VARIA_ZERO_INIT(arr);
}

template <typename T, int Size>
size_t vds_array_front(vds_array_t<T, Size> * arr)
{
	return 0;
}

template <typename T, int Size>
size_t vds_array_back(vds_array_t<T, Size> * arr)
{
	return arr->push_idx - 1;
}

template <typename T, int Size>
size_t vds_array_length(vds_array_t<T, Size> * arr)
{
	return arr->push_idx;
}

template <typename T, int Size>
size_t vds_array_capacity(vds_array_t<T, Size> * arr)
{
	return Size;
}

template <typename T, int Size>
bool vds_array_is_populated(vds_array_t<T, Size> * arr)
{
	return arr->push_idx > 0;
}

template <typename T, int Size>
bool vds_array_is_empty(vds_array_t<T, Size> * arr)
{
	return !vds_array_is_populated(arr);
}

template <typename T, int Size>
void vds_array_push(vds_array_t<T, Size> * arr, T value)
{
	DEBUG_ENSURE_UINT_LT(arr->push_idx, Size, "Attempted element push of full StaticArray");

	arr->data[arr->push_idx] = value;
	++arr->push_idx;
}

template <typename T, int Size>
void vds_array_push_without_data(vds_array_t<T, Size> * arr)
{
	DEBUG_ENSURE_UINT_LT(arr->push_idx, Size, "Attempted element push of full StaticArray");

	++arr->push_idx;
}

template <typename T, int Size>
T vds_array_pop(vds_array_t<T, Size> * arr)
{
	DEBUG_ENSURE_UINT_GTE(arr->push_idx, 1, "Attempted element pop of empty StaticArray.");

	--arr->push_idx;
	return arr->data[arr->push_idx];
}

template <typename T, int Size>
void vds_array_set(vds_array_t<T, Size> * arr, size_t index, T value)
{
	ENSURE_UINT_LT(index, Size, "Attempted to set an element out of range in StaticArray.");
	arr->data[index] = value;
}

template <typename T, int Size>
void vds_array_set_unsafe(vds_array_t<T, Size> * arr, size_t index, T value)
{
	DEBUG_ENSURE_UINT_LT(index, Size, "(Debug) Attempted to set an element out of range in StaticArray.");
	arr->data[index] = value;
}

template <typename T, int Size>
void vds_array_set_all(vds_array_t<T, Size> * arr, T value)
{
	for_range_var(i, Size)
	{
		arr->data[i] = value;
	}
}

template <typename T, int Size>
T const * vds_array_get(vds_array_t<T, Size> * arr, size_t index)
{
	ENSURE_UINT_LT(index, Size, "Attempted to get an element out of range in StaticArray.");
	return &arr->data[index];
}

template <typename T, int Size>
T const * vds_array_get_unsafe(vds_array_t<T, Size> * arr, size_t index)
{
	DEBUG_ENSURE_UINT_LT(index, Size, "Attempted to get an element out of range in StaticArray.");
	return &arr->data[index];
}

template <typename T, int Size>
T * vds_array_get_mut(vds_array_t<T, Size> * arr, size_t index)
{
	ENSURE_UINT_LT(index, Size, "Attempted to get an element out of range in StaticArray.");
	return &arr->data[index];
}

template <typename T, int Size>
T * vds_array_get_mut_unsafe(vds_array_t<T, Size> * arr, size_t index)
{
	DEBUG_ENSURE_UINT_LT(index, Size, "Attempted to get an element out of range in StaticArray.");
	return &arr->data[index];
}

template <typename T, int Size>
size_t vds_array_index_of(vds_array_t<T, Size> * arr, T const * value)
{
	for_range_var(i, arr->push_idx)
	{
		T const * target = vds_array_get(arr, i);

		if (*value == *target)
		{
			return i;
		}
	}

	return SIZE_MAX;
}

template <typename T, int Size>
vds::SearchResult<T> vds_array_find_get(vds_array_t<T, Size> * arr, T const * value)
{
	vds::SearchResult<T> result;

	for_range_var(i, arr->push_idx)
	{
		T const * target = vds_array_get(arr, i);

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

template <typename T, int Size>
vds::SearchResultMut<T> vds_array_find_get_mut(vds_array_t<T, Size> * arr, T const * value)
{
	vds::SearchResultMut<T> result;

	for_range_var(i, arr->push_idx)
	{
		T * target = vds_array_get_mut(arr, i);

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

template <typename T, int Size>
void vds_array_swap(vds_array_t<T, Size> * arr, size_t index_a, size_t index_b)
{
	T temp = arr->data[index_a];
	arr->data[index_a] = arr->data[index_b];
	arr->data[index_b] = temp;
}

template <typename T, int Size>
T vds_array_swap_and_pop(vds_array_t<T, Size> * arr, size_t index)
{
	vds_array_swap(arr, index, vds_array_back(arr));
	return vds_array_pop(arr);
}

template <typename T, int Size>
void vds_array_sort_stackmode(vds_array_t<T, Size> * arr)
{
	T temp;

	#define Q_LESS(i, j) arr->data[i] < arr->data[j]
	#define Q_SWAP(i, j) temp = arr->data[i], arr->data[i] = arr->data[j], arr->data[j] = temp

	QSORT(arr->push_idx, Q_LESS, Q_SWAP);

	#undef Q_LESS
	#undef Q_SWAP
}

template <typename T, int Size>
void vds_array_sort_all(vds_array_t<T, Size> * arr)
{
	T temp;

	#define Q_LESS(i, j) arr->data[i] < arr->data[j]
	#define Q_SWAP(i, j) temp = arr->data[i], arr->data[i] = arr->data[j], arr->data[j] = temp

	QSORT(Size, Q_LESS, Q_SWAP);

	#undef Q_LESS
	#undef Q_SWAP
}

template <typename T, int Size, typename FUNC>
void vds_array_iterate(vds_array_t<T, Size> * arr, const FUNC f)
{
	const size_t len = vds_array_length(arr)
	for_range_var(i, len)
	{
		T * element = vds_array_get_mut_unsafe(arr, i);
		f(element);
	}
}
