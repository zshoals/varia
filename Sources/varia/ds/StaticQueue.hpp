#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"

#include "StaticRingbuf.hpp"
#include "Result.hpp"

namespace vds
{

template<typename T, int Size>
struct StaticQueue
{
	vds::StaticRingbuf<T, Size> ring;

	// StaticQueue(void)
	// {
	// 	this->initialize();
	// }

	void initialize(void)
	{
		VARIA_ZERO_INIT(this);
	}

	//Tries to push a new value onto the queue. 
	//If there was room to add the value, returns true, otherwise false
	bool enqueue(T value)
	{
		bool has_room = ring.is_not_full();

		if (has_room)
		{
			ring.push_back(value);
			return true;
		}
		else
		{
			return false;
		}
	}

	vds::Result<T> dequeue(void)
	{
		vds::Result<T> result;
		bool has_elements = ring.is_populated();

		if (has_elements)
		{
			result.valid = vds::ResultStatus_e::Success;
			result.value = ring.pop_front();

			return result;
		}
		else
		{
			result.valid = vds::ResultStatus_e::Failure;

			return result;
		}
	}
};

}