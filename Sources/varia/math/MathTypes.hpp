#pragma once

#include "varia/simd/simd_types.h"

struct vec2
{
	float x;
	float y;
};

struct alignas(32) vec2q
{
	f32q xs;
	f32q ys;
};