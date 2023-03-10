#pragma once

#include <stdint.h>
#include <math.h>

#include "varia/simd/impl/types.h"
#include "varia/simd/impl/float32x4.h"
#include "varia/simd/impl/int32x4.h"
#include "varia/simd/impl/uint32x4.h"
#include "varia/simd/impl/type_conversions.h"


typedef varia_float32x4_t simd_fq;
typedef varia_int32x4_t simd_iq;
typedef varia_uint32x4_t simd_uq;

//implement these functions

//Floats
static inline simd_fq fq_load_u(float const * quad)
{
	return varia_float32x4_intrin_load_unaligned(quad);
}
static inline void fq_store_u(float * destination, simd_fq t)
{
	varia_float32x4_store_unaligned(destination, t);
}
static inline simd_fq fq_zeroes(void)
{
	return varia_float32x4_load_all(0.0f);
}
static inline simd_fq fq_min(simd_fq a, simd_fq b)
{
	return varia_float32x4_min(a, b);
}
static inline simd_fq fq_max(simd_fq a, simd_fq b)
{
	return varia_float32x4_max(a, b);
}
static inline simd_fq fq_add(simd_fq a, simd_fq b)
{
	return varia_float32x4_add(a, b);
}
static inline simd_fq fq_sub(simd_fq a, simd_fq b)
{
	return varia_float32x4_sub(a, b);
}
static inline simd_fq fq_mul(simd_fq a, simd_fq b)
{
	return varia_float32x4_mul(a, b);
}
static inline simd_fq fq_div(simd_fq a, simd_fq b)
{
	return varia_float32x4_div(a, b);
}
static inline simd_fq fq_cmpeq(simd_fq a, simd_fq b)
{
	return varia_float32x4_cmpeq(a, b);
}
static inline simd_fq fq_cmplt(simd_fq a, simd_fq b)
{
	return varia_float32x4_cmplt(a, b);
}
static inline simd_fq fq_cmple(simd_fq a, simd_fq b)
{
	return varia_float32x4_cmple(a, b);
}
static inline simd_fq fq_cmpgt(simd_fq a, simd_fq b)
{
	return varia_float32x4_cmpgt(a, b);
}
static inline simd_fq fq_cmpge(simd_fq a, simd_fq b)
{
	return varia_float32x4_cmpge(a, b);
}
static inline simd_fq fq_and(simd_fq a, simd_fq b)
{
	return varia_float32x4_and(a, b);
}
static inline simd_fq fq_or(simd_fq a, simd_fq b)
{
	return varia_float32x4_or(a, b);
}
static inline simd_fq fq_xor(simd_fq a, simd_fq b)
{
	return varia_float32x4_xor(a, b);
}
static inline simd_fq fq_not(simd_fq t)
{
	return varia_float32x4_not(t);
}
static inline simd_iq fq_as_iq(simd_fq t)
{
	return varia_float32x4_cast_to_int32x4(t);
}
static inline simd_uq fq_as_uq(simd_fq t)
{
	return varia_float32x4_cast_to_uint32x4(t);
}
static inline simd_iq fq_truncate_iq(simd_fq t)
{
	return varia_float32x4_truncate_to_int32x4(t);
}

static inline simd_iq fq_round_iq(simd_fq t)
{
	return varia_float32x4_convert_to_int32x4(t);
}



//Signed Ints
static inline simd_iq iq_load_u(int32_t const * quad)
{
	return varia_int32x4_intrin_load_unaligned(quad);
}
static inline void iq_store_u(int32_t * destination, simd_iq t)
{
	varia_int32x4_store_unaligned(destination, t);
}
static inline simd_iq iq_zeroes(void)
{
	return varia_int32x4_load_all(0);
}

static inline simd_iq iq_cmpeq(simd_iq a, simd_iq b)
{
	return varia_int32x4_cmpeq(a, b);
}

static inline simd_iq iq_add(simd_iq a, simd_iq b)
{
	return varia_int32x4_add(a, b);
}
static inline simd_iq iq_sub(simd_iq a, simd_iq b)
{
	return varia_int32x4_sub(a, b);
}
static inline simd_iq iq_and(simd_iq a, simd_iq b)
{
	return varia_int32x4_and(a, b);
}
static inline simd_iq iq_or(simd_iq a, simd_iq b)
{
	return varia_int32x4_or(a, b);
}
static inline simd_iq iq_xor(simd_iq a, simd_iq b)
{
	return varia_int32x4_xor(a, b);
}

static inline simd_fq iq_as_fq(simd_iq t)
{
	return varia_int32x4_cast_to_float32x4(t);
}
static inline simd_uq iq_as_uq(simd_iq t)
{
	return varia_int32x4_cast_to_uint32x4(t);
}

static inline simd_fq iq_convert_fq(simd_iq t)
{
	return varia_int32x4_convert_to_float32x4(t);
}



//Un-signed Ints
static inline simd_uq uq_load_u(uint32_t const * quad)
{
	return varia_uint32x4_intrin_load_unaligned(quad);
}
static inline void uq_store_u(uint32_t * destination, simd_uq t)
{
	varia_uint32x4_store_unaligned(destination, t);
}
static inline simd_uq uq_zeroes(void)
{
	return varia_uint32x4_load_all(0);
}

static inline simd_uq uq_cmpeq(simd_uq a, simd_uq b)
{
	return varia_uint32x4_cmpeq(a, b);
}
static inline simd_uq uq_and(simd_uq a, simd_uq b)
{
	return varia_uint32x4_and(a, b);
}
static inline simd_uq uq_or(simd_uq a, simd_uq b)
{
	return varia_uint32x4_or(a, b);
}
static inline simd_uq uq_xor(simd_uq a, simd_uq b)
{
	return varia_uint32x4_xor(a, b);
}

static inline simd_uq uq_not(simd_uq t)
{
	return varia_uint32x4_not(t);
}

static inline simd_uq uq_lshift_23(simd_uq t)
{
	return varia_uint32x4_shift_left(t, 23);
}
static inline simd_uq uq_rshift_23(simd_uq t)
{
	return varia_uint32x4_shift_right(t, 23);
}
static inline simd_uq uq_lshift_29(simd_uq t)
{
	return varia_uint32x4_shift_left(t, 29);
}

static inline simd_fq uq_as_fq(simd_uq t)
{
	return varia_uint32x4_cast_to_float32x4(t);
}
static inline simd_iq uq_as_iq(simd_uq t)
{
	return varia_uint32x4_cast_to_int32x4(t);
}
