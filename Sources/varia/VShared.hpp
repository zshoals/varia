#pragma once

#include <type_traits>

//Includes types and capabilities that are basically used everywhere


#include "varia/ds/VDS-Types.hpp"
struct Enum_Index
{
	Integer_64 i;
};

template <typename E>
Enum_Index enum_as_index(E enumeration)
{
	static_assert(std::is_enum<E>::value, "Provided value was not an enum.");

	Enum_Index idx = { static_cast<Integer_64>(enumeration) };
	return idx;
};

#include <assert.h>
#define VARIA_ASSERT(COND, MSG)\
	if (!(COND)) { __debugbreak(); assert(0 && (MSG)); }

#define VARIA_UNREACHABLE(MSG) VARIA_ASSERT(0, MSG)

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

//NOTE(<zshoals> 07-18-2023): There's a chance there's some crazy C++ subtleties missing by using these functions
//	use them at your own risk
template <typename T>
T * address_of(T & element)
{
	return &(element);
}

template <typename T>
T const * const_address_of(T const & element)
{
	return &(element);
}

template <typename T>
T & dereference(T * const element)
{
	return *(element);
}

template <typename T>
T const & dereference(T const * const element)
{
	return *(element);
}

static inline void varia_toggle(Boolean * b)
{
	*b = *b ? false : true;
}


#define VARIA_BYTES(N) (N)
#define VARIA_KILOBYTES(N) ((N) * 1000)
#define VARIA_MEGABYTES(N) ((N) * 1000 * 1000)

//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                   For range loops for convenience                   ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

//Note: For now, only support positive values
#define for_range(STOP)\
	assert((STOP) >= 0);\
	for (Integer_64 VARIA_MACRO_VAR(_i) = 0; VARIA_MACRO_VAR(_i) < (STOP); VARIA_MACRO_VAR(_i) += 1)

#define for_range_var(CAPTURE, STOP)\
	assert((STOP) >= 0);\
	for (Integer_64 (CAPTURE) = 0; (CAPTURE) < (STOP); (CAPTURE) += 1)

#define for_reverse_range(START)\
	assert((START) > 0);\
	for (Integer_64 VARIA_MACRO_VAR(_i) = (START) - 1; VARIA_MACRO_VAR(_i) >= 0; VARIA_MACRO_VAR(_i) -= 1)

#define for_reverse_range_var(CAPTURE, START)\
	assert((START) > 0);\
	for (Integer_64 (CAPTURE) = (START) - 1; (CAPTURE) >= 0; (CAPTURE) -= 1)
