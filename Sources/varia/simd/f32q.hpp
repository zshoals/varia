#pragma once

#include "varia/vcommon.hpp"
#include "kinc/simd/float32x4.h"

static constexpr double PI = 3.145926545;

typedef kinc_float32x4_t f32q;
typedef kinc_float32x4_mask_t f32q_mask;

static inline f32q f32q_load(const f32 *values) { return kinc_float32x4_intrin_load(values); }
static inline void f32q_store(f32 *destination, f32q n) { kinc_float32x4_store(destination, n); } 
static inline f32q f32q_set(f32 a, f32 b, f32 c, f32 d) { return kinc_float32x4_load(a, b, c, d); }
static inline f32q f32q_set_all(f32 n) { return kinc_float32x4_load_all(n); }
static inline f32 f32q_get(f32q n, int index) { return kinc_float32x4_get(n, index); }
static inline f32q f32q_abs(f32q n) { return kinc_float32x4_abs(n); }
static inline f32q f32q_add(f32q a, f32q b) { return kinc_float32x4_add(a, b); }
static inline f32q f32q_sub(f32q a, f32q b) { return kinc_float32x4_sub(a, b); }
static inline f32q f32q_mul(f32q a, f32q b) { return kinc_float32x4_mul(a, b); }
static inline f32q f32q_div(f32q a, f32q b) { return kinc_float32x4_div(a, b); }
static inline f32q f32q_max(f32q a, f32q b) { return kinc_float32x4_max(a, b); }
static inline f32q f32q_min(f32q a, f32q b) { return kinc_float32x4_min(a, b); }
static inline f32q f32q_neg(f32q n) { return kinc_float32x4_neg(n); }
static inline f32q f32q_sqrt(f32q n) { return kinc_float32x4_sqrt(n); }
static inline f32q f32q_reciprocal_approximation(f32q n) { return kinc_float32x4_reciprocal_approximation(n); }
static inline f32q f32q_reciprocal_sqrt_approximation(f32q n) { return kinc_float32x4_reciprocal_sqrt_approximation(n); }
static inline f32q_mask f32q_cmpeq(f32q a, f32q b) { return kinc_float32x4_cmpeq(a, b); }
static inline f32q_mask f32q_cmpneq(f32q a, f32q b) { return kinc_float32x4_cmpneq(a, b); }
static inline f32q_mask f32q_cmpgt(f32q a, f32q b) { return kinc_float32x4_cmpgt(a, b); }
static inline f32q_mask f32q_cmpgte(f32q a, f32q b) { return kinc_float32x4_cmpge(a, b); }
static inline f32q_mask f32q_cmplt(f32q a, f32q b) { return kinc_float32x4_cmplt(a, b); }
static inline f32q_mask f32q_cmplte(f32q a, f32q b) { return kinc_float32x4_cmple(a, b); }
static inline f32q f32q_select(f32q a, f32q b, f32q_mask mask) { return kinc_float32x4_sel(a, b, mask); } 

//Operator Overloads
static inline f32q operator+(f32q const & a, f32q const & b) { return f32q_add(a, b); }
static inline f32q operator-(f32q const & a, f32q const & b) { return f32q_sub(a, b); }
static inline f32q operator*(f32q const & a, f32q const & b) { return f32q_mul(a, b); }
static inline f32q operator/(f32q const & a, f32q const & b) { return f32q_div(a, b); }
static inline f32q operator-(f32q const & a) { return f32q_neg(a); }
static inline void operator+=(f32q & a, f32q const & b) { a = f32q_add(a, b); }
static inline void operator-=(f32q & a, f32q const & b) { a = f32q_sub(a, b); }
static inline void operator*=(f32q & a, f32q const & b) { a = f32q_mul(a, b); }
static inline void operator/=(f32q & a, f32q const & b) { a = f32q_div(a, b); }
static inline f32q_mask operator>(f32q const & a, f32q const & b) { return f32q_cmpgt(a, b); }
static inline f32q_mask operator>=(f32q const & a, f32q const & b) { return f32q_cmpgte(a, b); }
static inline f32q_mask operator<(f32q const & a, f32q const & b) { return f32q_cmplt(a, b); }
static inline f32q_mask operator<=(f32q const & a, f32q const & b) { return f32q_cmplte(a, b); }
static inline f32q_mask operator==(f32q const & a, f32q const & b) { return f32q_cmpeq(a, b); }
static inline f32q_mask operator!=(f32q const & a, f32q const & b) { return f32q_cmpneq(a, b); }

//Extensions
static inline f32q f32q_zero(void) { return f32q_set_all(0.0f); }
static inline f32q_mask f32q_ones_mask_lane0(void) { 
	return f32q_cmplt(f32q_set(0.0f, 1.0f, 1.0f, 1.0f), f32q_set(1.0f, 0.0f, 0.0f, 0.0f));
}
static inline f32q_mask f32q_ones_mask_lane1(void) { 
	return f32q_cmplt(f32q_set(1.0f, 0.0f, 1.0f, 1.0f), f32q_set(0.0f, 1.0f, 0.0f, 0.0f));
}
static inline f32q_mask f32q_ones_mask_lane2(void) { 
	return f32q_cmplt(f32q_set(1.0f, 1.0f, 0.0f, 1.0f), f32q_set(0.0f, 0.0f, 1.0f, 0.0f));
}
static inline f32q_mask f32q_ones_mask_lane3(void) { 
	return f32q_cmplt(f32q_set(1.0f, 1.0f, 1.0f, 0.0f), f32q_set(0.0f, 0.0f, 0.0f, 1.0f));
}
static inline f32q_mask f32q_mask_zeroes(void) { return f32q_set_all(0.0f); }
static inline f32q_mask f32q_mask_ones(void) 
{ 
	f32q zeroes = f32q_mask_zeroes();
	return f32q_cmpeq(zeroes, zeroes); 
}



static inline f32q f32q_pow(f32q n, int exp)
{
	for (int i = 0; i < exp; ++i)
	{
		n = f32q_mul(n, n);
	}

	return n;
}

static inline f32q f32q_rad2deg(f32q radians)
{
	return (radians * (f32q_set_all(180.0f) / f32q_set_all(PI)));
}

static inline f32q f32q_deg2rad(f32q degrees)
{
	return (degrees * (f32q_set_all(PI) / f32q_set_all(180.0f)));
}

static inline f32q f32q_lerp(f32q n, f32q starts, f32q ends)
{
	return (starts * (f32q_set_all(1.0f) - n)) + (ends * n);
}

static inline f32q f32q_linear_norm(f32q n, f32q starts, f32q ends)
{
	return (n - starts) / (ends - starts);
}

static inline f32q f32q_remap(f32q n, f32q starts1, f32q ends1, f32q starts2, f32q ends2)
{
	return f32q_linear_norm(f32q_lerp(n, starts1, ends1), starts2, ends2);
}

static inline f32q f32q_clamp(f32q n, f32q lowers, f32q uppers)
{
	return f32q_min(lowers, f32q_max(n, uppers));
}

static inline f32q f32q_smoothstep(f32q n, f32q lower_edges, f32q upper_edges)
{
	n = f32q_clamp(n, lower_edges, upper_edges);
	n = f32q_linear_norm(n, lower_edges, upper_edges);
	//Smoothstep approx = [[ x = 3x^2 - 2x^3]]
	return f32q_sub(f32q_mul(f32q_set_all(3.0f), f32q_pow(n, 2)), f32q_mul(f32q_set_all(2.0f), f32q_pow(n, 3)));
}