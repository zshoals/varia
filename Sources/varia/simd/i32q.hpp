#pragma once

#include "varia/simd/simd_types.h"

#include "varia/VShared.hpp"
#include "kinc/simd/int32x4.h"

VARIA_INLINE i32q i32q_load(const i32 *values) { return kinc_int32x4_intrin_load(values); }
VARIA_INLINE void i32q_store(i32 *values, i32q n) { kinc_int32x4_store(values, n); }
VARIA_INLINE i32q i32q_set(i32 a, i32 b, i32 c, i32 d) 
{ 
	i32 temp[4] = { a, b, c, d };
	return kinc_int32x4_load(&temp[0]);
}
VARIA_INLINE i32q i32q_set_all(i32 n) { return kinc_int32x4_load_all(n); }
VARIA_INLINE i32 i32q_get(i32q n, int index) { return kinc_int32x4_get(n, index); }
VARIA_INLINE i32q i32q_add(i32q a, i32q b) { return kinc_int32x4_add(a, b); }
VARIA_INLINE i32q i32q_sub(i32q a, i32q b) { return kinc_int32x4_sub(a, b); }
//No Multiply
//No Division
VARIA_INLINE i32q i32q_max(i32q a, i32q b) { return kinc_int32x4_max(a, b); }
VARIA_INLINE i32q i32q_min(i32q a, i32q b) { return kinc_int32x4_min(a, b); }
VARIA_INLINE i32q_mask i32q_cmpeq(i32q a, i32q b) { return kinc_int32x4_cmpeq(a, b); }
VARIA_INLINE i32q_mask i32q_cmpneq(i32q a, i32q b) { return kinc_int32x4_cmpneq(a, b); }
VARIA_INLINE i32q_mask i32q_cmpgt(i32q a, i32q b) { return kinc_int32x4_cmpgt(a, b); }
VARIA_INLINE i32q_mask i32q_cmpgte(i32q a, i32q b) { return kinc_int32x4_cmpge(a, b); }
VARIA_INLINE i32q_mask i32q_cmplt(i32q a, i32q b) { return kinc_int32x4_cmplt(a, b); }
VARIA_INLINE i32q_mask i32q_cmplte(i32q a, i32q b) { return kinc_int32x4_cmple(a, b); }
VARIA_INLINE i32q i32q_select(i32q a, i32q b, i32q mask) { return kinc_int32x4_sel(a, b, mask); }
VARIA_INLINE i32q i32q_or(i32q a, i32q b) { return kinc_int32x4_or(a, b); }
VARIA_INLINE i32q i32q_and(i32q a, i32q b) { return kinc_int32x4_and(a, b); }
VARIA_INLINE i32q i32q_xor(i32q a, i32q b) { return kinc_int32x4_xor(a, b); }
VARIA_INLINE i32q i32q_not(i32q n) { return kinc_int32x4_not(n); }

//Extensions
VARIA_INLINE i32q i32q_zero(void) { return i32q_set_all(0); }
VARIA_INLINE i32q i32q_ones(void) { return i32q_set_all(0xffffffff); }