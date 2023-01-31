#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"


template<typename T, int Size>
struct vds_stack_t
{
	T data[Size];
	size_t push_idx;
};

template<typename T, int Size>
void vds_stack_initialize(vds_stack_t<T, Size> * self)
{
	VARIA_ZERO_INIT(self);
}

template<typename T, int Size>
void vds_stack_push(vds_stack_t<T, Size> * self, T value)
{
	DEBUG_ENSURE_UINT_LT(push_idx, Size, "Attempted element push of full vds_stack_t.");

	self->data[self->push_idx] = value;
	++self->push_idx;
}

template<typename T, int Size>
T vds_stack_pop(vds_stack_t<T, Size> * self, T value)
{
	ENSURE_UINT_GTE(push_idx, 1, "Attempted element pop of empty vds_stack_t.");

	--self->push_idx;
	return self->data[self->push_idx];
}