#pragma once

#include "varia/vcommon.hpp"
#include "kinc/simd/int32x4.h"

typedef kinc_int32x4_t i32q;
typedef kinc_int32x4_mask_t i32q_mask;

static inline i32q i32q_load(const i32 *values) { return kinc_int32x4_intrin_load(values); }
static inline void i32q_store(i32 *values, i32q n) { kinc_int32x4_store(values, n); }
static inline i32q i32q_set(i32 a, i32 b, i32 c, i32 d) 
{ 
	i32 temp[4] = { a, b, c, d };
	return kinc_int32x4_load(&temp[0]);
}
static inline i32q i32q_set_all(i32 n) { return kinc_int32x4_load_all(n); }
static inline i32 i32q_get(i32q n, int index) { return kinc_int32x4_get(n, index); }
static inline i32q i32q_add(i32q a, i32q b) { return kinc_int32x4_add(a, b); }
static inline i32q i32q_sub(i32q a, i32q b) { return kinc_int32x4_sub(a, b); }
//No Multiply
//No Division
static inline i32q i32q_max(i32q a, i32q b) { return kinc_int32x4_max(a, b); }
static inline i32q i32q_min(i32q a, i32q b) { return kinc_int32x4_min(a, b); }
static inline i32q_mask i32q_cmpeq(i32q a, i32q b) { return kinc_int32x4_cmpeq(a, b); }
static inline i32q_mask i32q_cmpneq(i32q a, i32q b) { return kinc_int32x4_cmpneq(a, b); }
static inline i32q_mask i32q_cmpgt(i32q a, i32q b) { return kinc_int32x4_cmpgt(a, b); }
static inline i32q_mask i32q_cmpgte(i32q a, i32q b) { return kinc_int32x4_cmpge(a, b); }
static inline i32q_mask i32q_cmplt(i32q a, i32q b) { return kinc_int32x4_cmplt(a, b); }
static inline i32q_mask i32q_cmplte(i32q a, i32q b) { return kinc_int32x4_cmple(a, b); }
static inline i32q i32q_select(i32q a, i32q b, i32q mask) { return kinc_int32x4_sel(a, b, mask); }
static inline i32q i32q_or(i32q a, i32q b) { return kinc_int32x4_or(a, b); }
static inline i32q i32q_and(i32q a, i32q b) { return kinc_int32x4_and(a, b); }
static inline i32q i32q_xor(i32q a, i32q b) { return kinc_int32x4_xor(a, b); }
static inline i32q i32q_not(i32q n) { return kinc_int32x4_not(n); }

//Extensions
static inline i32q i32q_zero(void) { return i32q_set_all(0); }
static inline i32q i32q_ones(void) { return i32q_set_all(0xffffffff); }