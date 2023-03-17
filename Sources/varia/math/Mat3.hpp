#include "MathTypes.hpp"

#include "varia/Vcommon.hpp"
#include "varia/simd/f32q.hpp"

#include <string.h>


/*
	a b c d    00 01 02 03           (a00 + b04 + c08 + d12)
	e f g h    04 05 06 07           (e00 + f04 + g08 + h12)
	i j k l    08 09 10 11           
	m n o p    12 13 14 15           
*/


/*
	a b c      0 1 2
	d e f      3 4 5
	g h i      6 7 8

	a = a0 b3 c6   b = a1 b4 c7   c = a2 b5 c8
	d = d0 e3 f6   e = d1 e4 f7   f = d2 e5 f8
	g = g0 h3 i6   h = g1 h4 i7   i = g2 h5 i8



	a = aa bd cg     b = ab be ch     c = ac bf ci
	d = da ed fg     e = db ee fh     f = dc ef fi
	g = ga hd ig     h = gb he ih     i = gc hf ii
*/


VARIA_INLINE mat3 mat3_identity(void)
{
	mat3 out;

	memset(&out, 0, sizeof(mat3));
	out.m[0] = 1.0f;
	out.m[4] = 1.0f;
	out.m[8] = 1.0f;

	return out;
}

VARIA_INLINE mat3 mat3_transpose(mat3 m)
{
	mat3 out;

	out.m[0] = m.m[0];
	out.m[1] = m.m[3];
	out.m[2] = m.m[6];
	out.m[3] = m.m[1];
	out.m[4] = m.m[4];
	out.m[5] = m.m[7];
	out.m[6] = m.m[2];
	out.m[7] = m.m[5];
	out.m[8] = m.m[8];

	return out;
}

VARIA_INLINE mat3 mat3_mulmat3(mat3 left, mat3 right)
{
	mat3 out;

	out.m[0] = (left.m[0] * right.m[0]) + (left.m[1] * right.m[3]) + (left.m[2] * right.m[6]);
	out.m[1] = (left.m[0] * right.m[1]) + (left.m[1] * right.m[4]) + (left.m[2] * right.m[7]);
	out.m[2] = (left.m[0] * right.m[2]) + (left.m[1] * right.m[5]) + (left.m[2] * right.m[8]);

	out.m[3] = (left.m[3] * right.m[0]) + (left.m[4] * right.m[3]) + (left.m[5] * right.m[6]);
	out.m[4] = (left.m[3] * right.m[1]) + (left.m[4] * right.m[4]) + (left.m[5] * right.m[7]);
	out.m[5] = (left.m[3] * right.m[2]) + (left.m[4] * right.m[5]) + (left.m[5] * right.m[8]);

	out.m[6] = (left.m[6] * right.m[0]) + (left.m[7] * right.m[3]) + (left.m[8] * right.m[6]);
	out.m[7] = (left.m[6] * right.m[1]) + (left.m[7] * right.m[4]) + (left.m[8] * right.m[7]);
	out.m[8] = (left.m[6] * right.m[2]) + (left.m[7] * right.m[5]) + (left.m[8] * right.m[8]);

	return out;
}

VARIA_INLINE vec2 mat3_mulvec2(mat3 left, vec2 right)
{
	vec2 out;

	float w = 1.0f / left.m[8];
	out.x = (((left.m[0] * right.x) + (left.m[3] * right.y)) + left.m[2]) * w;
	out.y = (((left.m[1] * right.x) + (left.m[4] * right.y)) + left.m[5]) * w;

	return out;
}