#pragma once

#include "varia/vcommon.hpp"
#include <string.h>

#define VARIA_BITCAST(DESTINATION_PTR, SOURCE_PTR)\
	do\
	{\
		memcpy(DESTINATION_PTR, SOURCE_PTR, sizeof(*SOURCE_PTR));\
	} while (0)
	

template <typename TARGET_T, typename ORIGINAL_T>
VARIA_INLINE TARGET_T varia_bitcast(ORIGINAL_T * data)
{
	static_assert(sizeof(TARGET_T) == sizeof(*data), "Tried to bitcast elements of mismatched size.");

	TARGET_T out_type;
	memcpy(&out_type, data, sizeof(*data));

	return out_type;
}