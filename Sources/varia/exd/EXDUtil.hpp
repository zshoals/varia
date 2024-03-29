#pragma once

#include "varia/vcommon.hpp"
#include "varia/math/Math.hpp"

constexpr u8 exd_utility_calculate_id_shift(size_t entity_count)
{
	size_t nearest = vmath_pow2_next_nearest(entity_count);
	u8 shift = vmath_pow2_to_bitshift_value(nearest);

	//Note(zshoals Dec-12-2022): The shift value is used for bitmasking
	//This value should result in 
	return shift;
}
