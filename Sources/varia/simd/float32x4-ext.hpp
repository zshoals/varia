#pragma once

//Kinc Float32x4 trig extension

#include "kinc/simd/float32x4.h"
#include "kinc/simd/int32x4.h"
#include "kinc/simd/uint32x4.h"
#include "kinc/simd/type_conversions.h"
#include <math.h>

#define ALIGN16_BEG
#define ALIGN16_END

#define _PS_CONST(Name, Val)                                            \
  static const ALIGN16_BEG float _ps_##Name[4] ALIGN16_END = { Val, Val, Val, Val }
#define _PI32_CONST(Name, Val)                                            \
  static const ALIGN16_BEG uint32_t _pi32_##Name[4] ALIGN16_END = { Val, Val, Val, Val }
#define _PS_CONST_TYPE(Name, Type, Val)                                 \
  static const ALIGN16_BEG Type _ps_##Name[4] ALIGN16_END = { Val, Val, Val, Val }

_PS_CONST(1  , 1.0f);
_PS_CONST(0p5, 0.5f);
/* the smallest non denormalized float number */
_PS_CONST_TYPE(min_norm_pos, uint32_t, 0x00800000);
_PS_CONST_TYPE(mant_mask, uint32_t, 0x7f800000);
_PS_CONST_TYPE(inv_mant_mask, uint32_t, ~0x7f800000);

_PS_CONST_TYPE(sign_mask, uint32_t, (uint32_t)0x80000000);
_PS_CONST_TYPE(inv_sign_mask, uint32_t, ~0x80000000);

_PI32_CONST(1, 1);
_PI32_CONST(inv1, ~1);
_PI32_CONST(2, 2);
_PI32_CONST(4, 4);
_PI32_CONST(0x7f, 0x7f);

_PS_CONST(cephes_SQRTHF, 0.707106781186547524);
_PS_CONST(cephes_log_p0, 7.0376836292E-2);
_PS_CONST(cephes_log_p1, - 1.1514610310E-1);
_PS_CONST(cephes_log_p2, 1.1676998740E-1);
_PS_CONST(cephes_log_p3, - 1.2420140846E-1);
_PS_CONST(cephes_log_p4, + 1.4249322787E-1);
_PS_CONST(cephes_log_p5, - 1.6668057665E-1);
_PS_CONST(cephes_log_p6, + 2.0000714765E-1);
_PS_CONST(cephes_log_p7, - 2.4999993993E-1);
_PS_CONST(cephes_log_p8, + 3.3333331174E-1);
_PS_CONST(cephes_log_q1, -2.12194440e-4);
_PS_CONST(cephes_log_q2, 0.693359375);


static inline kinc_float32x4_t kinc_float32x4_mask_load_cast(uint32_t const quad[4])
{
	return kinc_uint32x4_cast_to_float32x4(kinc_uint32x4_intrin_load_unaligned(quad));
}

static inline kinc_float32x4_t kinc_int32x4_cvt_to_float32x4(kinc_int32x4_t t)
{
	int32_t quad[4];
	float quadf[4];
	kinc_int32x4_store_unaligned(&quad[0], t);

	quadf[0] = quad[0];
	quadf[1] = quad[1];
	quadf[2] = quad[2];
	quadf[3] = quad[3];

	return kinc_float32x4_intrin_load_unaligned(&quadf[0]);
}

static inline kinc_float32x4_t kinc_uint32x4_cvt_to_float32x4(kinc_uint32x4_t t)
{
	kinc_int32x4_t it = kinc_uint32x4_cast_to_int32x4(t);

	int32_t quad[4];
	float quadf[4];
	kinc_int32x4_store_unaligned(&quad[0], t);

	quadf[0] = quad[0];
	quadf[1] = quad[1];
	quadf[2] = quad[2];
	quadf[3] = quad[3];

	return kinc_float32x4_intrin_load_unaligned(&quadf[0]);
}

static inline kinc_int32x4_t kinc_float32x4_cvt_to_int32x4(kinc_float32x4_t t)
{
	float quadf[4];
	int32_t quad[4];
	kinc_float32x4_store_unaligned(&quadf[0], t);

	quad[0] = roundf(quadf[0]);
	quad[1] = roundf(quadf[1]);
	quad[2] = roundf(quadf[2]);
	quad[3] = roundf(quadf[3]);

	return kinc_int32x4_intrin_load_unaligned(&quad[0]);
}

static inline kinc_int32x4_t kinc_float32x4_cvt_truncate_to_int32x4(kinc_float32x4_t t)
{
	float quadf[4];
	int32_t quad[4];
	kinc_float32x4_store_unaligned(&quadf[0], t);

	quad[0] = floorf(quadf[0]);
	quad[1] = floorf(quadf[1]);
	quad[2] = floorf(quadf[2]);
	quad[3] = floorf(quadf[3]);

	return kinc_int32x4_intrin_load_unaligned(&quad[0]);
}

kinc_float32x4_t kinc_float32x4_log(kinc_float32x4_t x)
{
	kinc_int32x4_t emm0;
	kinc_float32x4_t one = kinc_float32x4_intrin_load_unaligned(_ps_1);
	kinc_float32x4_t invalid_mask = kinc_float32x4_cmple(x, kinc_float32x4_load_all(0.0f));

	x = kinc_float32x4_max(x, kinc_float32x4_mask_load_cast(_ps_min_norm_pos));
	emm0 = kinc_uint32x4_cast_to_int32x4(kinc_uint32x4_shift_right(kinc_float32x4_cast_to_uint32x4(x), 23));
	x = kinc_float32x4_and(x, kinc_float32x4_mask_load_cast(_ps_inv_mant_mask));
	x = kinc_float32x4_or(x, kinc_float32x4_intrin_load_unaligned(_ps_0p5));

	emm0 = kinc_int32x4_sub(emm0, kinc_uint32x4_cast_to_int32x4(kinc_uint32x4_intrin_load_unaligned(_pi32_0x7f)));

	kinc_float32x4_t e = kinc_int32x4_cvt_to_float32x4(emm0);
	e = kinc_float32x4_add(e, one);

	kinc_float32x4_t mask = kinc_float32x4_cmplt(x, kinc_float32x4_intrin_load_unaligned(_ps_cephes_SQRTHF));
	kinc_float32x4_t tmp = kinc_float32x4_and(x, mask);
	x = kinc_float32x4_sub(x, one);
	e = kinc_float32x4_sub(e, kinc_float32x4_and(one, mask));
	x = kinc_float32x4_add(x, tmp);

	kinc_float32x4_t z = kinc_float32x4_mul(x, x);

	kinc_float32x4_t y = kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_p0);
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_p1));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_p2));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_p3));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_p4));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_p5));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_p6));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_p7));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_p8));
	y = kinc_float32x4_mul(y, x);

	y = kinc_float32x4_mul(y, z);

	tmp = kinc_float32x4_mul(e, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_q1));
	y = kinc_float32x4_add(y, tmp);

	tmp = kinc_float32x4_mul(z, kinc_float32x4_intrin_load_unaligned(_ps_0p5));
	y = kinc_float32x4_sub(y, tmp);

	tmp = kinc_float32x4_mul(e, kinc_float32x4_intrin_load_unaligned(_ps_cephes_log_q2));
	x = kinc_float32x4_add(x, y);
	x = kinc_float32x4_add(x, tmp);
	x = kinc_float32x4_or(x, invalid_mask);

	return x;
}


_PS_CONST(exp_hi,	88.3762626647949f);
_PS_CONST(exp_lo,	-88.3762626647949f);

_PS_CONST(cephes_LOG2EF, 1.44269504088896341);
_PS_CONST(cephes_exp_C1, 0.693359375);
_PS_CONST(cephes_exp_C2, -2.12194440e-4);

_PS_CONST(cephes_exp_p0, 1.9875691500E-4);
_PS_CONST(cephes_exp_p1, 1.3981999507E-3);
_PS_CONST(cephes_exp_p2, 8.3334519073E-3);
_PS_CONST(cephes_exp_p3, 4.1665795894E-2);
_PS_CONST(cephes_exp_p4, 1.6666665459E-1);
_PS_CONST(cephes_exp_p5, 5.0000001201E-1);

kinc_float32x4_t kinc_float32x4_exp(kinc_float32x4_t x)
{
	kinc_float32x4_t tmp = kinc_float32x4_load_all(0.0f);
	kinc_float32x4_t fx;

	kinc_uint32x4_t emm0;

	kinc_float32x4_t one = kinc_float32x4_intrin_load_unaligned(_ps_1);
	x = kinc_float32x4_min(x, kinc_float32x4_intrin_load_unaligned(_ps_exp_hi));
	x = kinc_float32x4_max(x, kinc_float32x4_intrin_load_unaligned(_ps_exp_lo));

	fx = kinc_float32x4_mul(x, kinc_float32x4_intrin_load_unaligned(_ps_cephes_LOG2EF));
	fx = kinc_float32x4_add(fx, kinc_float32x4_intrin_load_unaligned(_ps_0p5));

	emm0 = kinc_float32x4_cvt_truncate_to_int32x4(fx);
	tmp = kinc_uint32x4_cast_to_float32x4(emm0);

	kinc_float32x4_t mask = kinc_float32x4_cmpgt(tmp, fx);
	mask = kinc_float32x4_and(mask, one);
	fx = kinc_float32x4_sub(tmp, mask);

	tmp = kinc_float32x4_mul(fx, kinc_float32x4_intrin_load_unaligned(_ps_cephes_exp_C1));
	kinc_float32x4_t z = kinc_float32x4_mul(fx, kinc_float32x4_intrin_load_unaligned(_ps_cephes_exp_C2));
	x = kinc_float32x4_sub(x, tmp);
	x = kinc_float32x4_sub(x, z);

	z = kinc_float32x4_mul(x, x);

	kinc_float32x4_t y = kinc_float32x4_intrin_load_unaligned(_ps_cephes_exp_p0);
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_exp_p1));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_exp_p2));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_exp_p3));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_exp_p4));
	y = kinc_float32x4_mul(y, x);
	y = kinc_float32x4_add(y, kinc_float32x4_intrin_load_unaligned(_ps_cephes_exp_p5));
	y = kinc_float32x4_mul(y, z);
	y = kinc_float32x4_add(y, x);
	y = kinc_float32x4_add(y, one);

	emm0 = kinc_float32x4_cvt_truncate_to_int32x4(fx);
	emm0 = kinc_int32x4_cast_to_uint32x4
	(
		kinc_int32x4_add
		(
			kinc_uint32x4_cast_to_int32x4(emm0),
			kinc_uint32x4_cast_to_int32x4(kinc_uint32x4_intrin_load_unaligned(_pi32_0x7f))
		)
	);
	emm0 = kinc_uint32x4_shift_left(emm0, 23);
	kinc_float32x4_t pow2n = kinc_uint32x4_cast_to_float32x4(emm0);

	y = kinc_float32x4_mul(y, pow2n);

	return y;
}