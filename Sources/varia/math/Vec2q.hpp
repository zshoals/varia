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

VARIA_INLINE vec2q operator-(vec2q const & v)
{
	vec2q out;
	out.xs = -out.xs;
	out.ys = -out.ys;

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





//Swapped...
VARIA_INLINE vec2q operator+(f32q const & b, vec2q const & a)
{
	vec2q out;
	out.xs = a.xs + b;
	out.ys = a.ys + b;

	return out;
}

VARIA_INLINE vec2q operator-(f32q const & b, vec2q const & a)
{
	vec2q out;
	out.xs = a.xs - b;
	out.ys = a.ys - b;

	return out;
}

VARIA_INLINE vec2q operator*(f32q const & b, vec2q const & a)
{
	vec2q out;
	out.xs = a.xs * b;
	out.ys = a.ys * b;

	return out;
}

VARIA_INLINE vec2q operator/(f32q const & b, vec2q const & a)
{
	vec2q out;
	out.xs = a.xs / b;
	out.ys = a.ys / b;

	return out;
}

VARIA_INLINE void operator+=(f32q const & b, vec2q & a)
{
	a.xs += b;
	a.ys += b;
}

VARIA_INLINE void operator-=(f32q const & b, vec2q & a)
{
	a.xs -= b;
	a.ys -= b;
}

VARIA_INLINE void operator*=(f32q const & b, vec2q & a)
{
	a.xs *= b;
	a.ys *= b;
}

VARIA_INLINE void operator/=(f32q const & b, vec2q & a)
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

VARIA_INLINE vec2q vec2q_flip_x(vec2q v)
{
	v.xs = -v.xs;

	return v;
}

VARIA_INLINE vec2q vec2q_flip_y(vec2q v)
{
	v.ys = -v.ys;

	return v;
}

VARIA_INLINE vec2q vec2q_invert(vec2q v)
{
	v.xs = -v.xs;
	v.ys = -v.ys;

	return v;
}

VARIA_INLINE vec2q vec2q_normalize(vec2q vs)
{
	f32q const lens = vec2q_length(vs);

	vs.xs /= lens;
	vs.ys /= lens;

	return vs;
}

VARIA_INLINE f32q vec2q_dot(vec2q a, vec2q b)
{
	return (a.xs * b.xs) + (a.ys * b.ys);
}

VARIA_INLINE vec2q vec2q_rotate(vec2q v, f32q radians)
{
	f32q const v_angle = vec2q_angle(v);
	f32q const v_sin = f32q_sin(v_angle);
	f32q const v_cos = f32q_cos(v_angle);

	vec2q rotated;
	rotated.xs = (v.xs * v_cos) - (v.ys * v_sin);
	rotated.ys = (v.xs * v_sin) + (v.ys * v_cos);

	return rotated;
}

VARIA_INLINE vec2q vec2q_unit_vector_from_angle(f32q radians)
{
	vec2q out = {};
	out.xs = f32q_cos(radians);
	out.ys = f32q_sin(radians);

	return out;
}

VARIA_INLINE vec2q vec2q_match_angle_of_other(vec2q self, vec2q other)
{
	other = vec2q_normalize(other);

	self.xs *= other.xs;
	self.ys *= other.ys;

	return self;
}

VARIA_INLINE f32q vec2q_angle_between(vec2q a, vec2q b)
{
	f32q a_ang = vec2q_angle(a);
	f32q b_ang = vec2q_angle(b);

	return a_ang - b_ang;
}

VARIA_INLINE f32q_mask vec2q_angles_are_close(vec2q a, vec2q b, f32q limits_radians)
{
	a = vec2q_normalize(a);
	b = vec2q_normalize(b);

	f32q dot = f32q_abs(vec2q_dot(a, b));

	return dot < limits_radians;
}

VARIA_INLINE f32q_mask vec2q_angles_are_equal(vec2q a, vec2q b)
{
	a = vec2q_normalize(a);
	b = vec2q_normalize(b);

	f32q dot = f32q_abs(vec2q_dot(a, b));

	return dot < f32q_EPSILON();
}

VARIA_INLINE vec2q vec2q_reflect(vec2q v, vec2q impact_plane)
{
	//   https://math.stackexchange.com/a/13263

	vec2q const v_invert = vec2q_invert(v);
	vec2q const impact_plane_norm = vec2q_normalize(impact_plane);

	f32q const norm_len = vec2q_dot(v, impact_plane_norm);

	return v - (f32q_two() * (norm_len * impact_plane_norm));
}

//Sort of worked, but result seemed slightly inaccurate (0.5 degrees off?)
// VARIA_INLINE vec2q vec2q_reflect_old(vec2q v, vec2q non_normalized_surface_vec)
// {
// 	f32q signs = f32q_sign(v.xs) * f32q_sign(v.ys);

// 	f32q impact_angles = vec2q_angle(v);
// 	f32q normal_angles = vec2q_angle(non_normalized_surface_vec);

// 	f32q angle_diff = f32q_abs((impact_angles + f32q_PI()) - normal_angles);

// 	f32q rotated = angle_diff + normal_angles;
// 	rotated = f32q_wrap_radians(rotated * signs);

// 	return vec2q_unit_vector_from_angle(rotated);
// }