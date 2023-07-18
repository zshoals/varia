#pragma once

//Includes types and capabilities that are basically used everywhere


#include "varia/ds/VDS-Types.hpp"


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



#define ZERO_INIT() {}

//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                   For range loops for convenience                   ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

//Note: For now, only support positive values
#define for_range(STOP)\
	assert(STOP >= 0);\
	for (Integer_64 VARIA_MACRO_VAR(_i) = 0; VARIA_MACRO_VAR(_i) < (STOP); VARIA_MACRO_VAR(_i) += 1)

#define for_range_var(CAPTURE, STOP)\
	assert(STOP >= 0);\
	for (Integer_64 (CAPTURE) = 0; (CAPTURE) < (STOP); (CAPTURE) += 1)

#define for_reverse_range(START)\
	assert(START > 0);\
	for (Integer_64 VARIA_MACRO_VAR(_i) = (START) - 1; VARIA_MACRO_VAR(_i) >= 0; VARIA_MACRO_VAR(_i) -= 1)

#define for_reverse_range_var(CAPTURE, START)\
	assert(START > 0);\
	for (Integer_64 (CAPTURE) = (START) - 1; (CAPTURE) >= 0; (CAPTURE) -= 1)
