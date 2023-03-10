#pragma once

#include <kinc/global.h>

/*! \file types.h
    \brief Provides 128bit SIMD types which are mapped to equivalent SSE or Neon types.
*/

#ifdef __cplusplus
extern "C" {
#endif

// Any level of AVX Capability (Could be AVX, AVX2, AVX512, etc.)
//(Currently) only used for checking existence of earlier SSE instruction sets
#if defined(__AVX__)
// Unfortunate situation here
// MSVC does not provide compiletime macros for the following instruction sets
// but their existence is implied by AVX and higher
#define KINC_SSE4_2
#define KINC_SSE4_1
#define KINC_SSSE3
#define KINC_SSE3
#endif

// SSE2 Capability check
// Note for Windows:
//	_M_IX86_FP checks SSE2 and SSE for 32bit Windows programs only, and is unset if not a 32bit program.
//	SSE2 and earlier is --guaranteed-- to be active for any 64bit Windows program
#if defined(__SSE2__) || (_M_IX86_FP == 2) || (defined(KORE_WINDOWS) && defined(KORE_64))
#define KINC_SSE2
#endif

// SSE Capability check
#if defined(__SSE__) || _M_IX86_FP == 2 || _M_IX86_FP == 1 || (defined(KORE_WINDOWS) && !defined(__aarch64__)) ||                                              \
    (defined(KORE_WINDOWSAPP) && !defined(__aarch64__)) || (defined(KORE_MACOS) && __x86_64)

#define KINC_SSE
#endif

// NEON Capability check
#if defined(KORE_IOS) || defined(KORE_SWITCH) || defined(__aarch64__) || defined(KORE_NEON)
#define KINC_NEON
#endif

// No SIMD Capabilities
#if !defined(KINC_SSE4_2) && !defined(KINC_SSE4_1) && !defined(KINC_SSSE3) && !defined(KINC_SSE3) && !defined(KINC_SSE2) && !defined(KINC_SSE) &&              \
    !defined(KINC_NEON)

#define KINC_NOSIMD
#endif

#define KINC_SHUFFLE_TABLE(LANE_A1, LANE_A2, LANE_B1, LANE_B2)\
	 ((((LANE_B2) & 0x3) << 6) | (((LANE_B1) & 0x3) << 4) | (((LANE_A2) & 0x3) << 2) | (((LANE_A1) & 0x3) << 0))

#if defined(KINC_SSE2)

// SSE_## related headers include earlier revisions, IE
// SSE2 contains all of SSE
#include <emmintrin.h>

typedef __m128 varia_float32x4_t;
typedef __m128 varia_float32x4_mask_t;

typedef __m128i varia_int32x4_t;
typedef __m128i varia_int32x4_mask_t;
typedef __m128i varia_uint32x4_t;
typedef __m128i varia_uint32x4_mask_t;

#elif defined(KINC_SSE)

#include <xmmintrin.h>

typedef __m128 varia_float32x4_t;
typedef __m128 varia_float32x4_mask_t;

typedef struct varia_int32x4 {
	int32_t values[4];
} varia_int32x4_t;

typedef struct varia_uint32x4 {
	uint32_t values[4];
} varia_uint32x4_t;

typedef varia_int32x4_t varia_int32x4_mask_t;
typedef varia_uint32x4_t varia_uint32x4_mask_t;

#elif defined(KINC_NEON)

#include <arm_neon.h>

typedef float32x4_t varia_float32x4_t;
typedef uint32x4_t varia_float32x4_mask_t;

typedef int32x4_t varia_int32x4_t;
typedef uint32x4_t varia_int32x4_mask_t;
typedef uint32x4_t varia_uint32x4_t;
typedef uint32x4_t varia_uint32x4_mask_t;

#elif defined(KINC_NOSIMD)

#include <kinc/math/core.h>

typedef struct varia_float32x4 {
	float values[4];
} varia_float32x4_t;

typedef varia_float32x4_t varia_float32x4_mask_t;



typedef struct varia_int32x4 {
	int32_t values[4];
} varia_int32x4_t;

typedef struct varia_uint32x4 {
	uint32_t values[4];
} varia_uint32x4_t;

typedef varia_int32x4_t varia_int32x4_mask_t;
typedef varia_uint32x4_t varia_uint32x4_mask_t;

#endif

#ifdef __cplusplus
}
#endif
