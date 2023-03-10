#pragma once

#include "varia/vcommon.hpp"
#include "types.h"

/*! \file uint32x4.h
    \brief Provides 128bit four-element unsigned 32-bit integer SIMD operations which are mapped to equivalent SSE2 or Neon operations.
*/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(KINC_SSE2)

VARIA_INLINE varia_uint32x4_t varia_uint32x4_intrin_load(const uint32_t *values) {
	return _mm_load_si128((const varia_uint32x4_t *)values);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_intrin_load_unaligned(const uint32_t *values) {
	return _mm_loadu_si128((const varia_uint32x4_t *)values);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_load(const uint32_t values[4]) {
	return _mm_set_epi32(values[3], values[2], values[1], values[0]);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_load_all(uint32_t t) {
	return _mm_set1_epi32(t);
}

VARIA_INLINE void varia_uint32x4_store(uint32_t *destination, varia_uint32x4_t value) {
	_mm_store_si128((varia_uint32x4_t *)destination, value);
}

VARIA_INLINE void varia_uint32x4_store_unaligned(uint32_t *destination, varia_uint32x4_t value) {
	_mm_storeu_si128((varia_uint32x4_t *)destination, value);
}

VARIA_INLINE uint32_t varia_uint32x4_get(varia_uint32x4_t t, int index) {
	union {
		__m128i value;
		uint32_t elements[4];
	} converter;
	converter.value = t;
	return converter.elements[index];
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_add(varia_uint32x4_t a, varia_uint32x4_t b) {
	return _mm_add_epi32(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_sub(varia_uint32x4_t a, varia_uint32x4_t b) {
	return _mm_sub_epi32(a, b);
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpeq(varia_uint32x4_t a, varia_uint32x4_t b) {
	return _mm_cmpeq_epi32(a, b);
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpneq(varia_uint32x4_t a, varia_uint32x4_t b) {
	return _mm_andnot_si128(_mm_cmpeq_epi32(a, b), _mm_set1_epi32(0xffffffff));
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpge(varia_uint32x4_t a, varia_uint32x4_t b) {
	__m128i bias_by = _mm_set1_epi32((uint32_t)0x80000000);
	return _mm_or_si128(_mm_cmpgt_epi32(_mm_sub_epi32(a, bias_by), _mm_sub_epi32(b, bias_by)), _mm_cmpeq_epi32(a, b));
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpgt(varia_uint32x4_t a, varia_uint32x4_t b) {
	__m128i bias_by = _mm_set1_epi32((uint32_t)0x80000000);
	return _mm_cmpgt_epi32(_mm_sub_epi32(a, bias_by), _mm_sub_epi32(b, bias_by));
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmple(varia_uint32x4_t a, varia_uint32x4_t b) {
	__m128i bias_by = _mm_set1_epi32((uint32_t)0x80000000);
	return _mm_or_si128(_mm_cmplt_epi32(_mm_sub_epi32(a, bias_by), _mm_sub_epi32(b, bias_by)), _mm_cmpeq_epi32(a, b));
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmplt(varia_uint32x4_t a, varia_uint32x4_t b) {
	__m128i bias_by = _mm_set1_epi32((uint32_t)0x80000000);
	return _mm_cmplt_epi32(_mm_sub_epi32(a, bias_by), _mm_sub_epi32(b, bias_by));
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_sel(varia_uint32x4_t a, varia_uint32x4_t b, varia_uint32x4_mask_t mask) {
	return _mm_xor_si128(b, _mm_and_si128(mask, _mm_xor_si128(a, b)));
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_max(varia_uint32x4_t a, varia_uint32x4_t b) {
	return varia_uint32x4_sel(a, b, varia_uint32x4_cmpgt(a, b));
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_min(varia_uint32x4_t a, varia_uint32x4_t b) {
	return varia_uint32x4_sel(a, b, varia_uint32x4_cmplt(a, b));
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_or(varia_uint32x4_t a, varia_uint32x4_t b) {
	return _mm_or_si128(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_and(varia_uint32x4_t a, varia_uint32x4_t b) {
	return _mm_and_si128(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_xor(varia_uint32x4_t a, varia_uint32x4_t b) {
	return _mm_xor_si128(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_not(varia_uint32x4_t t) {
	return _mm_xor_si128(t, _mm_set1_epi32(0xffffffff));
}

#define varia_uint32x4_shift_left(t, shift)\
	_mm_slli_epi32((t), (shift))

#define varia_uint32x4_shift_right(t, shift)\
	_mm_srli_epi32((t), (shift))


#elif defined(KINC_NEON)

VARIA_INLINE varia_uint32x4_t varia_uint32x4_intrin_load(const uint32_t *values) {
	return vld1q_u32(values);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_intrin_load_unaligned(const uint32_t *values) {
	return varia_uint32x4_intrin_load(values);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_load(const uint32_t values[4]) {
	return (varia_uint32x4_t){values[0], values[1], values[2], values[3]};
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_load_all(uint32_t t) {
	return (varia_uint32x4_t){t, t, t, t};
}

VARIA_INLINE void varia_uint32x4_store(uint32_t *destination, varia_uint32x4_t value) {
	vst1q_u32(destination, value);
}

VARIA_INLINE void varia_uint32x4_store_unaligned(uint32_t *destination, varia_uint32x4_t value) {
	varia_uint32x4_store(destination, value);
}

VARIA_INLINE uint32_t varia_uint32x4_get(varia_uint32x4_t t, int index) {
	return t[index];
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_add(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vaddq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_sub(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vsubq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_max(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vmaxq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_min(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vminq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpeq(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vceqq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpneq(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vmvnq_u32(vceqq_u32(a, b));
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpge(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vcgeq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpgt(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vcgtq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmple(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vcleq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmplt(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vcltq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_sel(varia_uint32x4_t a, varia_uint32x4_t b, varia_uint32x4_mask_t mask) {
	return vbslq_u32(mask, a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_or(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vorrq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_and(varia_uint32x4_t a, varia_uint32x4_t b) {
	return vandq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_xor(varia_uint32x4_t a, varia_uint32x4_t b) {
	return veorq_u32(a, b);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_not(varia_uint32x4_t t) {
	return vmvnq_u32(t);
}

#define varia_uint32x4_shift_left(t, shift)\
	return vshlq_n_u32((t), (shift))

#define varia_uint32x4_shift_right(t, shift)\
	return vshrq_n_u32((t), (shift))


#else

VARIA_INLINE varia_uint32x4_t varia_uint32x4_intrin_load(const uint32_t *values) {
	varia_uint32x4_t value;
	value.values[0] = values[0];
	value.values[1] = values[1];
	value.values[2] = values[2];
	value.values[3] = values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_intrin_load_unaligned(const uint32_t *values) {
	return varia_uint32x4_intrin_load(values);
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_load(const uint32_t values[4]) {
	varia_uint32x4_t value;
	value.values[0] = values[0];
	value.values[1] = values[1];
	value.values[2] = values[2];
	value.values[3] = values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_load_all(uint32_t t) {
	varia_uint32x4_t value;
	value.values[0] = t;
	value.values[1] = t;
	value.values[2] = t;
	value.values[3] = t;
	return value;
}

VARIA_INLINE void varia_uint32x4_store(uint32_t *destination, varia_uint32x4_t value) {
	destination[0] = value.values[0];
	destination[1] = value.values[1];
	destination[2] = value.values[2];
	destination[3] = value.values[3];
}

VARIA_INLINE void varia_uint32x4_store_unaligned(uint32_t *destination, varia_uint32x4_t value) {
	varia_uint32x4_store(destination, value);
}

VARIA_INLINE uint32_t varia_uint32x4_get(varia_uint32x4_t t, int index) {
	return t.values[index];
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_add(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_t value;
	value.values[0] = a.values[0] + b.values[0];
	value.values[1] = a.values[1] + b.values[1];
	value.values[2] = a.values[2] + b.values[2];
	value.values[3] = a.values[3] + b.values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_sub(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_t value;
	value.values[0] = a.values[0] - b.values[0];
	value.values[1] = a.values[1] - b.values[1];
	value.values[2] = a.values[2] - b.values[2];
	value.values[3] = a.values[3] - b.values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_max(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_t value;
	value.values[0] = a.values[0] > b.values[0] ? a.values[0] : b.values[0];
	value.values[1] = a.values[1] > b.values[1] ? a.values[1] : b.values[1];
	value.values[2] = a.values[2] > b.values[2] ? a.values[2] : b.values[2];
	value.values[3] = a.values[3] > b.values[3] ? a.values[3] : b.values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_min(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_t value;
	value.values[0] = a.values[0] > b.values[0] ? b.values[0] : a.values[0];
	value.values[1] = a.values[1] > b.values[1] ? b.values[1] : a.values[1];
	value.values[2] = a.values[2] > b.values[2] ? b.values[2] : a.values[2];
	value.values[3] = a.values[3] > b.values[3] ? b.values[3] : a.values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpeq(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_mask_t mask;
	mask.values[0] = a.values[0] == b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] == b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] == b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] == b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpneq(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_mask_t mask;
	mask.values[0] = a.values[0] != b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] != b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] != b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] != b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpge(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_mask_t mask;
	mask.values[0] = a.values[0] >= b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] >= b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] >= b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] >= b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmpgt(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_mask_t mask;
	mask.values[0] = a.values[0] > b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] > b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] > b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] > b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmple(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_mask_t mask;
	mask.values[0] = a.values[0] <= b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] <= b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] <= b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] <= b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_uint32x4_mask_t varia_uint32x4_cmplt(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_mask_t mask;
	mask.values[0] = a.values[0] < b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] < b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] < b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] < b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_sel(varia_uint32x4_t a, varia_uint32x4_t b, varia_uint32x4_mask_t mask) {
	varia_uint32x4_t value;
	value.values[0] = mask.values[0] != 0 ? a.values[0] : b.values[0];
	value.values[1] = mask.values[1] != 0 ? a.values[1] : b.values[1];
	value.values[2] = mask.values[2] != 0 ? a.values[2] : b.values[2];
	value.values[3] = mask.values[3] != 0 ? a.values[3] : b.values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_or(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_t value;
	value.values[0] = a.values[0] | b.values[0];
	value.values[1] = a.values[1] | b.values[1];
	value.values[2] = a.values[2] | b.values[2];
	value.values[3] = a.values[3] | b.values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_and(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_t value;
	value.values[0] = a.values[0] & b.values[0];
	value.values[1] = a.values[1] & b.values[1];
	value.values[2] = a.values[2] & b.values[2];
	value.values[3] = a.values[3] & b.values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_xor(varia_uint32x4_t a, varia_uint32x4_t b) {
	varia_uint32x4_t value;
	value.values[0] = a.values[0] ^ b.values[0];
	value.values[1] = a.values[1] ^ b.values[1];
	value.values[2] = a.values[2] ^ b.values[2];
	value.values[3] = a.values[3] ^ b.values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_not(varia_uint32x4_t t) {
	varia_uint32x4_t value;
	value.values[0] = ~t.values[0];
	value.values[1] = ~t.values[1];
	value.values[2] = ~t.values[2];
	value.values[3] = ~t.values[3];
	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_shift_left(varia_uint32x4_t t, const int shift) {
	varia_uint32x4_t value;
	value.values[0] = t.values[0] << shift;
	value.values[1] = t.values[1] << shift;
	value.values[2] = t.values[2] << shift;
	value.values[3] = t.values[3] << shift;

	return value;
}

VARIA_INLINE varia_uint32x4_t varia_uint32x4_shift_right(varia_uint32x4_t t, const int shift) {
	varia_uint32x4_t value;
	value.values[0] = t.values[0] >> shift;
	value.values[1] = t.values[1] >> shift;
	value.values[2] = t.values[2] >> shift;
	value.values[3] = t.values[3] >> shift;

	return value;
}

#endif

#ifdef __cplusplus
}
#endif
