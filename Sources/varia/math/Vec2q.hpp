#pragma once

#include "varia/math/MathTypes.hpp"

#include "varia/vcommon.hpp"

#include "varia/simd/f32q.hpp"
#include "varia/simd/simd_trig.h"


//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||              Operator Overloads (Vec2q <--> Vec2q)                  ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
VARIA_INLINE vec2q operator+(vec2q const & a, vec2q const & b)
{
	vec2q out;
	out.xs = a.xs + b.xs;
	out.ys = a.ys + b.ys;

	return out;
}

VARIA_INLINE vec2q operator-(vec2q const & a, vec2q const & b)
{
	vec2q out;
	out.xs = a.xs - b.xs;
	out.ys = a.ys - b.ys;

	return out;
}

VARIA_INLINE vec2q operator*(vec2q const & a, vec2q const & b)
{
	vec2q out;
	out.xs = a.xs * b.xs;
	out.ys = a.ys * b.ys;

	return out;
}

VARIA_INLINE vec2q operator/(vec2q const & a, vec2q const & b)
{
	vec2q out;
	out.xs = a.xs / b.xs;
	out.ys = a.ys / b.ys;

	return out;
}

VARIA_INLINE void operator+=(vec2q & a, vec2q const & b)
{
	a.xs += b.xs;
	a.ys += b.ys;
}

VARIA_INLINE void operator-=(vec2q & a, vec2q const & b)
{
	a.xs -= b.xs;
	a.ys -= b.ys;
}

VARIA_INLINE void operator*=(vec2q & a, vec2q const & b)
{
	a.xs *= b.xs;
	a.ys *= b.ys;
}

VARIA_INLINE void operator/=(vec2q & a, vec2q const & b)
{
	a.xs /= b.xs;
	a.ys /= b.ys;
}


//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||              Operator Overloads (Vec2q <--> F32q)                   ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

VARIA_INLINE vec2q operator+(vec2q const & a, f32q const & b)
{
	vec2q out;
	out.xs = a.xs + b;
	out.ys = a.ys + b;

	return out;
}

VARIA_INLINE vec2q operator-(vec2q const & a, f32q const & b)
{
	vec2q out;
	out.xs = a.xs - b;
	out.ys = a.ys - b;

	return out;
}

VARIA_INLINE vec2q operator*(vec2q const & a, f32q const & b)
{
	vec2q out;
	out.xs = a.xs * b;
	out.ys = a.ys * b;

	return out;
}

VARIA_INLINE vec2q operator/(vec2q const & a, f32q const & b)
{
	vec2q out;
	out.xs = a.xs / b;
	out.ys = a.ys / b;

	return out;
}

VARIA_INLINE void operator+=(vec2q & a, f32q const & b)
{
	a.xs += b;
	a.ys += b;
}

VARIA_INLINE void operator-=(vec2q & a, f32q const & b)
{
	a.xs -= b;
	a.ys -= b;
}

VARIA_INLINE void operator*=(vec2q & a, f32q const & b)
{
	a.xs *= b;
	a.ys *= b;
}

VARIA_INLINE void operator/=(vec2q & a, f32q const & b)
{
	a.xs /= b;
	a.ys /= b;
}


//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||              Operator Overloads End!!!!!!!!!!!!!!!!                 ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

VARIA_INLINE f32q vec2q_length(vec2q v)
{
	return f32q_sqrt(f32q_pow2(v.xs) + f32q_pow2(v.ys));
}

VARIA_INLINE f32q vec2q_angle(vec2q v)
{
	return f32q_wrap_radians(f32q_atan2(v.ys, v.xs));
}

VARIA_INLINE vec2q vec2q_invert(vec2q v)
{
	f32q negation = f32q_negative_one();
	v.xs *= negation;
	v.ys *= negation;

	return v;
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

//2 * (a + (dot(inv_anorm,nnorm) * n))
VARIA_INLINE vec2q vec2q_reflect(vec2q v, vec2q impact_plane)
{
	vec2q v_invert = vec2q_invert(v);
	vec2q vnorm = vec2q_normalize(v_invert);
	vec2q impact_plane_norm = vec2q_normalize(impact_plane);

	f32q norm_len = vec2q_dot(vnorm, impact_plane_norm);
	vec2q norm_len_vec;
	norm_len_vec.xs = norm_len;
	norm_len_vec.ys = norm_len;

	vec2q two;
	two.xs = f32q_two();
	two.ys = f32q_two();


	return (v * norm_len_vec) - (two * (norm_len_vec * impact_plane_norm));
}

VARIA_INLINE vec2q vec2q_reflect_old(vec2q v, vec2q non_normalized_surface_vec)
{
	f32q signs = f32q_sign(v.xs) * f32q_sign(v.ys);

	f32q impact_angles = vec2q_angle(v);
	f32q normal_angles = vec2q_angle(non_normalized_surface_vec);

	f32q angle_diff = f32q_abs((impact_angles + f32q_PI()) - normal_angles);

	f32q rotated = angle_diff + normal_angles;
	rotated = f32q_wrap_radians(rotated * signs);

	return vec2q_unit_vector_from_angle(rotated);
}