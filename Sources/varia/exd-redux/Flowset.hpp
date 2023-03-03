#pragma once

#include "varia/vcommon.hpp"
#include "varia/simd/u32q.hpp"

#include <emmintrin.h>

struct alignas(64) exd_flowset_t
{
	uint32_t a[4];
	uint32_t b[4];
	uint32_t c[4];
	uint32_t d[4];
};

void func(void)
{
	alignof(exd_flowset_t);

	alignof(__m128);
}