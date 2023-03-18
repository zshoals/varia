#pragma once

#include "varia/simd/simd_types.h"

struct alignas(32) vec2q
{
	f32q xs;
	f32q ys;
};
