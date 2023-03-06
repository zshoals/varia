#pragma once

#include "varia/math/MathTypes.hpp"

#include "varia/vcommon.hpp"

#include "varia/simd/f32q.hpp"
#include "varia/simd/simd_trig.h"

VARIA_INLINE f32q vec2q_length(vec2q v)
{
	return f32q_sqrt(f32q_pow2(v.xs) + f32q_pow2(v.ys));
}

VARIA_INLINE f32q vec2q_atan2(vec2q v)
{
	return f32q_wrap_radians(f32q_atan2(v.ys, v.xs));
}

VARIA_INLINE vec2q vec2q_normalize(vec2q vs)
{
	f32q lens = vec2q_length(vs);

	vs.xs /= lens;
	vs.ys /= lens;

	return vs;
}

VARIA_INLINE vec2q vec2q_unit_vector_from_angle(f32q radians)
{
	vec2q out = {};
	out.xs = f32q_cos(radians);
	out.ys = f32q_sin(radians);

	return out;
}

VARIA_INLINE f32q vec2q_dot(vec2q a, vec2q b)
{
	return (a.xs * b.xs) + (a.ys * b.ys);
}

//Assumes radians?
//Probably doesn't work at all, actually?
VARIA_INLINE vec2q vec2q_reflect(vec2q v, vec2q non_normalized_surface_vec)
{
	f32q signs = f32q_sign(v.xs) * f32q_sign(v.ys);

	f32q impact_angles = vec2q_atan2(v) * signs;
	f32q normal_angles = vec2q_atan2(non_normalized_surface_vec);

	f32q angle_diff = f32q_abs((impact_angles + f32q_PI()) - normal_angles);

	f32q rotated = angle_diff + normal_angles;
	rotated = f32q_wrap_radians(rotated * signs);

	return vec2q_unit_vector_from_angle(rotated);
}