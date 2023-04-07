#pragma once

#include "varia/Vcommon.hpp"
#include "varia/ds/Allocator.hpp"
#include "varia/util/Memory.hpp"
#include "varia/ds/Option.hpp"
#include "varia/ds/ControlFlow.hpp"

#include "varia/Log.hpp"
#include "varia/Validation.hpp"

#include "varia/ds/qsort.h"

template <typename T>
struct vds_array_t
{
	T * _data;
	T * _error_object;
	i64 _length;
	i64 _capacity;

	T const & operator[](i64 i) const 
	{ 
		DEBUG_ENSURE(i >= 0 && i < this->_capacity, "vds_array_t:> Index out of range");
		return this->_data[i]; 
	}

	T & operator[](i64 i) 
	{ 
		DEBUG_ENSURE(i >= 0 && i < this->_capacity, "vds_array_t:> Index out of range");
		return this->_data[i]; 
	}

	T & at(i64 i)
	{
		DEBUG_ENSURE(i >= 0 && i < this->_capacity, "vds_array_t:> Index out of range");
		return this->_data[i];
	}

};

template <typename T>
void vds_array_initialize(vds_array_t<T> * arr, vds_allocator_t * alloc, i64 count)
{
	arr->_error_object = vds_allocator_malloc_aligned(alloc, T, 1, 16);
	arr->_data = vds_allocator_malloc_aligned(alloc, T, count, 64);
	arr->_capacity = count;
	arr->_length = 0;

	memset(arr->_error_object, 0xC9, sizeof(T));
}


//Note(zshoals 03-14-2023):> Warning: You must include extra space for one error object!
template <typename T>
void vds_array_initialize_direct(vds_array_t<T> * arr, T * mem, i64 count_excluding_error_object)
{
	arr->_error_object = mem;
	arr->_data = mem + 1;
	arr->_capacity = count_excluding_error_object;
	arr->_length = 0;
}

template <typename T>
void vds_array_reset(vds_array_t<T> * arr)
{
	arr->_length = 0;
}

template <typename T>
vds_array_t<T> vds_array_deep_copy(vds_array_t<T> * arr, vds_allocator_t * alloc)
{
	vds_array_t<T> out;
	out._error_object = vds_allocator_allocate_aligned_count(alloc, sizeof(T), 1, 16);
	out._data = vds_allocator_allocate_aligned_count(alloc, sizeof(T), arr->_capacity, 64);
	out._capacity = arr->_capacity;
	out._length = arr->_length;

	return out;
}

template <typename T>
i64 vds_array_length(vds_array_t<T> * arr)
{
	return arr->_length;
}

template <typename T>
i64 vds_array_capacity(vds_array_t<T> * arr)
{
	return arr->_capacity;
}

template <typename T>
bool vds_array_is_error_element(vds_array_t<T> * arr, T * possible_err)
{
	return (possible_err == arr->_error_object);
}

template <typename T>
T * vds_array_data(vds_array_t<T> * arr)
{
	return arr->_data;
}

template <typename T>
void vds_array_maximize(vds_array_t<T> * arr)
{
	arr->_length = arr->_capacity;
}

template <typename T>
void vds_array_push(vds_array_t<T> * arr, T value)
{
	if (arr->_length < arr->_capacity)
	{
		arr->_data[arr->_length] = value;
		++arr->_length;
	}
	else
	{
		Glog_string("vds_array_t:> Pushed a full array at "); Glog_time(); Glog_newline();
	}
}

template <typename T>
T const * vds_array_pop(vds_array_t<T> * arr)
{
	if (arr->_length > 0)
	{
		--arr->_length;

		return &arr->_data[arr->_length];
	}
	else
	{
		Glog_string("vds_array_t:> Popped an empty array at "); Glog_time(); Glog_newline();

		return arr->_error_object;
	}
}

template <typename T>
T const * vds_array_swap_and_pop(vds_array_t<T> * arr, int64_t swap_idx)
{
	if (arr->_length > 0)
	{
		vds_array_swap(arr, swap_idx, arr->_length - 1);

		--arr->_length;
		return &arr->_data[arr->_length];
	}
	else
	{
		Glog_string("vds_array_t:> Popped an empty array at "); Glog_time(); Glog_newline();

		return arr->_error_object;
	}
}

template <typename T, typename FUNC>
void vds_array_pop_all(vds_array_t<T> * arr, FUNC f)
{
	for (i64 i = arr->_length - 1; i >= 0; --i)
	{
		T * element = &arr->_data[i];
		f(element);

		--arr->_length;
	}
}

template <typename T>
void vds_array_swap(vds_array_t<T> * arr, i64 a, i64 b)
{
	T temp = (*arr)[a];
	(*arr)[a] = (*arr)[b];
	(*arr)[b] = temp;
}

template <typename T, typename FUNC>
void vds_array_iterate(vds_array_t<T> * arr, FUNC f)
{
	i64 const len = arr->_length;
	for (i64 i = 0; i < len; ++i)
	{
		T * element = &arr->_data[i];
		f(element, i);
	}
}

template <typename T, typename FUNC>
void vds_array_iterate_cf(vds_array_t<T> * arr, FUNC f)
{
	i64 const len = arr->_length;
	vds_controlflow_e flowstate = vds_controlflow_e::Continue;
	for (i64 i = 0; i < len; ++i)
	{
		if (flowstate == vds_controlflow_e::Break) { break; }
		T * element = &arr->_data[i];
		f(element, i, &flowstate);
	}
}

template <typename T, typename FUNC>
void vds_array_reverse_iterate(vds_array_t<T> * arr, FUNC f)
{
	i64 const len = arr->_length - 1;
	for (i64 = len; i > 0; --i)
	{
		T * element = &arr->_data[i];
		f(element, i);
	}
}

template <typename T, typename FUNC>
vds_option_t<T *> vds_array_find_get(vds_array_t<T> * arr, FUNC f)
{
	i64 const len = arr->_length;
	for (i64 i = 0; i < len; ++i)
	{
		T * element = &arr->_data[i];
		if (f(element)) return vds_option_create_some(element);
	}

	return vds_option_create_none<T *>();
}


template <typename T>
void vds_array_sort_all(vds_array_t<T> * arr)
{
	T temp;

	#define Q_LESS(i, j) arr->_data[i] < arr->_data[j]
	#define Q_SWAP(i, j) temp = arr->_data[i], arr->_data[i] = arr->_data[j], arr->_data[j] = temp

	QSORT(arr->_length, Q_LESS, Q_SWAP);

	#undef Q_LESS
	#undef Q_SWAP
}


template <typename T>
vds_array_t<T> vds_array_subsect(vds_array_t<T> * arr, i64 begin, i64 end)
{
	vds_array_t<T> sub;


	if //Out of bounds
	( 
		(begin < 0) || 
		(begin >= arr->_capacity) || 
		(end < begin) || 
		(end >= arr->_capacity)
	)
	{
		//TODO(zshoals 03-20-2023):> Not safe at all; what if the original element gets obliterated?
		//Actually maybe it's ok...unsure
		sub._data = arr->_error_object;
		sub._error_object = arr->_error_object;
		sub._capacity = 0;
		sub._length = 0;

		return sub;
	}
	else
	{
		sub._data = &arr->_data[begin];
		sub._error_object = arr->_error_object;
		sub._capacity = end - begin + 1;
		//Note(zshoals 03-10-2023):> Is it proper to make the push count 0?
		//If the exterior array has pushed values, and then this one pushes...then what?
		sub._length = 0;

		return sub;
	}

}