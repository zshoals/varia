#pragma once

//Includes types that are basically used everywhere

#include <stdint.h>
#include <assert.h>
#include <string.h>

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
typedef unsigned int uint;

#define VARIA_STRINGIFY_IMPL(X) #X
#define VARIA_STRINGIFY(X) VARIA_STRINGIFY_IMPL(X)

#define VARIA_CONCAT_IMPL(A, B) A##B
#define VARIA_CONCAT(A, B) VARIA_CONCAT_IMPL(A, B)

#define VARIA_NO_OPERATION() ((void)0)

#define VARIA_ZERO_INIT(THIS_PTR) memset((THIS_PTR), 0, (sizeof(*THIS_PTR)))
#define VARIA_ZERO_INIT_SIZE(PTR, SIZE) memset((PTR), 0, SIZE)

//Note: For now, only support positive values
#define for_range(STOP)\
	assert(STOP >= 0);\
	for (size_t VARIA_CONCAT(_i, __LINE__) = 0; VARIA_CONCAT(_i, __LINE__) < (STOP); ++VARIA_CONCAT(_i, __LINE__))
#define for_range_var(CAPTURE, STOP)\
	assert(STOP >= 0);\
	for (size_t (CAPTURE) = 0; (CAPTURE) < (STOP); ++(CAPTURE))
#define for_reverse_range(START)\
	assert(START >= 0);\
	for (size_t VARIA_CONCAT(_i, __LINE__) = (START); VARIA_CONCAT(_i, __LINE__)--;)
#define for_reverse_range_var(CAPTURE, START)\
	assert(START >= 0);\
	for (size_t (CAPTURE) = (START); (CAPTURE)--;)
#define for_range_var_slice(CAPTURE, START, STOP)\
	assert(START >= 0 && STOP >= 0);\
	for (size_t (CAPTURE) = START; (CAPTURE) < (STOP); ++(CAPTURE))
#define for_range_var_step(CAPTURE, STOP, STEP)\
	assert(STOP >= 0);\
	for (size_t (CAPTURE) = 0; (CAPTURE) < (STOP); (CAPTURE) += (STEP))