#pragma once

//Includes types that are basically used everywhere

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;
typedef unsigned char uchar;
typedef unsigned char byte;
typedef unsigned int uint;
#include "varia/math/MathTypes.hpp"


#ifdef _MSC_VER
	#define VARIA_INLINE static __forceinline
#else
	#define VARIA_INLINE static __attribute__((always_inline))
#endif

#ifdef KORE_WINDOWS
	#define VARIA_FUNCTION_NAME __FUNCTION__
#else
	#define VARIA_FUNCTION_NAME ""
#endif

#define VARIA_STRINGIFY_IMPL(X) #X
#define VARIA_STRINGIFY(X) VARIA_STRINGIFY_IMPL(X)

#define VARIA_CONCAT_IMPL(A, B) A##B
#define VARIA_CONCAT(A, B) VARIA_CONCAT_IMPL(A, B)
#define VARIA_MACRO_VAR(VAR_NAME) VARIA_CONCAT(___, VARIA_CONCAT(__LINE__, VAR_NAME))

#define VARIA_NO_OPERATION() ((void)0)
#define VARIA_INTENTIONALLY_DO_NOTHING() ((void)0)

#define VARIA_ITERATION_GUARD(ITERATION_VARIABLE, MAX_COUNT)\
	if ((ITERATION_VARIABLE) > (MAX_COUNT)) { __debugbreak(); }



//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                   For range loops for convenience                   ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

//Note: For now, only support positive values
#define for_range(STOP)\
	assert(STOP >= 0);\
	for (int VARIA_CONCAT(_i, __LINE__) = 0; VARIA_CONCAT(_i, __LINE__) < (STOP); ++VARIA_CONCAT(_i, __LINE__))

#define for_range_var(CAPTURE, STOP)\
	assert(STOP >= 0);\
	for (int (CAPTURE) = 0; (CAPTURE) < (STOP); ++(CAPTURE))

#define for_reverse_range(START)\
	assert(START >= 0);\
	for (int VARIA_CONCAT(_i, __LINE__) = (START); VARIA_CONCAT(_i, __LINE__)--;)

#define for_reverse_range_var(CAPTURE, START)\
	assert(START >= 0);\
	for (int (CAPTURE) = (START); (CAPTURE)--;)

#define for_range_var_slice(CAPTURE, START, STOP)\
	assert(START >= 0 && STOP >= 0);\
	for (int (CAPTURE) = START; (CAPTURE) < (STOP); ++(CAPTURE))

#define for_range_var_step(CAPTURE, STOP, STEP)\
	assert(STOP >= 0);\
	for (int (CAPTURE) = 0; (CAPTURE) < (STOP); (CAPTURE) += (STEP))