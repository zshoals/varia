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


//Note(zshoals 01-24-2023):> Maybe too clunky to use in practice
//taken from https://stackoverflow.com/questions/13842468/comma-in-c-c-macro
//Actually should prefer "stacknew" vscode snippet, which generates this pattern without
//relying on macro weirdness. Kept for posterity.
#define VARIA_GROUP_ARGS(...) __VA_ARGS__
#define VARIA_STACK_NEW(TYPE, HANDLE)\
	TYPE VARIA_CONCAT(_trashme_, HANDLE);\
	TYPE * HANDLE = &(VARIA_CONCAT(_trashme_, HANDLE))


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