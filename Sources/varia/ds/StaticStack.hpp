#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"

namespace vds
{

template<typename T, int Size>
struct StaticStack
{
	T data[Size];
	size_t push_idx = 0;

	// StaticStack(void)
	// {
	// 	this->initialize();
	// }

	void initialize(void)
	{
		VARIA_ZERO_INIT(this);
	}

	void push(T value)
	{
		DEBUG_ENSURE_UINT_LT(push_idx, Size, "Attempted element push of full StaticStack.");

		this->data[this->push_idx] = value;
		++this->push_idx;
	}

	T pop(T value)
	{
		ENSURE_UINT_GTE(push_idx, 1, "Attempted element pop of empty StaticStack.");

		--this->push_idx;
		return this->data[this->push_idx];
	}
};

}