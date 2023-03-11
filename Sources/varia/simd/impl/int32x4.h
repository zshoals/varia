#pragma once

#include "varia/Vcommon.hpp"
#include "types.h"

/*! \file int32x4.h
    \brief Provides 128bit four-element signed 32-bit integer SIMD operations which are mapped to equivalent SSE2 or Neon operations.
*/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(KINC_SSE2)

VARIA_INLINE varia_int32x4_t varia_int32x4_intrin_load(const int32_t *values) {
	return _mm_load_si128((const varia_int32x4_t *)values);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_intrin_load_unaligned(const int32_t *values) {
	return _mm_loadu_si128((const varia_int32x4_t *)values);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_load(const int32_t values[4]) {
	return _mm_set_epi32(values[3], values[2], values[1], values[0]);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_load_all(int32_t t) {
	return _mm_set1_epi32(t);
}

VARIA_INLINE void varia_int32x4_store(int32_t *destination, varia_int32x4_t value) {
	_mm_store_si128((varia_int32x4_t *)destination, value);
}

VARIA_INLINE void varia_int32x4_store_unaligned(int32_t *destination, varia_int32x4_t value) {
	_mm_storeu_si128((varia_int32x4_t *)destination, value);
}

VARIA_INLINE int32_t varia_int32x4_get(varia_int32x4_t t, int index) {
	union {
		__m128i value;
		int32_t elements[4];
	} converter;
	converter.value = t;
	return converter.elements[index];
}

VARIA_INLINE varia_int32x4_t varia_int32x4_add(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_add_epi32(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_sub(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_sub_epi32(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_max(varia_int32x4_t a, varia_int32x4_t b) {
	__m128i mask = _mm_cmpgt_epi32(a, b);
	return _mm_xor_si128(b, _mm_and_si128(mask, _mm_xor_si128(a, b)));
}

VARIA_INLINE varia_int32x4_t varia_int32x4_min(varia_int32x4_t a, varia_int32x4_t b) {
	__m128i mask = _mm_cmplt_epi32(a, b);
	return _mm_xor_si128(b, _mm_and_si128(mask, _mm_xor_si128(a, b)));
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpeq(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_cmpeq_epi32(a, b);
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpge(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_or_si128(_mm_cmpgt_epi32(a, b), _mm_cmpeq_epi32(a, b));
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpgt(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_cmpgt_epi32(a, b);
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmple(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_or_si128(_mm_cmplt_epi32(a, b), _mm_cmpeq_epi32(a, b));
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmplt(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_cmplt_epi32(a, b);
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpneq(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_andnot_si128(_mm_cmpeq_epi32(a, b), _mm_set1_epi32(0xffffffff));
}

VARIA_INLINE varia_int32x4_t varia_int32x4_sel(varia_int32x4_t a, varia_int32x4_t b, varia_int32x4_mask_t mask) {
	return _mm_xor_si128(b, _mm_and_si128(mask, _mm_xor_si128(a, b)));
}

VARIA_INLINE varia_int32x4_t varia_int32x4_or(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_or_si128(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_and(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_and_si128(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_xor(varia_int32x4_t a, varia_int32x4_t b) {
	return _mm_xor_si128(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_not(varia_int32x4_t t) {
	return _mm_xor_si128(t, _mm_set1_epi32(0xffffffff));
}

#define varia_int32x4_shift_left(t, shift)\
	return _mm_slli_epi32((t), (shift))

#define varia_int32x4_shift_right(t, shift)\
	return _mm_srli_epi32((t), (shift))


#elif defined(KINC_NEON)


VARIA_INLINE varia_int32x4_t varia_int32x4_intrin_load(const int32_t *values) {
	return vld1q_s32(values);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_intrin_load_unaligned(const int32_t *values) {
	return varia_int32x4_intrin_load(values);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_load(const int32_t values[4]) {
	return (varia_int32x4_t){values[0], values[1], values[2], values[3]};
}

VARIA_INLINE varia_int32x4_t varia_int32x4_load_all(int32_t t) {
	return (varia_int32x4_t){t, t, t, t};
}

VARIA_INLINE void varia_int32x4_store(int32_t *destination, varia_int32x4_t value) {
	vst1q_s32(destination, value);
}

VARIA_INLINE void varia_int32x4_store_unaligned(int32_t *destination, varia_int32x4_t value) {
	varia_int32x4_store(destination, value);
}

VARIA_INLINE int32_t varia_int32x4_get(varia_int32x4_t t, int index) {
	return t[index];
}

VARIA_INLINE varia_int32x4_t varia_int32x4_add(varia_int32x4_t a, varia_int32x4_t b) {
	return vaddq_s32(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_sub(varia_int32x4_t a, varia_int32x4_t b) {
	return vsubq_s32(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_max(varia_int32x4_t a, varia_int32x4_t b) {
	return vmaxq_s32(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_min(varia_int32x4_t a, varia_int32x4_t b) {
	return vminq_s32(a, b);
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpeq(varia_int32x4_t a, varia_int32x4_t b) {
	return vceqq_s32(a, b);
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpge(varia_int32x4_t a, varia_int32x4_t b) {
	return vcgeq_s32(a, b);
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpgt(varia_int32x4_t a, varia_int32x4_t b) {
	return vcgtq_s32(a, b);
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmple(varia_int32x4_t a, varia_int32x4_t b) {
	return vcleq_s32(a, b);
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmplt(varia_int32x4_t a, varia_int32x4_t b) {
	return vcltq_s32(a, b);
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpneq(varia_int32x4_t a, varia_int32x4_t b) {
	return vmvnq_u32(vceqq_s32(a, b));
}

VARIA_INLINE varia_int32x4_t varia_int32x4_sel(varia_int32x4_t a, varia_int32x4_t b, varia_int32x4_mask_t mask) {
	return vbslq_s32(mask, a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_or(varia_int32x4_t a, varia_int32x4_t b) {
	return vorrq_s32(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_and(varia_int32x4_t a, varia_int32x4_t b) {
	return vandq_s32(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_xor(varia_int32x4_t a, varia_int32x4_t b) {
	return veorq_s32(a, b);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_not(varia_int32x4_t t) {
	return vmvnq_s32(t);
}

#define varia_int32x4_shift_left(t, shift)\
	return vshlq_n_s32((t), (shift))

#define varia_int32x4_shift_right(t, shift)\
	return vshrq_n_s32((t), (shift))


#else

VARIA_INLINE varia_int32x4_t varia_int32x4_intrin_load(const int32_t *values) {
	varia_int32x4_t value;
	value.values[0] = values[0];
	value.values[1] = values[1];
	value.values[2] = values[2];
	value.values[3] = values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_intrin_load_unaligned(const int32_t *values) {
	return varia_int32x4_intrin_load(values);
}

VARIA_INLINE varia_int32x4_t varia_int32x4_load(const int32_t values[4]) {
	varia_int32x4_t value;
	value.values[0] = values[0];
	value.values[1] = values[1];
	value.values[2] = values[2];
	value.values[3] = values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_load_all(int32_t t) {
	varia_int32x4_t value;
	value.values[0] = t;
	value.values[1] = t;
	value.values[2] = t;
	value.values[3] = t;
	return value;
}

VARIA_INLINE void varia_int32x4_store(int32_t *destination, varia_int32x4_t value) {
	destination[0] = value.values[0];
	destination[1] = value.values[1];
	destination[2] = value.values[2];
	destination[3] = value.values[3];
}

VARIA_INLINE void varia_int32x4_store_unaligned(int32_t *destination, varia_int32x4_t value) {
	varia_int32x4_store(destination, value);
}

VARIA_INLINE int32_t varia_int32x4_get(varia_int32x4_t t, int index) {
	return t.values[index];
}

VARIA_INLINE varia_int32x4_t varia_int32x4_add(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_t value;
	value.values[0] = a.values[0] + b.values[0];
	value.values[1] = a.values[1] + b.values[1];
	value.values[2] = a.values[2] + b.values[2];
	value.values[3] = a.values[3] + b.values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_sub(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_t value;
	value.values[0] = a.values[0] - b.values[0];
	value.values[1] = a.values[1] - b.values[1];
	value.values[2] = a.values[2] - b.values[2];
	value.values[3] = a.values[3] - b.values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_max(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_t value;
	value.values[0] = a.values[0] > b.values[0] ? a.values[0] : b.values[0];
	value.values[1] = a.values[1] > b.values[1] ? a.values[1] : b.values[1];
	value.values[2] = a.values[2] > b.values[2] ? a.values[2] : b.values[2];
	value.values[3] = a.values[3] > b.values[3] ? a.values[3] : b.values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_min(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_t value;
	value.values[0] = a.values[0] > b.values[0] ? b.values[0] : a.values[0];
	value.values[1] = a.values[1] > b.values[1] ? b.values[1] : a.values[1];
	value.values[2] = a.values[2] > b.values[2] ? b.values[2] : a.values[2];
	value.values[3] = a.values[3] > b.values[3] ? b.values[3] : a.values[3];
	return value;
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpeq(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_mask_t mask;
	mask.values[0] = a.values[0] == b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] == b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] == b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] == b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpge(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_mask_t mask;
	mask.values[0] = a.values[0] >= b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] >= b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] >= b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] >= b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpgt(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_mask_t mask;
	mask.values[0] = a.values[0] > b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] > b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] > b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] > b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmple(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_mask_t mask;
	mask.values[0] = a.values[0] <= b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] <= b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] <= b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] <= b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmplt(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_mask_t mask;
	mask.values[0] = a.values[0] < b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] < b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] < b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] < b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_int32x4_mask_t varia_int32x4_cmpneq(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_mask_t mask;
	mask.values[0] = a.values[0] != b.values[0] ? 0xffffffff : 0;
	mask.values[1] = a.values[1] != b.values[1] ? 0xffffffff : 0;
	mask.values[2] = a.values[2] != b.values[2] ? 0xffffffff : 0;
	mask.values[3] = a.values[3] != b.values[3] ? 0xffffffff : 0;
	return mask;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_sel(varia_int32x4_t a, varia_int32x4_t b, varia_int32x4_mask_t mask) {
	varia_int32x4_t value;
	value.values[0] = mask.values[0] != 0 ? a.values[0] : b.values[0];
	value.values[1] = mask.values[1] != 0 ? a.values[1] : b.values[1];
	value.values[2] = mask.values[2] != 0 ? a.values[2] : b.values[2];
	value.values[3] = mask.values[3] != 0 ? a.values[3] : b.values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_or(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_t value;
	value.values[0] = a.values[0] | b.values[0];
	value.values[1] = a.values[1] | b.values[1];
	value.values[2] = a.values[2] | b.values[2];
	value.values[3] = a.values[3] | b.values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_and(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_t value;
	value.values[0] = a.values[0] & b.values[0];
	value.values[1] = a.values[1] & b.values[1];
	value.values[2] = a.values[2] & b.values[2];
	value.values[3] = a.values[3] & b.values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_xor(varia_int32x4_t a, varia_int32x4_t b) {
	varia_int32x4_t value;
	value.values[0] = a.values[0] ^ b.values[0];
	value.values[1] = a.values[1] ^ b.values[1];
	value.values[2] = a.values[2] ^ b.values[2];
	value.values[3] = a.values[3] ^ b.values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_not(varia_int32x4_t t) {
	varia_int32x4_t value;
	value.values[0] = ~t.values[0];
	value.values[1] = ~t.values[1];
	value.values[2] = ~t.values[2];
	value.values[3] = ~t.values[3];
	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_shift_left(varia_int32x4_t t, const int shift) {
	varia_int32x4_t value;
	value.values[0] = t.values[0] << shift;
	value.values[1] = t.values[1] << shift;
	value.values[2] = t.values[2] << shift;
	value.values[3] = t.values[3] << shift;

	return value;
}

VARIA_INLINE varia_int32x4_t varia_int32x4_shift_right(varia_int32x4_t t, const int shift) {
	varia_int32x4_t value;
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
