#pragma once

#include "varia/math/MathTypes.hpp"

#include "varia/vcommon.hpp"

#include "varia/simd/f32q.hpp"
#include "varia/simd/simd_trig.h"

constexpr VARIA_INLINE f32q vec2q_length(vec2q vs)
{
	return f32q_sqrt(f32q_pow2(vs.xs) + f32q_pow2(vs.ys));
}

constexpr VARIA_INLINE vec2q vec2q_normalize(vec2q vs)
{
	f32q lens = vec2q_length(vs);

	vs.xs /= lens;
	vs.ys /= lens;

	return vs;
}

constexpr VARIA_INLINE f32q vec2q_dot(vec2q a, vec2q b)
{
	return (a.xs * b.xs) + (a.ys * b.ys);
}

// constexpr VARIA_INLINE vec2q vec2q_deflect(vec2q v, vec2q non_normalized_surface_vec)
// {
// 	return void;
// }