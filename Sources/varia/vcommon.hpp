#pragma once

//Includes types and capabilities that are basically used everywhere

//:::::::::Basic Types::::::::::

#include <stdint.h>

typedef int8_t   integer_8;
typedef int16_t  integer_16;
typedef int32_t  integer_32;
typedef int64_t  integer_64;

typedef uint8_t  signless_8;
typedef uint16_t signless_16;
typedef uint32_t signless_32;
typedef uint64_t signless_64;

typedef float    float_32;
typedef double   float_64;

typedef unsigned char uchar;
typedef unsigned char byte;

//::::::::::::::::::::::::::::::


#ifdef _MSC_VER
	#define VARIA_INLINE static __forceinline
#else
	#define VARIA_INLINE static __attribute__((always_inline))
#endif

#define VARIA_STRINGIFY_IMPL(X) #X
#define VARIA_STRINGIFY(X) VARIA_STRINGIFY_IMPL(X)

#define VARIA_CONCAT_IMPL(A, B) A##B
#define VARIA_CONCAT(A, B) VARIA_CONCAT_IMPL(A, B)
#define VARIA_MACRO_VAR(VAR_NAME) VARIA_CONCAT(___, VARIA_CONCAT(__LINE__, VAR_NAME))

#define VARIA_NO_OPERATION() ((void)0)

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
	for (integer_64 VARIA_MACRO_VAR(_i) = 0; VARIA_MACRO_VAR(_i) < (STOP); VARIA_MACRO_VAR(_i) += 1)

#define for_range_var(CAPTURE, STOP)\
	assert(STOP >= 0);\
	for (integer_64 (CAPTURE) = 0; (CAPTURE) < (STOP); (CAPTURE) += 1)

#define for_reverse_range(START)\
	assert(START > 0);\
	for (integer_64 VARIA_MACRO_VAR(_i) = (START) - 1; VARIA_MACRO_VAR(_i) >= 0; VARIA_MACRO_VAR(_i) -= 1)

#define for_reverse_range_var(CAPTURE, START)\
	assert(START > 0);\
	for (integer_64 (CAPTURE) = (START) - 1; (CAPTURE) >= 0; (CAPTURE) -= 1)
