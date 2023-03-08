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


struct alignas(64) mat4q
{
	f32q a;
	f32q b;
	f32q c;
	f32q d;

	f32q e;
	f32q f;
	f32q g;
	f32q h;

	f32q i;
	f32q j;
	f32q k;
	f32q l;

	f32q m;
	f32q n;
	f32q o;
	f32q p;
};