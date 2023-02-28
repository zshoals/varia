#pragma once

#include "varia/vcommon.hpp"
#include "kinc/simd/uint32x4.h"

typedef kinc_uint32x4_t u32q;
typedef kinc_uint32x4_mask_t u32q_mask;

static inline u32q u32q_load(const u32 *values) { return kinc_uint32x4_intrin_load(values); }
static inline void u32q_store(u32 *values, u32q n) { kinc_uint32x4_store(values, n); }
static inline u32q u32q_set(u32 a, u32 b, u32 c, u32 d) 
{ 
	u32 temp[4] = { a, b, c, d };
	return kinc_uint32x4_load(&temp[0]);
}
static inline u32q u32q_set_all(u32 n) { return kinc_uint32x4_load_all(n); }
static inline u32 u32q_get(u32q n, int index) { return kinc_uint32x4_get(n, index); }
static inline u32q u32q_add(u32q a, u32q b) { return kinc_uint32x4_add(a, b); }
static inline u32q u32q_sub(u32q a, u32q b) { return kinc_uint32x4_sub(a, b); }
//No Multiply
//No Division
static inline u32q u32q_max(u32q a, u32q b) { return kinc_uint32x4_max(a, b); }
static inline u32q u32q_min(u32q a, u32q b) { return kinc_uint32x4_min(a, b); }
static inline u32q_mask u32q_cmpeq(u32q a, u32q b) { return kinc_uint32x4_cmpeq(a, b); }
static inline u32q_mask u32q_cmpneq(u32q a, u32q b) { return kinc_uint32x4_cmpneq(a, b); }
static inline u32q_mask u32q_cmpgt(u32q a, u32q b) { return kinc_uint32x4_cmpgt(a, b); }
static inline u32q_mask u32q_cmpgte(u32q a, u32q b) { return kinc_uint32x4_cmpge(a, b); }
static inline u32q_mask u32q_cmplt(u32q a, u32q b) { return kinc_uint32x4_cmplt(a, b); }
static inline u32q_mask u32q_cmplte(u32q a, u32q b) { return kinc_uint32x4_cmple(a, b); }
static inline u32q u32q_select(u32q a, u32q b, u32q mask) { return kinc_uint32x4_sel(a, b, mask); }
static inline u32q u32q_or(u32q a, u32q b) { return kinc_uint32x4_or(a, b); }
static inline u32q u32q_and(u32q a, u32q b) { return kinc_uint32x4_and(a, b); }
static inline u32q u32q_xor(u32q a, u32q b) { return kinc_uint32x4_xor(a, b); }
static inline u32q u32q_not(u32q n) { return kinc_uint32x4_not(n); }
#define u32q_shift_left(n, shift) kinc_uint32x4_shift_left((n), (shift))
#define u32q_shift_right(n, shift) kinc_uint32x4_shift_right((n), (shift))

//Operator Overloads
static inline u32q operator+(u32q const & a, u32q const & b) { return u32q_add(a, b); }
static inline u32q operator-(u32q const & a, u32q const & b) { return u32q_sub(a, b); }
static inline void operator+=(u32q & a, u32q const & b) { a = u32q_add(a, b); }
static inline void operator-=(u32q & a, u32q const & b) { a = u32q_sub(a, b); }
static inline u32q_mask operator>(u32q const & a, u32q const & b) { return u32q_cmpgt(a, b); }
static inline u32q_mask operator>=(u32q const & a, u32q const & b) { return u32q_cmpgte(a, b); }
static inline u32q_mask operator<(u32q const & a, u32q const & b) { return u32q_cmplt(a, b); }
static inline u32q_mask operator<=(u32q const & a, u32q const & b) { return u32q_cmplte(a, b); }
static inline u32q_mask operator==(u32q const & a, u32q const & b) { return u32q_cmpeq(a, b); }
static inline u32q_mask operator!=(u32q const & a, u32q const & b) { return u32q_cmpneq(a, b); }
static inline u32q operator&(u32q const & a, u32q const & b) { return u32q_and(a, b); }
static inline u32q operator|(u32q const & a, u32q const & b) { return u32q_or(a, b); }
static inline u32q operator^(u32q const & a, u32q const & b) { return u32q_xor(a, b); }
static inline void operator&=(u32q & a, u32q const & b) { a = u32q_and(a, b); }
static inline void operator|=(u32q & a, u32q const & b) { a = u32q_or(a, b); }
static inline void operator^=(u32q & a, u32q const & b) { a = u32q_xor(a, b); }

//Extensions
static inline u32q u32q_zero(void) { return u32q_set_all(0); }
static inline u32q u32q_ones(void) { return u32q_set_all(0xffffffff); }