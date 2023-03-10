#pragma once

#include "varia/simd/simd_types.h"

#include "varia/vcommon.hpp"
#include "varia/math/MathConstants.hpp"
#include "kinc/simd/float32x4.h"
#include "simd_trig.h"

#ifdef VARIA_LOG_LOGGING_ENABLED
	#include "varia/log.hpp"
#endif

VARIA_INLINE f32q f32q_load(const f32 *values) { return kinc_float32x4_intrin_load(values); }
VARIA_INLINE f32q f32q_load_u(const f32 *values) { return kinc_float32x4_intrin_load_unaligned(values); }
VARIA_INLINE void f32q_store(f32 *destination, f32q n) { kinc_float32x4_store(destination, n); } 
VARIA_INLINE void f32q_store_u(f32 *destination, f32q n) { kinc_float32x4_store_unaligned(destination, n); } 
VARIA_INLINE f32q f32q_set(f32 a, f32 b, f32 c, f32 d) { return kinc_float32x4_load(a, b, c, d); }
VARIA_INLINE f32q f32q_set_all(f32 n) { return kinc_float32x4_load_all(n); }

//Renamed versions of set/set_all for semantics
VARIA_INLINE f32q f32q_create(f32 a, f32 b, f32 c, f32 d) { return f32q_set(a, b, c, d); }
VARIA_INLINE f32q f32q_create_all(f32 n) { return f32q_set_all(n); }

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
VARIA_INLINE f32q f32q_one(void) { return f32q_set_all(1.0f); }
VARIA_INLINE f32q f32q_two(void) { return f32q_set_all(2.0f); }
VARIA_INLINE f32q f32q_negative_one(void) { return f32q_set_all(-1.0f); }
VARIA_INLINE f32q f32q_PI(void) { return f32q_set_all(VARIA_PI); }
VARIA_INLINE f32q f32q_TAU(void) { return f32q_set_all(VARIA_PI * 2.0f); }
VARIA_INLINE f32q f32q_EPSILON(void) { return f32q_set_all(VARIA_EPSILON); }
VARIA_INLINE f32q_mask f32q_mask_false(void) { return f32q_set_all(0.0f); }
VARIA_INLINE f32q_mask f32q_mask_true(void) 
{ 
	f32q zeroes = f32q_mask_false();
	return f32q_cmpeq(zeroes, zeroes); 
}

VARIA_INLINE f32q f32q_conditional_add(f32q a, f32q b_masked, f32q_mask condition)
{
	return a + (b_masked & condition);
}

VARIA_INLINE f32q f32q_conditional_sub(f32q a, f32q b_masked, f32q_mask condition)
{
	return a - (b_masked & condition);
}

VARIA_INLINE f32q f32q_conditional_mul(f32q a, f32q b_masked, f32q_mask condition)
{
	return f32q_select( (a * b_masked), a, condition);
}

VARIA_INLINE f32q f32q_conditional_div(f32q a, f32q b_masked, f32q_mask condition)
{
	return f32q_select( (a / b_masked), a, condition);
}

VARIA_INLINE f32q f32q_conditional_clear(f32q n, f32q_mask condition)
{
	return f32q_select(n, f32q_zero(), condition);
}

VARIA_INLINE f32q_mask f32q_condition_merge(f32q_mask condition_a, f32q_mask condition_b)
{
	return condition_a | condition_b;
}

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

VARIA_INLINE int f32q_movemask(f32q mask)
{
	#if defined(KINC_SSE2) || defined(KINC_SSE)
		return _mm_movemask_ps(mask);
	#else
		uint32_t cvt[4];
		uint32_t int_mask;
		VARIA_BITCAST(&cvt[0], &mask.values[0]);

		int_mask |= ((cvt[0] >> 31) & 0x1) << 0;
		int_mask |= ((cvt[1] >> 31) & 0x1) << 1;
		int_mask |= ((cvt[2] >> 31) & 0x1) << 2;
		int_mask |= ((cvt[3] >> 31) & 0x1) << 3;

		int out;
		VARIA_BITCAST(&out, &int_mask);

		return out;
	#endif
}

VARIA_INLINE f32q f32q_sign(f32q n)
{
	f32q_mask mask = n < f32q_zero();
	return f32q_select(f32q_negative_one(), f32q_one(), mask);
}

VARIA_INLINE f32q f32q_round(f32q n)
{
	#if defined(KINC_SSE2)
		__m128i y1 = _mm_cvtps_epi32(n);
		f32q y2 = _mm_cvtepi32_ps(y1);

		y2 |= (n & f32q_set_all(-0.0f));

		f32q overflow_mask = _mm_castsi128_ps
		(
			_mm_andnot_si128(_mm_cmpeq_epi32(y1, _mm_set1_epi32(0x80000000)), _mm_set1_epi32(0xffffffff))
		);

		return f32q_select(y2, n, overflow_mask);
	#else
		//TODO(zshoals 03-05-2023):> Won't work if we're on NEON; we need to explicitly handle NEON now :/
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

		// f32q overflow_mask = _mm_cmpneq_ps(_mm_castsi128_ps(y1), _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));
		f32q overflow_mask = _mm_castsi128_ps
		(
			_mm_andnot_si128(_mm_cmpeq_epi32(y1, _mm_set1_epi32(0x80000000)), _mm_set1_epi32(0xffffffff))
		);

		return f32q_select(y2, n, overflow_mask);
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
		y -= f32q_set_all(1.0f) & (y > n);

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

VARIA_INLINE f32q f32q_ceil(f32q n)
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

VARIA_INLINE f32q f32q_pow3(f32q n)
{
	return n * n * n;
}

VARIA_INLINE f32q f32q_rad2deg(f32q radians)
{
	float const PI = 3.145926545f;
	f32q const f_1_over_180 = (f32q_one() / f32q_set_all(PI));

	return (radians * (f32q_set_all(180.0f) * f_1_over_180));
}

VARIA_INLINE f32q f32q_deg2rad(f32q degrees)
{
	float const PI = 3.145926545f;
	f32q const f_rad_over_deg = f32q_set_all(PI) / f32q_set_all(180.0f);

	return (degrees * f_rad_over_deg);
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

VARIA_INLINE f32q f32q_wrap(f32q n, f32q limit)
{
	return n - (limit * f32q_floor(n * (f32q_set_all(1.0f) / limit) ));
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

VARIA_INLINE f32q f32q_wrap_degrees(f32q degrees)
{
	f32q const f1 = f32q_set_all(1.0f);
	f32q const f360 = f32q_set_all(360.0f);
	return degrees - (f360 * f32q_floor(degrees * (f1 / f360)));
}

VARIA_INLINE f32q f32q_wrap_radians(f32q radians)
{
	f32q const f1 = f32q_set_all(1.0f);
	f32q const fTAU = f32q_set_all(VARIA_TAU);
	return radians - (fTAU * f32q_floor(radians * (f1 / fTAU)));
}


VARIA_INLINE f32q f32q_shuffle_aebf(f32q abcd, f32q efgh)
{
	return kinc_float32x4_shuffle_aebf(abcd, efgh);
}

VARIA_INLINE f32q f32q_shuffle_cgdh(f32q abcd, f32q efgh)
{
	return kinc_float32x4_shuffle_cgdh(abcd, efgh);
}


//Debugging utilities
#ifdef VARIA_LOG_LOGGING_ENABLED

	static void f32q_Gprint(f32q n)
	{
		alignas(16) float out[4];
		f32q_store(&out[0], n);

		Glog_newline();
		Glog_string("Float32x4");
		Glog_float(out[0]); Glog_newline();
		Glog_float(out[1]); Glog_newline();
		Glog_float(out[2]); Glog_newline();
		Glog_float(out[3]); Glog_newline();
		Glog_newline();
	}

	static void f32q_Gprint_info(f32q n, char const * title_info)
	{
		alignas(16) float out[4];
		f32q_store(&out[0], n);

		Glog_newline();
		Glog_string(title_info);
		Glog_newline();
		Glog_float(out[0]); Glog_newline();
		Glog_float(out[1]); Glog_newline();
		Glog_float(out[2]); Glog_newline();
		Glog_float(out[3]); Glog_newline();
		Glog_newline();
	}

#endif