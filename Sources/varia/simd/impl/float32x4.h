#pragma once

#include "varia/Vcommon.hpp"
#include "types.h"

#include <string.h>

/*! \file float32x4.h
    \brief Provides 128bit four-element floating point SIMD operations which are mapped to equivalent SSE or Neon operations.
*/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(KINC_SSE)

VARIA_INLINE varia_float32x4_t varia_float32x4_intrin_load(const float *values) {
	return _mm_load_ps(values);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_intrin_load_unaligned(const float *values) {
	return _mm_loadu_ps(values);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_load(float a, float b, float c, float d) {
	return _mm_set_ps(d, c, b, a);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_load_all(float t) {
	return _mm_set_ps1(t);
}

VARIA_INLINE void varia_float32x4_store(float *destination, varia_float32x4_t value) {
	_mm_store_ps(destination, value);
}

VARIA_INLINE void varia_float32x4_store_unaligned(float *destination, varia_float32x4_t value) {
	_mm_storeu_ps(destination, value);
}

VARIA_INLINE float varia_float32x4_get(varia_float32x4_t t, int index) {
	union {
		__m128 value;
		float elements[4];
	} converter;
	converter.value = t;
	return converter.elements[index];
}

VARIA_INLINE varia_float32x4_t varia_float32x4_abs(varia_float32x4_t t) {
	__m128 mask = _mm_set_ps1(-0.f);
	return _mm_andnot_ps(mask, t);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_add(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_add_ps(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_div(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_div_ps(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_mul(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_mul_ps(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_neg(varia_float32x4_t t) {
	__m128 negative = _mm_set_ps1(-1.0f);
	return _mm_mul_ps(t, negative);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_reciprocal_approximation(varia_float32x4_t t) {
	return _mm_rcp_ps(t);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_reciprocal_sqrt_approximation(varia_float32x4_t t) {
	return _mm_rsqrt_ps(t);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_sub(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_sub_ps(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_sqrt(varia_float32x4_t t) {
	return _mm_sqrt_ps(t);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_max(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_max_ps(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_min(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_min_ps(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpeq(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_cmpeq_ps(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpge(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_cmpge_ps(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpgt(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_cmpgt_ps(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmple(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_cmple_ps(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmplt(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_cmplt_ps(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpneq(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_cmpneq_ps(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_sel(varia_float32x4_t a, varia_float32x4_t b, varia_float32x4_mask_t mask) {
	return _mm_xor_ps(b, _mm_and_ps(mask, _mm_xor_ps(a, b)));
}

VARIA_INLINE varia_float32x4_t varia_float32x4_or(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_or_ps(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_and(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_and_ps(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_xor(varia_float32x4_t a, varia_float32x4_t b) {
	return _mm_xor_ps(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_not(varia_float32x4_t t) {
	__m128 zeroes = _mm_setzero_ps();
	return _mm_xor_ps(t, _mm_cmpeq_ps(zeroes, zeroes));
}

#define varia_float32x4_shuffle_custom(abcd, efgh, left_1, left_2, right_1, right_2)\
	_mm_shuffle_ps((abcd), (efgh), KINC_SHUFFLE_TABLE((left_1), (left_2), (right_1), (right_2)))

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_aebf(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	//aka unpacklo aka zip1 aka interleave low
	return _mm_unpacklo_ps(abcd, efgh);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_cgdh(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	//aka unpackhi aka zip2 aka interleave high
	return _mm_unpackhi_ps(abcd, efgh);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_abef(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	//aka movelh
	return _mm_movelh_ps(abcd, efgh);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_ghcd(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	//aka movehl
	return _mm_movehl_ps(abcd, efgh);
}

VARIA_INLINE int varia_float32x4_movemask(varia_float32x4_t t)
{
	return _mm_movemask_ps(t);
}










#elif defined(KINC_NEON)

VARIA_INLINE varia_float32x4_t varia_float32x4_intrin_load(const float *values) {
	return vld1q_f32(values);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_intrin_load_unaligned(const float *values) {
	return varia_float32x4_intrin_load(values);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_load(float a, float b, float c, float d) {
	return (varia_float32x4_t){a, b, c, d};
}

VARIA_INLINE varia_float32x4_t varia_float32x4_load_all(float t) {
	return (varia_float32x4_t){t, t, t, t};
}

VARIA_INLINE void varia_float32x4_store(float *destination, varia_float32x4_t value) {
	vst1q_f32(destination, value);
}

VARIA_INLINE void varia_float32x4_store_unaligned(float *destination, varia_float32x4_t value) {
	varia_float32x4_store(destination, value);
}

VARIA_INLINE float varia_float32x4_get(varia_float32x4_t t, int index) {
	return t[index];
}

VARIA_INLINE varia_float32x4_t varia_float32x4_abs(varia_float32x4_t t) {
	return vabsq_f32(t);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_add(varia_float32x4_t a, varia_float32x4_t b) {
	return vaddq_f32(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_div(varia_float32x4_t a, varia_float32x4_t b) {
#if defined(__aarch64__)
	return vdivq_f32(a, b);
#else
	float32x4_t inv = vrecpeq_f32(b);
	float32x4_t restep = vrecpsq_f32(b, inv);
	inv = vmulq_f32(restep, inv);
	return vmulq_f32(a, inv);
#endif
}

VARIA_INLINE varia_float32x4_t varia_float32x4_mul(varia_float32x4_t a, varia_float32x4_t b) {
	return vmulq_f32(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_neg(varia_float32x4_t t) {
	return vnegq_f32(t);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_reciprocal_approximation(varia_float32x4_t t) {
	return vrecpeq_f32(t);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_reciprocal_sqrt_approximation(varia_float32x4_t t) {
	return vrsqrteq_f32(t);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_sub(varia_float32x4_t a, varia_float32x4_t b) {
	return vsubq_f32(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_sqrt(varia_float32x4_t t) {
#if defined(__aarch64__)
	return vsqrtq_f32(t);
#else
	return vmulq_f32(t, vrsqrteq_f32(t));
#endif
}

VARIA_INLINE varia_float32x4_t varia_float32x4_max(varia_float32x4_t a, varia_float32x4_t b) {
	return vmaxq_f32(a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_min(varia_float32x4_t a, varia_float32x4_t b) {
	return vminq_f32(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpeq(varia_float32x4_t a, varia_float32x4_t b) {
	return vceqq_f32(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpge(varia_float32x4_t a, varia_float32x4_t b) {
	return vcgeq_f32(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpgt(varia_float32x4_t a, varia_float32x4_t b) {
	return vcgtq_f32(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmple(varia_float32x4_t a, varia_float32x4_t b) {
	return vcleq_f32(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmplt(varia_float32x4_t a, varia_float32x4_t b) {
	return vcltq_f32(a, b);
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpneq(varia_float32x4_t a, varia_float32x4_t b) {
	return vmvnq_u32(vceqq_f32(a, b));
}

VARIA_INLINE varia_float32x4_t varia_float32x4_sel(varia_float32x4_t a, varia_float32x4_t b, varia_float32x4_mask_t mask) {
	return vbslq_f32(mask, a, b);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_or(varia_float32x4_t a, varia_float32x4_t b) {
	uint32x4_t acvt = vreinterpretq_u32_f32(a);
	uint32x4_t bcvt = vreinterpretq_u32_f32(b);

	return vreinterpretq_f32_u32(vorrq_u32(acvt, bcvt));
}

VARIA_INLINE varia_float32x4_t varia_float32x4_and(varia_float32x4_t a, varia_float32x4_t b) {
	uint32x4_t acvt = vreinterpretq_u32_f32(a);
	uint32x4_t bcvt = vreinterpretq_u32_f32(b);

	return vreinterpretq_f32_u32(vandq_u32(acvt, bcvt));
}

VARIA_INLINE varia_float32x4_t varia_float32x4_xor(varia_float32x4_t a, varia_float32x4_t b) {
	uint32x4_t acvt = vreinterpretq_u32_f32(a);
	uint32x4_t bcvt = vreinterpretq_u32_f32(b);

	return vreinterpretq_f32_u32(veorq_u32(acvt, bcvt));
}

VARIA_INLINE varia_float32x4_t varia_float32x4_not(varia_float32x4_t t) {
	uint32x4_t tcvt = vreinterpretq_u32_f32(t);

	return vreinterpretq_f32_u32(vmvnq_u32(tcvt));
}

#define varia_float32x4_shuffle_custom(abcd, efgh, left_1, left_2, right_1, right_2)\
	(varia_float32x4_t){\
		vgetq_lane_f32((abcd), ((left_1) & 0x3)),\
		vgetq_lane_f32((abcd), ((left_2) & 0x3)),\
		vgetq_lane_f32((efgh), ((right_1) & 0x3)),\
		vgetq_lane_f32((efgh), ((right_2) & 0x3))\
	}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_aebf(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	#if defined(__aarch64__)

	return vzip1q_f32(abcd, efgh);

	#else

	float a = vgetq_lane_f32(abcd, 0);
	float b = vgetq_lane_f32(abcd, 1);
	float e = vgetq_lane_f32(efgh, 0);
	float f = vgetq_lane_f32(efgh, 1);

	return (varia_float32x4_t){a, e, b, f};

	#endif
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_cgdh(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	#if defined(__aarch64__)

	return vzip2q_f32(abcd, efgh);

	#else

	float c = vgetq_lane_f32(abcd, 2);
	float d = vgetq_lane_f32(abcd, 3);
	float g = vgetq_lane_f32(efgh, 2);
	float h = vgetq_lane_f32(efgh, 3);

	return (varia_float32x4_t){c, g, d, h};

	#endif
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_abef(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	float32x2_t ab = vget_low_f32(abcd);
	float32x2_t ef = vget_low_f32(efgh);

	return vcombine_f32(ab, ef);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_ghcd(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	float32x2_t cd = vget_high_f32(abcd);
	float32x2_t gh = vget_high_f32(efgh);

	return vcombine_f32(gh, cd);
}

VARIA_INLINE int varia_float32x4_movemask(varia_float32x4_t t)
{
	float extracted[4];
	varia_float32x4_store_unaligned(&extracted[0], t);

	uint32_t cvtU[4];
	memcpy(&cvtU[0], &extracted[0], sizeof(t));

	uint32_t out;
	int32_t outreal;

	out |= (cvtU[0] & 0x80000000) >> 31;
	out |= (cvtU[1] & 0x80000000) >> 30;
	out |= (cvtU[2] & 0x80000000) >> 29;
	out |= (cvtU[3] & 0x80000000) >> 28;

	memcpy(&outreal, &out, sizeof(outreal));

	return outreal;
}













#else

#include <math.h>

VARIA_INLINE varia_float32x4_t varia_float32x4_intrin_load(const float *values) {
	varia_float32x4_t value;
	value.values[0] = values[0];
	value.values[1] = values[1];
	value.values[2] = values[2];
	value.values[3] = values[3];
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_intrin_load_unaligned(const float *values) {
	return varia_float32x4_intrin_load(values);
}

VARIA_INLINE varia_float32x4_t varia_float32x4_load(float a, float b, float c, float d) {
	varia_float32x4_t value;
	value.values[0] = a;
	value.values[1] = b;
	value.values[2] = c;
	value.values[3] = d;
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_load_all(float t) {
	varia_float32x4_t value;
	value.values[0] = t;
	value.values[1] = t;
	value.values[2] = t;
	value.values[3] = t;
	return value;
}

VARIA_INLINE void varia_float32x4_store(float *destination, varia_float32x4_t value) {
	destination[0] = value.values[0];
	destination[1] = value.values[1];
	destination[2] = value.values[2];
	destination[3] = value.values[3];
}

VARIA_INLINE void varia_float32x4_store_unaligned(float *destination, varia_float32x4_t value) {
	varia_float32x4_store(destination, value);
}

VARIA_INLINE float varia_float32x4_get(varia_float32x4_t t, int index) {
	return t.values[index];
}

VARIA_INLINE varia_float32x4_t varia_float32x4_abs(varia_float32x4_t t) {
	varia_float32x4_t value;
	value.values[0] = kinc_abs(t.values[0]);
	value.values[1] = kinc_abs(t.values[1]);
	value.values[2] = kinc_abs(t.values[2]);
	value.values[3] = kinc_abs(t.values[3]);
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_add(varia_float32x4_t a, varia_float32x4_t b) {
	varia_float32x4_t value;
	value.values[0] = a.values[0] + b.values[0];
	value.values[1] = a.values[1] + b.values[1];
	value.values[2] = a.values[2] + b.values[2];
	value.values[3] = a.values[3] + b.values[3];
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_div(varia_float32x4_t a, varia_float32x4_t b) {
	varia_float32x4_t value;
	value.values[0] = a.values[0] / b.values[0];
	value.values[1] = a.values[1] / b.values[1];
	value.values[2] = a.values[2] / b.values[2];
	value.values[3] = a.values[3] / b.values[3];
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_mul(varia_float32x4_t a, varia_float32x4_t b) {
	varia_float32x4_t value;
	value.values[0] = a.values[0] * b.values[0];
	value.values[1] = a.values[1] * b.values[1];
	value.values[2] = a.values[2] * b.values[2];
	value.values[3] = a.values[3] * b.values[3];
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_neg(varia_float32x4_t t) {
	varia_float32x4_t value;
	value.values[0] = -t.values[0];
	value.values[1] = -t.values[1];
	value.values[2] = -t.values[2];
	value.values[3] = -t.values[3];
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_reciprocal_approximation(varia_float32x4_t t) {
	varia_float32x4_t value;
	value.values[0] = 1.0f / t.values[0];
	value.values[1] = 1.0f / t.values[1];
	value.values[2] = 1.0f / t.values[2];
	value.values[3] = 1.0f / t.values[3];
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_reciprocal_sqrt_approximation(varia_float32x4_t t) {
	varia_float32x4_t value;
	value.values[0] = 1.0f / sqrtf(t.values[0]);
	value.values[1] = 1.0f / sqrtf(t.values[1]);
	value.values[2] = 1.0f / sqrtf(t.values[2]);
	value.values[3] = 1.0f / sqrtf(t.values[3]);
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_sub(varia_float32x4_t a, varia_float32x4_t b) {
	varia_float32x4_t value;
	value.values[0] = a.values[0] - b.values[0];
	value.values[1] = a.values[1] - b.values[1];
	value.values[2] = a.values[2] - b.values[2];
	value.values[3] = a.values[3] - b.values[3];
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_sqrt(varia_float32x4_t t) {
	varia_float32x4_t value;
	value.values[0] = sqrtf(t.values[0]);
	value.values[1] = sqrtf(t.values[1]);
	value.values[2] = sqrtf(t.values[2]);
	value.values[3] = sqrtf(t.values[3]);
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_max(varia_float32x4_t a, varia_float32x4_t b) {
	varia_float32x4_t value;
	value.values[0] = kinc_max(a.values[0], b.values[0]);
	value.values[1] = kinc_max(a.values[1], b.values[1]);
	value.values[2] = kinc_max(a.values[2], b.values[2]);
	value.values[3] = kinc_max(a.values[3], b.values[3]);
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_min(varia_float32x4_t a, varia_float32x4_t b) {
	varia_float32x4_t value;
	value.values[0] = kinc_min(a.values[0], b.values[0]);
	value.values[1] = kinc_min(a.values[1], b.values[1]);
	value.values[2] = kinc_min(a.values[2], b.values[2]);
	value.values[3] = kinc_min(a.values[3], b.values[3]);
	return value;
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpeq(varia_float32x4_t a, varia_float32x4_t b) {
	uint32_t mask_cvt[4];
	mask_cvt[0] = a.values[0] == b.values[0] ? 0xffffffff : 0;
	mask_cvt[1] = a.values[1] == b.values[1] ? 0xffffffff : 0;
	mask_cvt[2] = a.values[2] == b.values[2] ? 0xffffffff : 0;
	mask_cvt[3] = a.values[3] == b.values[3] ? 0xffffffff : 0;

	varia_float32x4_mask_t mask;
	memcpy(&mask.values[0], &mask_cvt[0], sizeof(mask_cvt));

	return mask;
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpge(varia_float32x4_t a, varia_float32x4_t b) {
	uint32_t mask_cvt[4];
	mask_cvt[0] = a.values[0] >= b.values[0] ? 0xffffffff : 0;
	mask_cvt[1] = a.values[1] >= b.values[1] ? 0xffffffff : 0;
	mask_cvt[2] = a.values[2] >= b.values[2] ? 0xffffffff : 0;
	mask_cvt[3] = a.values[3] >= b.values[3] ? 0xffffffff : 0;

	varia_float32x4_mask_t mask;
	memcpy(&mask.values[0], &mask_cvt[0], sizeof(mask_cvt));

	return mask;
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpgt(varia_float32x4_t a, varia_float32x4_t b) {
	uint32_t mask_cvt[4];
	mask_cvt[0] = a.values[0] > b.values[0] ? 0xffffffff : 0;
	mask_cvt[1] = a.values[1] > b.values[1] ? 0xffffffff : 0;
	mask_cvt[2] = a.values[2] > b.values[2] ? 0xffffffff : 0;
	mask_cvt[3] = a.values[3] > b.values[3] ? 0xffffffff : 0;

	varia_float32x4_mask_t mask;
	memcpy(&mask.values[0], &mask_cvt[0], sizeof(mask_cvt));

	return mask;
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmple(varia_float32x4_t a, varia_float32x4_t b) {
	uint32_t mask_cvt[4];
	mask_cvt[0] = a.values[0] <= b.values[0] ? 0xffffffff : 0;
	mask_cvt[1] = a.values[1] <= b.values[1] ? 0xffffffff : 0;
	mask_cvt[2] = a.values[2] <= b.values[2] ? 0xffffffff : 0;
	mask_cvt[3] = a.values[3] <= b.values[3] ? 0xffffffff : 0;

	varia_float32x4_mask_t mask;
	memcpy(&mask.values[0], &mask_cvt[0], sizeof(mask_cvt));

	return mask;
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmplt(varia_float32x4_t a, varia_float32x4_t b) {
	uint32_t mask_cvt[4];
	mask_cvt[0] = a.values[0] < b.values[0] ? 0xffffffff : 0;
	mask_cvt[1] = a.values[1] < b.values[1] ? 0xffffffff : 0;
	mask_cvt[2] = a.values[2] < b.values[2] ? 0xffffffff : 0;
	mask_cvt[3] = a.values[3] < b.values[3] ? 0xffffffff : 0;

	varia_float32x4_mask_t mask;
	memcpy(&mask.values[0], &mask_cvt[0], sizeof(mask_cvt));

	return mask;
}

VARIA_INLINE varia_float32x4_mask_t varia_float32x4_cmpneq(varia_float32x4_t a, varia_float32x4_t b) {
	uint32_t mask_cvt[4];
	mask_cvt[0] = a.values[0] != b.values[0] ? 0xffffffff : 0;
	mask_cvt[1] = a.values[1] != b.values[1] ? 0xffffffff : 0;
	mask_cvt[2] = a.values[2] != b.values[2] ? 0xffffffff : 0;
	mask_cvt[3] = a.values[3] != b.values[3] ? 0xffffffff : 0;

	varia_float32x4_mask_t mask;
	memcpy(&mask.values[0], &mask_cvt[0], sizeof(mask_cvt));

	return mask;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_sel(varia_float32x4_t a, varia_float32x4_t b, varia_float32x4_mask_t mask) {
	varia_float32x4_t value;
	value.values[0] = mask.values[0] != 0.0f ? a.values[0] : b.values[0];
	value.values[1] = mask.values[1] != 0.0f ? a.values[1] : b.values[1];
	value.values[2] = mask.values[2] != 0.0f ? a.values[2] : b.values[2];
	value.values[3] = mask.values[3] != 0.0f ? a.values[3] : b.values[3];
	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_or(varia_float32x4_t a, varia_float32x4_t b) {
	uint32_t acvt[4];
	uint32_t bcvt[4];
	memcpy(&acvt[0], &a.values[0], sizeof(a));
	memcpy(&bcvt[0], &b.values[0], sizeof(b));

	acvt[0] |= bcvt[0];
	acvt[1] |= bcvt[1];
	acvt[2] |= bcvt[2];
	acvt[3] |= bcvt[3];

	varia_float32x4_t value; 
	memcpy(&value.values[0], &acvt[0], sizeof(acvt));

	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_and(varia_float32x4_t a, varia_float32x4_t b) {
	uint32_t acvt[4];
	uint32_t bcvt[4];
	memcpy(&acvt[0], &a.values[0], sizeof(a));
	memcpy(&bcvt[0], &b.values[0], sizeof(b));

	acvt[0] &= bcvt[0];
	acvt[1] &= bcvt[1];
	acvt[2] &= bcvt[2];
	acvt[3] &= bcvt[3];

	varia_float32x4_t value; 
	memcpy(&value.values[0], &acvt[0], sizeof(acvt));

	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_xor(varia_float32x4_t a, varia_float32x4_t b) {
	uint32_t acvt[4];
	uint32_t bcvt[4];
	memcpy(&acvt[0], &a.values[0], sizeof(a));
	memcpy(&bcvt[0], &b.values[0], sizeof(b));

	acvt[0] ^= bcvt[0];
	acvt[1] ^= bcvt[1];
	acvt[2] ^= bcvt[2];
	acvt[3] ^= bcvt[3];

	varia_float32x4_t value; 
	memcpy(&value.values[0], &acvt[0], sizeof(acvt));

	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_not(varia_float32x4_t t) {
	uint32_t tcvt[4];
	memcpy(&tcvt[0], &t.values[0], sizeof(t));

	tcvt[0] = ~tcvt[0];
	tcvt[1] = ~tcvt[1];
	tcvt[2] = ~tcvt[2];
	tcvt[3] = ~tcvt[3];

	varia_float32x4_t value; 
	memcpy(&value.values[0], &tcvt[0], sizeof(tcvt));

	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_custom(varia_float32x4_t abcd, varia_float32x4_t efgh, 
	const uint32_t left_1, const uint32_t left_2, const uint32_t right_1, const uint32_t right_2) {
	varia_float32x4_t value;

	value.values[0] = abcd.values[left_1 & 0x3];
	value.values[1] = abcd.values[left_2 & 0x3];
	value.values[2] = efgh.values[right_1 & 0x3];
	value.values[3] = efgh.values[right_2 & 0x3];

	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_aebf(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	varia_float32x4_t value;

	value.values[0] = abcd.values[0];
	value.values[1] = efgh.values[0];
	value.values[2] = abcd.values[1];
	value.values[3] = efgh.values[1];

	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_cgdh(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	varia_float32x4_t value;

	value.values[0] = abcd.values[2];
	value.values[1] = efgh.values[2];
	value.values[2] = abcd.values[3];
	value.values[3] = efgh.values[3];

	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_abef(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	varia_float32x4_t value;

	value.values[0] = abcd.values[0];
	value.values[1] = abcd.values[1];
	value.values[2] = efgh.values[0];
	value.values[3] = efgh.values[1];

	return value;
}

VARIA_INLINE varia_float32x4_t varia_float32x4_shuffle_ghcd(varia_float32x4_t abcd, varia_float32x4_t efgh) {
	varia_float32x4_t value;

	value.values[0] = efgh.values[2];
	value.values[1] = efgh.values[3];
	value.values[2] = abcd.values[2];
	value.values[3] = abcd.values[3];

	return value;
}

VARIA_INLINE int varia_float32x4_movemask(varia_float32x4_t t)
{
	float extracted[4];
	varia_float32x4_store_unaligned(&extracted[0], t);

	uint32_t cvtU[4];
	memcpy(&cvtU[0], &extracted[0], sizeof(t));

	uint32_t out;
	int32_t outreal;

	out |= (cvtU[0] & 0x80000000) >> 31;
	out |= (cvtU[1] & 0x80000000) >> 30;
	out |= (cvtU[2] & 0x80000000) >> 29;
	out |= (cvtU[3] & 0x80000000) >> 28;

	memcpy(&outreal, &out, sizeof(outreal));

	return outreal;
}



#endif

#ifdef __cplusplus
}
#endif
