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

// struct alignas(64) mat3
// {
// 	float a; float b; float c;
// 	float d; float e; float f;
// 	float g; float h; float i;
// };

struct alignas(64) mat3
{
	float m[9];
};

struct alignas(64) mat4
{
	float m[16];
};
