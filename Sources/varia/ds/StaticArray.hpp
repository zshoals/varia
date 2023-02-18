#pragma once

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
	alignas(16) T data[Size];
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
	memset(arr, 0, sizeof(*arr));
}

template <typename T, int Size>
size_t vds_array_front(vds_array_t<T, Size> const * arr)
{
	return 0;
}

template <typename T, int Size>
size_t vds_array_back(vds_array_t<T, Size> const * arr)
{
	return arr->push_idx - 1;
}

template <typename T, int Size>
size_t vds_array_length(vds_array_t<T, Size> const * arr)
{
	return arr->push_idx;
}

template <typename T, int Size>
size_t vds_array_capacity(vds_array_t<T, Size> const * arr)
{
	return Size;
}

template <typename T, int Size>
bool vds_array_is_populated(vds_array_t<T, Size> const * arr)
{
	return arr->push_idx > 0;
}

template <typename T, int Size>
bool vds_array_is_empty(vds_array_t<T, Size> const * arr)
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
bool vds_array_try_push(vds_array_t<T, Size> * arr, T value)
{
	if (arr->push_idx < Size)
	{
		arr->data[arr->push_idx] = value;
		++arr->push_idx;
		return true;
	}

	return false;
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

// template <typename T, int Size>
// void vds_array_try_pop(vds_array_t<T, Size> * arr, T value)
// {
// 	if (arr->push_idx >= 1)
// 	{
// 		--arr->push_idx;
// 		return arr->data[arr->push_idx];
// 	}
// }

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
T const * vds_array_get(vds_array_t<T, Size> const * arr, size_t index)
{
	ENSURE_UINT_LT(index, Size, "Attempted to get an element out of range in StaticArray.");
	return &arr->data[index];
}

template <typename T, int Size>
T const * vds_array_get_unsafe(vds_array_t<T, Size> const * arr, size_t index)
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
size_t vds_array_index_of(vds_array_t<T, Size> const * arr, T const * value)
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
vds_search_result_t<T> vds_array_find_get(vds_array_t<T, Size> * arr, T const * value)
{
	vds_search_result_t<T> result;

	for_range_var(i, arr->push_idx)
	{
		T const * target = vds_array_get(arr, i);

		if (*value == *target)
		{
			result.value = target;
			result.was_found = VDS_SEARCH_RESULT_STATUS_FOUND_E;
			return result;
		}
	}

	result.value = nullptr;
	result.was_found = VDS_SEARCH_RESULT_STATUS_MISSING_E;
	return result;
}

template <typename T, int Size>
vds_search_result_mut_t<T> vds_array_find_get_mut(vds_array_t<T, Size> * arr, T const * value)
{
	vds_search_result_mut_t<T> result;

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

template <typename T, int Size, typename Predicate>
void vds_array_for_each(vds_array_t<T, Size> * arr, const Predicate f)
{
	const size_t len = vds_array_length(arr);
	for_range_var(i, len)
	{
		T * element = vds_array_get_mut_unsafe(arr, i);
		f(element);
	}
}

template <typename T, int Size, typename Predicate>
void vds_array_for_each_with_index(vds_array_t<T, Size> * arr, const Predicate f)
{
	const size_t len = vds_array_length(arr);
	for_range_var(i, len)
	{
		T * element = vds_array_get_mut_unsafe(arr, i);
		f(element, i);
	}
}

template <typename T, int Size, typename Predicate>
void vds_array_iterate_step_4(vds_array_t<T, Size> * arr, const Predicate f)
{
	static_assert(Size % 4 == 0, "Size must be a multiple of four"); //4 f32's to a SIMD register in SSE2

	const size_t len = vds_array_length(arr);
	for(int i = 0; i < len; i += 4)
	{
		T * quad_element = vds_array_get_mut_unsafe(arr, i);
		f(quad_element);
	}
}

template <typename T, int Size, typename Predicate>
void vds_array_iterate_step_8(vds_array_t<T, Size> * arr, const Predicate f)
{
	static_assert(Size % 8 == 0, "Size must be a multiple of eight"); //4 f32's to a SIMD register in SSE2

	const size_t len = vds_array_length(arr);
	for(int i = 0; i < len; i += 8)
	{
		T * octo_element = vds_array_get_mut_unsafe(arr, i);
		f(octo_element);
	}
}

template <typename T, int Size, typename Predicate>
void vds_array_iterate_step_16(vds_array_t<T, Size> * arr, const Predicate f)
{
	static_assert(Size % 16 == 0, "Size must be a multiple of sixteen"); //4 f32's to a SIMD register in SSE2

	const size_t len = vds_array_length(arr);
	for(int i = 0; i < len; i += 16)
	{
		T * sixteen_element = vds_array_get_mut_unsafe(arr, i);
		f(sixteen_element);
	}
}




//Note(zshoals 02-02-2023):> Testing of overloading a dereference operator
//Wanted to try and make it so that a pointer cannot accidentally be incremented/added to
//or perform an improper action with it.

// struct bob
// {
// 	int a;
// };

// template<typename T>
// struct const_ptr
// {
// 	T * const ptr;
// 	T * operator->() { return ptr; }
// };

// void random(void)
// {
// 	vds_array_t<bob, 100> ___do_not_use_arr;
// 	vds_array_t<bob, 100> * arr = &___do_not_use_arr;

// 	vds_array_for_each(arr, [](const_ptr<bob> value)
// 	{
// 		value->a += 3498080;
// 	});
// }