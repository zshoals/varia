/* SIMD (SSE1+MMX or SSE2) implementation of sin, cos, exp and log
   Inspired by Intel Approximate Math library, and based on the
   corresponding algorithms of the cephes math library
   The default is to use the SSE1 version. If you define USE_SSE2 the
   the SSE2 intrinsics will be used in place of the MMX intrinsics. Do
   not expect any significant performance improvement with SSE2.
*/

/* Copyright (C) 2007  Julien Pommier
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  (this is the zlib license)
*/



#pragma once

#include <stdint.h>

#include "varia/simd/trig/kinc_simd_trig_impl.h"

#define ALIGN16_BEG
#define ALIGN16_END

#define _PS_CONST(Name, Val)                                            \
  static const ALIGN16_BEG float _ps_##Name[4] ALIGN16_END = { Val, Val, Val, Val }
#define _PI32_CONST(Name, Val)                                            \
  static const ALIGN16_BEG int32_t _pi32_##Name[4] ALIGN16_END = { Val, Val, Val, Val }
#define _PS_CONST_TYPE(Name, Type, Val)                                 \
  static const ALIGN16_BEG Type _ps_##Name[4] ALIGN16_END = { Val, Val, Val, Val }

_PS_CONST(1  , 1.0f);
_PS_CONST(0p5, 0.5f);
/* the smallest non denormalized float number */
_PS_CONST_TYPE(min_norm_pos, int32_t, 0x00800000);
_PS_CONST_TYPE(mant_mask, int32_t, 0x7f800000);
_PS_CONST_TYPE(inv_mant_mask, int32_t, ~0x7f800000);

_PS_CONST_TYPE(sign_mask, int32_t, (int32_t)0x80000000);
_PS_CONST_TYPE(inv_sign_mask, int32_t, ~0x80000000);

_PI32_CONST(1, 1);
_PI32_CONST(inv1, ~1);
_PI32_CONST(2, 2);
_PI32_CONST(4, 4);
_PI32_CONST(0x7f, 0x7f);

_PS_CONST(cephes_SQRTHF, 0.707106781186547524f);
_PS_CONST(cephes_log_p0, 7.0376836292E-2f);
_PS_CONST(cephes_log_p1, - 1.1514610310E-1f);
_PS_CONST(cephes_log_p2, 1.1676998740E-1f);
_PS_CONST(cephes_log_p3, - 1.2420140846E-1f);
_PS_CONST(cephes_log_p4, + 1.4249322787E-1f);
_PS_CONST(cephes_log_p5, - 1.6668057665E-1f);
_PS_CONST(cephes_log_p6, + 2.0000714765E-1f);
_PS_CONST(cephes_log_p7, - 2.4999993993E-1f);
_PS_CONST(cephes_log_p8, + 3.3333331174E-1f);
_PS_CONST(cephes_log_q1, -2.12194440e-4f);
_PS_CONST(cephes_log_q2, 0.693359375f);

static inline simd_fq simd_fq_log(simd_fq x)
{
	simd_iq emm0;
	simd_fq one = fq_load_u(_ps_1);
	simd_fq invalid_mask = fq_cmple(x, fq_zeroes());

	x = fq_max(x, iq_as_fq(iq_load_u(_ps_min_norm_pos)));

	emm0 = uq_as_iq(uq_rshift_23(fq_as_uq(x)));

	x = fq_and(x, iq_as_fq(iq_load_u(_ps_inv_mant_mask)));
	x = fq_or(x, fq_load_u(_ps_0p5));

	emm0 = iq_sub(emm0, iq_load_u(_pi32_0x7f));
	simd_fq e = iq_convert_fq(emm0);

	e = fq_add(e, one);

	simd_fq mask = fq_cmplt(x, fq_load_u(_ps_cephes_SQRTHF));
	simd_fq tmp = fq_and(x, mask);
	x = fq_sub(x, one);
	e = fq_sub(e, fq_and(one, mask));
	x = fq_add(x, tmp);

	simd_fq z = fq_mul(x, x);

	simd_fq y = fq_load_u(_ps_cephes_log_p0);
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_log_p1));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_log_p2));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_log_p3));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_log_p4));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_log_p5));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_log_p6));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_log_p7));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_log_p8));
	y = fq_mul(y, x);

	y = fq_mul(y, z);

	tmp = fq_mul(e, fq_load_u(_ps_cephes_log_q1));
	y = fq_add(y, tmp);

	tmp = fq_mul(z, fq_load_u(_ps_0p5));
	y = fq_sub(y, tmp);

	tmp = fq_mul(e, fq_load_u(_ps_cephes_log_q2));
	x = fq_add(x, y);
	x = fq_add(x, tmp);
	x = fq_or(x, invalid_mask);

	return x;
}







_PS_CONST(exp_hi,	88.3762626647949f);
_PS_CONST(exp_lo,	-88.3762626647949f);

_PS_CONST(cephes_LOG2EF, 1.44269504088896341f);
_PS_CONST(cephes_exp_C1, 0.693359375f);
_PS_CONST(cephes_exp_C2, -2.12194440e-4f);

_PS_CONST(cephes_exp_p0, 1.9875691500E-4f);
_PS_CONST(cephes_exp_p1, 1.3981999507E-3f);
_PS_CONST(cephes_exp_p2, 8.3334519073E-3f);
_PS_CONST(cephes_exp_p3, 4.1665795894E-2f);
_PS_CONST(cephes_exp_p4, 1.6666665459E-1f);
_PS_CONST(cephes_exp_p5, 5.0000001201E-1f);


static inline simd_fq simd_fq_exp(simd_fq x)
{
	simd_fq tmp = fq_zeroes();
	simd_fq fx;
	simd_iq emm0;

	simd_fq one = fq_load_u(_ps_1);

	x = fq_min(x, fq_load_u(_ps_exp_hi));
	x = fq_max(x, fq_load_u(_ps_exp_lo));

	fx = fq_mul(x, fq_load_u(_ps_cephes_LOG2EF));
	fx = fq_add(fx, fq_load_u(_ps_0p5));

	emm0 = fq_truncate_iq(fx);
	tmp = iq_convert_fq(emm0);

	simd_fq mask = fq_cmpgt(tmp, fx);
	mask = fq_and(mask, one);
	fx = fq_sub(tmp, mask);

	tmp = fq_mul(fx, fq_load_u(_ps_cephes_exp_C1));
	simd_fq z = fq_mul(fx, fq_load_u(_ps_cephes_exp_C2));
	x = fq_sub(x, tmp);
	x = fq_sub(x, z);

	z = fq_mul(x, x);

	simd_fq y = fq_load_u(_ps_cephes_exp_p0);
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_exp_p1));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_exp_p2));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_exp_p3));
	y = fq_mul(y, x);
	y = fq_add(y, fq_load_u(_ps_cephes_exp_p4));
	y = fq_mul(y, z);
	y = fq_add(y, x);
	y = fq_add(y, one);

	emm0 = fq_truncate_iq(fx);
	emm0 = iq_add(emm0, iq_load_u(_pi32_0x7f));
	emm0 = uq_as_iq(uq_lshift_23(iq_as_uq(emm0)));

	simd_fq pow2n = iq_as_fq(emm0);
	y = fq_mul(y, pow2n);

	return y;
}



_PS_CONST(minus_cephes_DP1, -0.78515625f);
_PS_CONST(minus_cephes_DP2, -2.4187564849853515625e-4f);
_PS_CONST(minus_cephes_DP3, -3.77489497744594108e-8f);
_PS_CONST(sincof_p0, -1.9515295891E-4f);
_PS_CONST(sincof_p1,  8.3321608736E-3f);
_PS_CONST(sincof_p2, -1.6666654611E-1f);
_PS_CONST(coscof_p0,  2.443315711809948E-005f);
_PS_CONST(coscof_p1, -1.388731625493765E-003f);
_PS_CONST(coscof_p2,  4.166664568298827E-002f);
_PS_CONST(cephes_FOPI, 1.27323954473516f); // 4 / M_PI

static inline simd_fq simd_fq_sin(simd_fq x)
{
	simd_fq xmm1;
	simd_fq xmm2 = fq_zeroes();
	simd_fq xmm3;
	simd_fq sign_bit;
	simd_fq y;

	simd_iq emm0;
	simd_iq emm2;

	sign_bit = x;

	x = fq_and(x, iq_as_fq(iq_load_u(_ps_inv_sign_mask)));

	sign_bit = fq_and(sign_bit, iq_as_fq(iq_load_u(_ps_sign_mask)));

	y = fq_mul(x, fq_load_u(_ps_cephes_FOPI));

	emm2 = fq_truncate_iq(y);

	emm2 = iq_add(emm2, iq_load_u(_pi32_1));
	emm2 = iq_and(emm2, iq_load_u(_pi32_inv1));
	y = iq_convert_fq(emm2);

	emm0 = iq_and(emm2, iq_load_u(_pi32_4));
	emm0 = uq_as_iq(uq_lshift_29(iq_as_uq(emm0)));

	emm2 = iq_and(emm2, iq_load_u(_pi32_2));
	emm2 = uq_as_iq(uq_cmpeq(iq_as_uq(emm2), uq_zeroes()));

	simd_fq swap_sign_bit = iq_as_fq(emm0);
	simd_fq poly_mask = iq_as_fq(emm2);
	sign_bit = fq_xor(sign_bit, swap_sign_bit);

	xmm1 = fq_load_u(_ps_minus_cephes_DP1);
	xmm2 = fq_load_u(_ps_minus_cephes_DP2);
	xmm3 = fq_load_u(_ps_minus_cephes_DP3);
	xmm1 = fq_mul(y, xmm1);
	xmm2 = fq_mul(y, xmm2);
	xmm3 = fq_mul(y, xmm3);
	x = fq_add(x, xmm1);
	x = fq_add(x, xmm2);
	x = fq_add(x, xmm3);

	y = fq_load_u(_ps_coscof_p0);
	simd_fq z = fq_mul(x, x);

	y = fq_mul(y, z);
	y = fq_add(y, fq_load_u(_ps_coscof_p1));
	y = fq_mul(y, z);
	y = fq_add(y, fq_load_u(_ps_coscof_p2));
	//Yes, twice
	y = fq_mul(y, z);
	y = fq_mul(y, z);
	simd_fq tmp = fq_mul(z, fq_load_u(_ps_0p5));
	y = fq_sub(y, tmp);
	y = fq_add(y, fq_load_u(_ps_1));

	simd_fq y2 = fq_load_u(_ps_sincof_p0);
	y2 = fq_mul(y2, z);
	y2 = fq_add(y2, fq_load_u(_ps_sincof_p1));
	y2 = fq_mul(y2, z);
	y2 = fq_add(y2, fq_load_u(_ps_sincof_p2));
	y2 = fq_mul(y2, z);
	y2 = fq_mul(y2, x);
	y2 = fq_add(y2, x);

	xmm3 = poly_mask;
	y2 = fq_and(xmm3, y2);
	y = fq_and(fq_not(xmm3), y);
	y = fq_add(y, y2);

	y = fq_xor(y, sign_bit);

	return y;
}





static inline simd_fq simd_fq_cos(simd_fq x)
{
	simd_fq xmm1;
	simd_fq xmm2 = fq_zeroes();
	simd_fq xmm3;
	simd_fq y;

	simd_iq emm0;
	simd_iq emm2;

	x = fq_and(x, iq_as_fq(iq_load_u(_ps_inv_sign_mask)));

	y = fq_mul(x, fq_load_u(_ps_cephes_FOPI));

	emm2 = fq_truncate_iq(y);

	emm2 = iq_add(emm2, iq_load_u(_pi32_1));
	emm2 = iq_and(emm2, iq_load_u(_pi32_inv1));
	y = iq_convert_fq(emm2);

	emm2 = iq_sub(emm2, iq_load_u(_pi32_2));

	simd_iq tmp_invert = uq_as_iq(uq_not(iq_as_uq(emm2)));
	emm0 = iq_and(tmp_invert, iq_load_u(_pi32_4));
	emm0 = uq_as_iq(uq_lshift_29(iq_as_uq(emm0)));

	emm2 = iq_and(emm2, iq_load_u(_pi32_2));
	emm2 = iq_cmpeq(emm2, iq_zeroes());

	simd_fq sign_bit = iq_as_fq(emm0);
	simd_fq poly_mask = iq_as_fq(emm2);

	xmm1 = fq_load_u(_ps_minus_cephes_DP1);
	xmm2 = fq_load_u(_ps_minus_cephes_DP2);
	xmm3 = fq_load_u(_ps_minus_cephes_DP3);
	xmm1 = fq_mul(y, xmm1);
	xmm2 = fq_mul(y, xmm2);
	xmm3 = fq_mul(y, xmm3);
	x = fq_add(x, xmm1);
	x = fq_add(x, xmm2);
	x = fq_add(x, xmm3);

	y = fq_load_u(_ps_coscof_p0);
	simd_fq z = fq_mul(x, x);

	y = fq_mul(y, z);
	y = fq_add(y, fq_load_u(_ps_coscof_p1));
	y = fq_mul(y, z);
	y = fq_add(y, fq_load_u(_ps_coscof_p2));
	//Yes, twice
	y = fq_mul(y, z);
	y = fq_mul(y, z);
	simd_fq tmp = fq_mul(z, fq_load_u(_ps_0p5));
	y = fq_sub(y, tmp);
	y = fq_add(y, fq_load_u(_ps_1));

	simd_fq y2 = fq_load_u(_ps_sincof_p0);
	y2 = fq_mul(y2, z);
	y2 = fq_add(y2, fq_load_u(_ps_sincof_p1));
	y2 = fq_mul(y2, z);
	y2 = fq_add(y2, fq_load_u(_ps_sincof_p2));
	y2 = fq_mul(y2, z);
	y2 = fq_mul(y2, x);
	y2 = fq_add(y2, x);

	xmm3 = poly_mask;
	y2 = fq_and(xmm3, y2);
	y = fq_and(fq_not(xmm3), y);
	y = fq_add(y, y2);

	y = fq_xor(y, sign_bit);

	return y;
}