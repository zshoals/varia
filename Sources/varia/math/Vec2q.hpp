#pragma once

#include "varia/math/MathTypes.hpp"

#include "varia/vcommon.hpp"

#include "varia/simd/f32q.hpp"
#include "varia/simd/simd_trig.h"

constexpr VARIA_INLINE void vec2q_normalize(vec2q & vs)
{
	f32q lens = f32q_sqrt(f32q_pow2(vs.xs) + f32q_pow2(vs.ys));

	vs.xs /= lens;
	vs.ys /= lens;
}

constexpr VARIA_INLINE f32q vec2q_dot(vec2q & a, vec2q & b)
{
	return (a.xs * b.xs) + (a.ys * b.ys);
}

