#pragma once

#include "varia/Vcommon.hpp"
#include "types.h"
#include <kinc/global.h>
#include <string>

/*! \file type_conversions.h
	\brief Provides type casts and type conversions between all 128bit SIMD types
*/

#ifdef __cplusplus
extern "C" {
#endif


#if defined(KINC_SSE2)

//====================================================
//                  Conversion
//====================================================



VARIA_INLINE varia_int32x4_t varia_float32x4_convert_to_int32x4(varia_float32x4_t t)
{
	return _mm_cvtps_epi32(t);
}

VARIA_INLINE varia_int32x4_t varia_float32x4_truncate_to_int32x4(varia_float32x4_t t)
{
	return _mm_cvttps_epi32(t);
}

VARIA_INLINE varia_float32x4_t varia_int32x4_convert_to_float32x4(varia_int32x4_t t)
{
	return _mm_cvtepi32_ps(t);
}

VARIA_INLINE uint32_t varia_uint32x4_extract_uint32(varia_uint32x4_t t)
{
	int32_t pre_out = _mm_cvtsi128_si32(t);
	uint32_t out;
	memcpy(&out, &pre_out, sizeof(pre_out));

	return out;
}



//====================================================
//                  Casts
//====================================================



//Float32x4 ----> Other
VARIA_INLINE varia_int32x4_t varia_float32x4_cast_to_int32x4(varia_float32x4_t t) {
	return _mm_castps_si128(t);
}

VARIA_INLINE varia_uint32x4_t varia_float32x4_cast_to_uint32x4(varia_float32x4_t t) {
	return _mm_castps_si128(t);
}



//Int32x4 ----> Other
VARIA_INLINE varia_float32x4_t varia_int32x4_cast_to_float32x4(varia_int32x4_t t) {
	return _mm_castsi128_ps(t);
}

VARIA_INLINE varia_uint32x4_t varia_int32x4_cast_to_uint32x4(varia_int32x4_t t) {
	//SSE2's m128i is every int type, so we can just return any inbound int type parameter
	return t;
}



//Unsigned Int32x4 ----> Other
VARIA_INLINE varia_float32x4_t varia_uint32x4_cast_to_float32x4(varia_uint32x4_t t) {
	return _mm_castsi128_ps(t);
}

VARIA_INLINE varia_int32x4_t varia_uint32x4_cast_to_int32x4(varia_uint32x4_t t) {
	return t;
}


#elif defined(KINC_SSE)

//====================================================
//                  Conversion
//====================================================

#include <math.h>


VARIA_INLINE varia_int32x4_t varia_float32x4_convert_to_int32x4(varia_float32x4_t t)
{
	float extracted[4];
	_mm_storeu_ps(&extracted[0], t);

	varia_int32x4_t cvt;

	cvt.values[0] = (int32_t)roundf(extracted[0]);
	cvt.values[1] = (int32_t)roundf(extracted[1]);
	cvt.values[2] = (int32_t)roundf(extracted[2]);
	cvt.values[3] = (int32_t)roundf(extracted[3]);

	return cvt;
}

VARIA_INLINE varia_int32x4_t varia_float32x4_truncate_to_int32x4(varia_float32x4_t t)
{
	//FILL ME
	float extracted[4];
	_mm_storeu_ps(&extracted[0], t);

	varia_int32x4_t cvt;

	cvt.values[0] = (int32_t)truncf(extracted[0]);
	cvt.values[1] = (int32_t)truncf(extracted[1]);
	cvt.values[2] = (int32_t)truncf(extracted[2]);
	cvt.values[3] = (int32_t)truncf(extracted[3]);

	return cvt;
}

VARIA_INLINE varia_float32x4_t varia_int32x4_convert_to_float32x4(varia_int32x4_t t)
{
	float cvt[4];

	cvt[0] = (float)t.values[0];
	cvt[1] = (float)t.values[1];
	cvt[2] = (float)t.values[2];
	cvt[3] = (float)t.values[3];

	return _mm_loadu_ps(&cvt[0]);
}


VARIA_INLINE uint32_t varia_uint32x4_extract_uint32(varia_uint32x4_t t)
{
	return t.values[0];
}





//====================================================
//                  Casts
//====================================================




//Float32x4 ----> Other
VARIA_INLINE varia_int32x4_t varia_float32x4_cast_to_int32x4(varia_float32x4_t t) {
	float extracted[4];
	_mm_storeu_ps(&extracted[0], t);

	varia_int32x4_t cvt;
	memcpy(&cvt.values[0], &extracted[0], sizeof(extracted));

	return cvt;
}

VARIA_INLINE varia_uint32x4_t varia_float32x4_cast_to_uint32x4(varia_float32x4_t t) {
	float extracted[4];
	_mm_storeu_ps(&extracted[0], t);

	varia_uint32x4_t cvt;
	memcpy(&cvt.values[0], &extracted[0], sizeof(extracted));

	return cvt;
}



//Int32x4 ----> Other
VARIA_INLINE varia_float32x4_t varia_int32x4_cast_to_float32x4(varia_int32x4_t t) {
	float cvt[4];
	memcpy(&cvt[0], &t.values[0], sizeof(t));

	return _mm_loadu_ps(&cvt[0]);
}


//Unsigned Int32x4 ----> Other
VARIA_INLINE varia_float32x4_t varia_uint32x4_cast_to_float32x4(varia_uint32x4_t t) {
	float cvt[4];
	memcpy(&cvt[0], &t.values[0], sizeof(t));

	return _mm_loadu_ps(&cvt[0]);
}



#elif defined(KINC_NEON)

//====================================================
//                  Conversion
//====================================================


VARIA_INLINE varia_int32x4_t varia_float32x4_convert_to_int32x4(varia_float32x4_t t)
{
	return vcvtaq_s32_f32(t);
}

VARIA_INLINE varia_int32x4_t varia_float32x4_truncate_to_int32x4(varia_float32x4_t t)
{
	return vcvtq_s32_f32(t);
}

VARIA_INLINE varia_float32x4_t varia_int32x4_convert_to_float32x4(varia_int32x4_t t)
{
	return vcvtq_f32_s32(t);
}


VARIA_INLINE uint32_t varia_uint32x4_extract_uint32(varia_uint32x4_t t)
{
	return t[index];
}



//====================================================
//                  Casts
//====================================================




//Float32x4 ----> Other
VARIA_INLINE varia_int32x4_t varia_float32x4_cast_to_int32x4(varia_float32x4_t t) {
	return vreinterpretq_s32_f32(t);
}

VARIA_INLINE varia_uint32x4_t varia_float32x4_cast_to_uint32x4(varia_float32x4_t t) {
	return vreinterpretq_u32_f32(t);
}


//Int32x4 ----> Other
VARIA_INLINE varia_float32x4_t varia_int32x4_cast_to_float32x4(varia_int32x4_t t) {
	return vreinterpretq_f32_s32(t);
}

VARIA_INLINE varia_uint32x4_t varia_int32x4_cast_to_uint32x4(varia_int32x4_t t) {
	return vreinterpretq_u32_s32(t);
}


//Unsigned Int32x4 ----> Other
VARIA_INLINE varia_float32x4_t varia_uint32x4_cast_to_float32x4(varia_uint32x4_t t) {
	return vreinterpretq_f32_u32(t);
}

VARIA_INLINE varia_int32x4_t varia_uint32x4_cast_to_int32x4(varia_uint32x4_t t) {
	return vreinterpretq_s32_u32(t);
}


//KINC_NOSIMD float fallbacks casts
#else




//====================================================
//                  Conversion
//====================================================

#include <math.h>

VARIA_INLINE varia_int32x4_t varia_float32x4_convert_to_int32x4(varia_float32x4_t t)
{
	varia_int32x4_t out;

	out.values[0] = (int32_t)roundf(t.values[0]);
	out.values[1] = (int32_t)roundf(t.values[1]);
	out.values[2] = (int32_t)roundf(t.values[2]);
	out.values[3] = (int32_t)roundf(t.values[3]);

	return out;
}

VARIA_INLINE varia_int32x4_t varia_float32x4_truncate_to_int32x4(varia_float32x4_t t)
{
	varia_int32x4_t out;

	out.values[0] = (int32_t)truncf(t.values[0]);
	out.values[1] = (int32_t)truncf(t.values[1]);
	out.values[2] = (int32_t)truncf(t.values[2]);
	out.values[3] = (int32_t)truncf(t.values[3]);

	return out;
}

VARIA_INLINE varia_float32x4_t varia_int32x4_convert_to_float32x4(varia_int32x4_t t)
{
	varia_float32x4_t out;

	out.values[0] = (float)t.values[0];
	out.values[1] = (float)t.values[1];
	out.values[2] = (float)t.values[2];
	out.values[3] = (float)t.values[3];

	return out;
}


VARIA_INLINE uint32_t varia_uint32x4_extract_uint32(varia_uint32x4_t t)
{
	return t.values[0];
}



//====================================================
//                  Casts
//====================================================




//Float32x4 ----> Other
VARIA_INLINE varia_int32x4_t varia_float32x4_cast_to_int32x4(varia_float32x4_t t) {
	varia_int32x4_t cvt;
	memcpy(&cvt.values[0], &t.values[0], sizeof(t));

	return cvt;
}

VARIA_INLINE varia_uint32x4_t varia_float32x4_cast_to_uint32x4(varia_float32x4_t t) {
	varia_uint32x4_t cvt;
	memcpy(&cvt.values[0], &t.values[0], sizeof(t));

	return cvt;
}


//Int32x4 ----> Float32x4
VARIA_INLINE varia_float32x4_t varia_int32x4_cast_to_float32x4(varia_int32x4_t t) {
	varia_float32x4_t cvt;
	memcpy(&cvt.values[0], &t.values[0], sizeof(t));

	return cvt;
}


//Unsigned Int32x4 ----> Float32x4
VARIA_INLINE varia_float32x4_t varia_uint32x4_cast_to_float32x4(varia_uint32x4_t t) {
	varia_float32x4_t cvt;
	memcpy(&cvt.values[0], &t.values[0], sizeof(t));

	return cvt;
}



#endif //KINC_NOSIMD floats




//Shared signed and unsigned integer vectors for SSE and SIMD-fallback
#if !defined(KINC_SSE2) && (defined(KINC_SSE) || defined(KINC_NOSIMD)) 

//Int32x4 ----> Other
VARIA_INLINE varia_uint32x4_t varia_int32x4_cast_to_uint32x4(varia_int32x4_t t) {
	varia_uint32x4_t cvt;
	memcpy(&cvt.values[0], &t.values[0], sizeof(t));

	return cvt;
}


//Unsigned Int32x4 ----> Other
VARIA_INLINE varia_int32x4_t varia_uint32x4_cast_to_int32x4(varia_uint32x4_t t) {
	varia_int32x4_t cvt;
	memcpy(&cvt.values[0], &t.values[0], sizeof(t));

	return cvt;
}


#endif //KINC_SSE || KINC_NOSIMD



#ifdef __cplusplus
}
#endif
