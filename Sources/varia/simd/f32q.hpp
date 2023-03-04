#pragma once

#include "varia/simd/simd_types.h"

#include "varia/vcommon.hpp"
#include "kinc/simd/float32x4.h"
#include "simd_trig.h"

VARIA_INLINE f32q f32q_load(const f32 *values) { return kinc_float32x4_intrin_load(values); }
VARIA_INLINE f32q f32q_load_u(const f32 *values) { return kinc_float32x4_intrin_load_unaligned(values); }
VARIA_INLINE void f32q_store(f32 *destination, f32q n) { kinc_float32x4_store(destination, n); } 
VARIA_INLINE void f32q_store_u(f32 *destination, f32q n) { kinc_float32x4_store_unaligned(destination, n); } 
VARIA_INLINE f32q f32q_set(f32 a, f32 b, f32 c, f32 d) { return kinc_float32x4_load(a, b, c, d); }
VARIA_INLINE f32q f32q_set_all(f32 n) { return kinc_float32x4_load_all(n); }
VARIA_INLINE f32 f32q_get(f32q n, int index) { return kinc_float32x4_get(n, index); }
VARIA_INLINE f32q f32q_abs(f32q n) { return kinc_float32x4_abs(n); }
VARIA_INLINE f32q f32q_add(f32q a, f32q b) { return kinc_float32x4_add(a, b); }
VARIA_INLINE f32q f32q_sub(f32q a, f32q b) { return kinc_float32x4_sub(a, b); }
VARIA_INLINE f32q f32q_mul(f32q a, f32q b) { return kinc_float32x4_mul(a, b); }
VARIA_INLINE f32q f32q_div(f32q a, f32q b) { return kinc_float32x4_div(a, b); }
VARIA_INLINE f32q f32q_max(f32q a, f32q b) { return kinc_float32x4_max(a, b); }
VARIA_INLINE f32q f32q_min(f32q a, f32q b) { return kinc_float32x4_min(a, b); }
VARIA_INLINE f32q f32q_neg(f32q n) { return kinc_float32x4_neg(n); }
VARIA_INLINE f32q f32q_sqrt(f32q n) { return kinc_float32x4_sqrt(n); }
VARIA_INLINE f32q f32q_reciprocal_approximation(f32q n) { return kinc_float32x4_reciprocal_approximation(n); }
VARIA_INLINE f32q f32q_reciprocal_sqrt_approximation(f32q n) { return kinc_float32x4_reciprocal_sqrt_approximation(n); }
VARIA_INLINE f32q f32q_and(f32q a, f32q b) { return kinc_float32x4_and(a, b); }
VARIA_INLINE f32q f32q_or(f32q a, f32q b) { return kinc_float32x4_or(a, b); }
VARIA_INLINE f32q f32q_xor(f32q a, f32q b) { return kinc_float32x4_xor(a, b); }
VARIA_INLINE f32q f32q_not(f32q n) { return kinc_float32x4_not(n); }
VARIA_INLINE f32q_mask f32q_cmpeq(f32q a, f32q b) { return kinc_float32x4_cmpeq(a, b); }
VARIA_INLINE f32q_mask f32q_cmpneq(f32q a, f32q b) { return kinc_float32x4_cmpneq(a, b); }
VARIA_INLINE f32q_mask f32q_cmpgt(f32q a, f32q b) { return kinc_float32x4_cmpgt(a, b); }
VARIA_INLINE f32q_mask f32q_cmpgte(f32q a, f32q b) { return kinc_float32x4_cmpge(a, b); }
VARIA_INLINE f32q_mask f32q_cmplt(f32q a, f32q b) { return kinc_float32x4_cmplt(a, b); }
VARIA_INLINE f32q_mask f32q_cmplte(f32q a, f32q b) { return kinc_float32x4_cmple(a, b); }
VARIA_INLINE f32q f32q_select(f32q a, f32q b, f32q_mask mask) { return kinc_float32x4_sel(a, b, mask); } 

//Operator Overloads
VARIA_INLINE f32q operator+(f32q const & a, f32q const & b) { return f32q_add(a, b); }
VARIA_INLINE f32q operator-(f32q const & a, f32q const & b) { return f32q_sub(a, b); }
VARIA_INLINE f32q operator*(f32q const & a, f32q const & b) { return f32q_mul(a, b); }
VARIA_INLINE f32q operator/(f32q const & a, f32q const & b) { return f32q_div(a, b); }
VARIA_INLINE f32q operator-(f32q const & a) { return f32q_neg(a); }
VARIA_INLINE void operator+=(f32q & a, f32q const & b) { a = f32q_add(a, b); }
VARIA_INLINE void operator-=(f32q & a, f32q const & b) { a = f32q_sub(a, b); }
VARIA_INLINE void operator*=(f32q & a, f32q const & b) { a = f32q_mul(a, b); }
VARIA_INLINE void operator/=(f32q & a, f32q const & b) { a = f32q_div(a, b); }
VARIA_INLINE f32q operator&(f32q const & a, f32q const & b) { return f32q_and(a, b); }
VARIA_INLINE f32q operator|(f32q const & a, f32q const & b) { return f32q_or(a, b); }
VARIA_INLINE f32q operator^(f32q const & a, f32q const & b) { return f32q_xor(a, b); }
VARIA_INLINE f32q operator~(f32q const & n) { return f32q_not(n); }
VARIA_INLINE void operator&=(f32q & a, f32q const & b) { a = f32q_and(a, b); }
VARIA_INLINE void operator|=(f32q & a, f32q const & b) { a = f32q_or(a, b); }
VARIA_INLINE void operator^=(f32q & a, f32q const & b) { a = f32q_xor(a, b); }
VARIA_INLINE f32q_mask operator>(f32q const & a, f32q const & b) { return f32q_cmpgt(a, b); }
VARIA_INLINE f32q_mask operator>=(f32q const & a, f32q const & b) { return f32q_cmpgte(a, b); }
VARIA_INLINE f32q_mask operator<(f32q const & a, f32q const & b) { return f32q_cmplt(a, b); }
VARIA_INLINE f32q_mask operator<=(f32q const & a, f32q const & b) { return f32q_cmplte(a, b); }
VARIA_INLINE f32q_mask operator==(f32q const & a, f32q const & b) { return f32q_cmpeq(a, b); }
VARIA_INLINE f32q_mask operator!=(f32q const & a, f32q const & b) { return f32q_cmpneq(a, b); }

//Extensions
VARIA_INLINE f32q f32q_zero(void) { return f32q_set_all(0.0f); }
VARIA_INLINE f32q_mask f32q_ones_mask_lane0(void) { 
	return f32q_cmplt(f32q_set(0.0f, 1.0f, 1.0f, 1.0f), f32q_set(1.0f, 0.0f, 0.0f, 0.0f));
}
VARIA_INLINE f32q_mask f32q_ones_mask_lane1(void) { 
	return f32q_cmplt(f32q_set(1.0f, 0.0f, 1.0f, 1.0f), f32q_set(0.0f, 1.0f, 0.0f, 0.0f));
}
VARIA_INLINE f32q_mask f32q_ones_mask_lane2(void) { 
	return f32q_cmplt(f32q_set(1.0f, 1.0f, 0.0f, 1.0f), f32q_set(0.0f, 0.0f, 1.0f, 0.0f));
}
VARIA_INLINE f32q_mask f32q_ones_mask_lane3(void) { 
	return f32q_cmplt(f32q_set(1.0f, 1.0f, 1.0f, 0.0f), f32q_set(0.0f, 0.0f, 0.0f, 1.0f));
}
VARIA_INLINE f32q_mask f32q_mask_zeroes(void) { return f32q_set_all(0.0f); }
VARIA_INLINE f32q_mask f32q_mask_ones(void) 
{ 
	f32q zeroes = f32q_mask_zeroes();
	return f32q_cmpeq(zeroes, zeroes); 
}

VARIA_INLINE f32q f32q_round(f32q n)
{
	#if defined(KINC_SSE2)
		__m128i y1 = _mm_cvtps_epi32(n);
		f32q y2 = _mm_cvtepi32_ps(y1);

		y2 |= (n & f32q_set_all(-0.0f));

		f32q overflow_mask = _mm_cmpneq_ps(_mm_castsi128_ps(y1), _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));

		return f32q_select(overflow_mask, y2, n);
	#else
		n.values[0] = roundf(n.values[0]);
		n.values[1] = roundf(n.values[1]);
		n.values[2] = roundf(n.values[2]);
		n.values[3] = roundf(n.values[3]);

		return n;
	#endif
}

VARIA_INLINE f32q f32q_truncate(f32q n)
{
	#if defined(KINC_SSE2)
		__m128i y1 = _mm_cvttps_epi32(n);
		f32q y2 = _mm_cvtepi32_ps(y1);

		y2 |= (n & f32q_set_all(-0.0f));

		f32q overflow_mask = _mm_cmpneq_ps(_mm_castsi128_ps(y1), _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));

		return f32q_select(overflow_mask, y2, n);
	#else
		n.values[0] = (float)((int)(n.values[0]));
		n.values[1] = (float)((int)(n.values[1]));
		n.values[2] = (float)((int)(n.values[2]));
		n.values[3] = (float)((int)(n.values[3]));

		return n;
	#endif
}

VARIA_INLINE f32q f32q_floor(f32q n)
{
	#if defined(KINC_SSE) || defined(KINC_SSE2)
		f32q y = f32q_round(n);
		y += f32q_set_all(1.0f) & (y > n);

		y |= (n & f32q_set_all(-0.0f));

		return y;
	#else
		n.values[0] = floorf(n.values[0]);
		n.values[1] = floorf(n.values[1]);
		n.values[2] = floorf(n.values[2]);
		n.values[3] = floorf(n.values[3]);

		return n;
	#endif
}

VARIA_INLINE f32q f32q_floor(f32q n)
{
	#if defined(KINC_SSE) || defined(KINC_SSE2)
		f32q y = f32q_round(n);
		y += f32q_set_all(1.0f) & (y < n);

		y |= (n & f32q_set_all(-0.0f));

		return y;
	#else
		n.values[0] = ceilf(n.values[0]);
		n.values[1] = ceilf(n.values[1]);
		n.values[2] = ceilf(n.values[2]);
		n.values[3] = ceilf(n.values[3]);

		return n;
	#endif
}

VARIA_INLINE f32q f32q_pow(f32q n, int exp)
{
	for (int i = 0; i < exp; ++i)
	{
		n = f32q_mul(n, n);
	}

	return n;
}

VARIA_INLINE f32q f32q_pow2(f32q n)
{
	return n * n;
}

VARIA_INLINE f32q f32q_rad2deg(f32q radians)
{
	const float PI = 3.145926545f;
	return (radians * (f32q_set_all(180.0f) / f32q_set_all(PI)));
}

VARIA_INLINE f32q f32q_deg2rad(f32q degrees)
{
	const float PI = 3.145926545f;
	return (degrees * (f32q_set_all(PI) / f32q_set_all(180.0f)));
}

VARIA_INLINE f32q f32q_lerp(f32q n, f32q starts, f32q ends)
{
	return (starts * (f32q_set_all(1.0f) - n)) + (ends * n);
}

VARIA_INLINE f32q f32q_linear_norm(f32q n, f32q starts, f32q ends)
{
	return (n - starts) / (ends - starts);
}

VARIA_INLINE f32q f32q_remap(f32q n, f32q starts1, f32q ends1, f32q starts2, f32q ends2)
{
	return f32q_linear_norm(f32q_lerp(n, starts1, ends1), starts2, ends2);
}

VARIA_INLINE f32q f32q_clamp(f32q n, f32q lowers, f32q uppers)
{
	return f32q_min(lowers, f32q_max(n, uppers));
}

VARIA_INLINE f32q f32q_smoothstep(f32q n, f32q lower_edges, f32q upper_edges)
{
	n = f32q_clamp(n, lower_edges, upper_edges);
	n = f32q_linear_norm(n, lower_edges, upper_edges);
	//Smoothstep approx = [[ x = 3x^2 - 2x^3]]
	return f32q_sub(f32q_mul(f32q_set_all(3.0f), f32q_pow(n, 2)), f32q_mul(f32q_set_all(2.0f), f32q_pow(n, 3)));
}

VARIA_INLINE f32q f32q_sin(f32q angles)
{
	return simd_fq_sin(angles);
}

VARIA_INLINE f32q f32q_cos(f32q angles)
{
	return simd_fq_cos(angles);
}

VARIA_INLINE f32q f32q_tan(f32q angles)
{
	return simd_fq_tan(angles);
}

VARIA_INLINE f32q f32q_atan2(f32q ys, f32q xs)
{
	return simd_fq_atan2(ys, xs);
}

VARIA_INLINE f32q f32q_wrap_angle(f32q degrees)
{
	//355 + 1000
	//1000 - 355 = 645

	// 200 - 1000
	//-800
	f32q degrees_original = degrees;
	f32q f360 = f32q_set_all(360.0f);
	f32q f0 = f32q_zero();

	//Surpasses 360 degrees
	f32q ones = f32q_set_all(1.0f);
	f32q angle_fill_percent = degrees / f360;

	f32q underflows = degrees < f0;
	f32q overflows = degrees >= f360;
	f32q optional_add_one_if_underflow = angle_fill_percent * (underflows & ones);
	f32q optional_sub_one_if_overflow = angle_fill_percent * (overflows & ones);

}