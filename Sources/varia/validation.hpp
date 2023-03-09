#pragma once

#ifndef NDEBUG
	#include <assert.h>
#else //Assert was disabled, but we want our assertion kit to always be enabled
	#undef NDEBUG
	#include <assert.h>
	#define NDEBUG
#endif

void varia_validation_ensure_int_gt(long long a, long long b, char const * message, char const * file, int line);
void varia_validation_ensure_int_gte(long long a, long long b, char const * message, char const * file, int line);
void varia_validation_ensure_int_lt(long long a, long long b, char const * message, char const * file, int line);
void varia_validation_ensure_int_lte(long long a, long long b, char const * message, char const * file, int line);
void varia_validation_ensure_int_equals(long long a, long long b, char const * message, char const * file, int line);
void varia_validation_ensure_int_in_range(long long value, long long lower, long long upper, char const * message, char const * file, int line);

void varia_validation_ensure_uint_gt(unsigned long long a, unsigned long long b, char const * message, char const * file, int line);
void varia_validation_ensure_uint_gte(unsigned long long a, unsigned long long b, char const * message, char const * file, int line);
void varia_validation_ensure_uint_lt(unsigned long long a, unsigned long long b, char const * message, char const * file, int line);
void varia_validation_ensure_uint_lte(unsigned long long a, unsigned long long b, char const * message, char const * file, int line);
void varia_validation_ensure_uint_equals(unsigned long long a, unsigned long long b, char const * message, char const * file, int line);
void varia_validation_ensure_uint_in_range(unsigned long long value, unsigned long long lower, unsigned long long upper, char const * message, char const * file, int line);

void varia_validation_ensure_float_gt(long double a, long double b, char const * message, char const * file, int line);
void varia_validation_ensure_float_gte(long double a, long double b, char const * message, char const * file, int line);
void varia_validation_ensure_float_lt(long double a, long double b, char const * message, char const * file, int line);
void varia_validation_ensure_float_lte(long double a, long double b, char const * message, char const * file, int line);
void varia_validation_ensure_float_equals(long double a, long double b, char const * message, char const * file, int line);
void varia_validation_ensure_float_in_range(long double value, long double lower, long double upper, char const * message, char const * file, int line);

void varia_validation_ensure_ptr_not_null(void * ptr, char const * message, char const * file, int line);
void varia_validation_ensure_ptr_is_null(void * ptr, char const * message, char const * file, int line);

void varia_validation_ensure_const_ptr_not_null(void const * ptr, char const * message, char const * file, int line);
void varia_validation_ensure_const_ptr_is_null(void const * ptr, char const * message, char const * file, int line);

void varia_custom_assert(bool condition, char const * expr, char const * message, char const * file, int line);

#ifndef VARIA_DISABLE_UNFORCED_ASSERTS

	#define DEBUG_ASSERT(Expression, Message) varia_custom_assert(Expression, #Expression, Message, __FILE__, __LINE__)


	#define DEBUG_ENSURE_TRUE(Expression, Message) assert(( (Message), (Expression) ))
	#define DEBUG_ENSURE_FALSE(Expression, Message) assert(( (Message), !(Expression) ))
	#define DEBUG_ENSURE_KILL_NOW(Message) assert(( (Message), 0 ))

	#define DEBUG_ENSURE_INT_GT(A, B, Message) varia_validation_ensure_int_gt(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_INT_GTE(A, B, Message) varia_validation_ensure_int_gte(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_INT_LT(A, B, Message) varia_validation_ensure_int_lt(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_INT_LTE(A, B, Message) varia_validation_ensure_int_lte(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_INT_EQUALS(A, B, Message) varia_validation_ensure_int_equals(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_INT_IN_RANGE(Value, Lower, Upper, Message) varia_validation_ensure_int_in_range(Value, Lower, Upper, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_INT_GT_ZERO(Value, Message) varia_validation_ensure_int_gt(Value, 0, Message, __FILE__, __LINE__)

	#define DEBUG_ENSURE_UINT_GT(A, B, Message) varia_validation_ensure_uint_gt(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_UINT_GTE(A, B, Message) varia_validation_ensure_uint_gte(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_UINT_LT(A, B, Message) varia_validation_ensure_uint_lt(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_UINT_LTE(A, B, Message) varia_validation_ensure_uint_lte(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_UINT_EQUALS(A, B, Message) varia_validation_ensure_uint_equals(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_UINT_IN_RANGE(Value, Lower, Upper, Message) varia_validation_ensure_uint_in_range(Value, Lower, Upper, Message,  __FILE__, __LINE__)
	#define DEBUG_ENSURE_UINT_GT_ZERO(Value, Message) varia_validation_ensure_uint_gt(Value, 0, Message, __FILE__, __LINE__)

	#define DEBUG_ENSURE_FLOAT_GT(A, B, Message) varia_validation_ensure_float_gt(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_FLOAT_GTE(A, B, Message) varia_validation_ensure_float_gte(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_FLOAT_LT(A, B, Message) varia_validation_ensure_float_lt(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_FLOAT_LTE(A, B, Message) varia_validation_ensure_float_lte(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_FLOAT_EQUALS(A, B, Message) varia_validation_ensure_float_equals(A, B, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_FLOAT_IN_RANGE(Value, Lower, Upper, Message) varia_validation_ensure_float_in_range(Value, Lower, Upper, Message,  __FILE__, __LINE__)

	#define DEBUG_ENSURE_PTR_NOT_NULL(Pointer, Message) varia_validation_ensure_ptr_not_null(Pointer, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_PTR_IS_NULL(Pointer, Message) varia_validation_ensure_ptr_is_null(Pointer, Message, __FILE__, __LINE__)

	#define DEBUG_ENSURE_CONST_PTR_NOT_NULL(Pointer, Message) varia_validation_ensure_const_ptr_not_null(Pointer, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_CONST_PTR_IS_NULL(Pointer, Message) varia_validation_ensure_const_ptr_is_null(Pointer, Message, __FILE__, __LINE__)

#else
	#define DEBUG_ENSURE_TRUE(Expression, Message) ((void)0)
	#define DEBUG_ENSURE_FALSE(Expression, Message) ((void)0)
	#define DEBUG_ENSURE_KILL_NOW(Message) ((void)0)

	#define DEBUG_ENSURE_INT_GT(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_INT_GTE(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_INT_LT(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_INT_LTE(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_INT_EQUALS(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_INT_IN_RANGE(Value, Lower, Upper, Message) ((void)0)
	#define DEBUG_ENSURE_INT_GT_ZERO(Value, Message) ((void)0)

	#define DEBUG_ENSURE_UINT_GT(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_UINT_GTE(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_UINT_LT(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_UINT_LTE(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_UINT_EQUALS(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_UINT_IN_RANGE(Value, Lower, Upper, Message) ((void)0)
	#define DEBUG_ENSURE_UINT_GT_ZERO(Value, Message) ((void)0)

	#define DEBUG_ENSURE_FLOAT_GT(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_FLOAT_GTE(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_FLOAT_LT(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_FLOAT_LTE(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_FLOAT_EQUALS(A, B, Message) ((void)0)
	#define DEBUG_ENSURE_FLOAT_IN_RANGE(Value, Lower, Upper, Message) ((void)0)

	#define DEBUG_ENSURE_PTR_NOT_NULL(Pointer, Message) ((void)0)
	#define DEBUG_ENSURE_PTR_IS_NULL(Pointer, Message) ((void)0)

	#define DEBUG_ENSURE_CONST_PTR_NOT_NULL(Pointer, Message) ((void)0)
	#define DEBUG_ENSURE_CONST_PTR_IS_NULL(Pointer, Message) ((void)0)
#endif


//These assertions are never disabled, even in release builds
//An important note: These may need have a different assert mechanism, otherwise it may enter a debugmode state
//in release to clients (not good!). Might have to change these to abort() or something instead,
//maybe with some sort of crash message box.
#define ENSURE_TRUE(Expression, Message) assert(( (Message), (Expression) ))
#define ENSURE_FALSE(Expression, Message) assert(( (Message), !(Expression) ))

#define ENSURE_INT_GT(A, B, Message) varia_validation_ensure_int_gt(A, B, Message, __FILE__, __LINE__)
#define ENSURE_INT_GTE(A, B, Message) varia_validation_ensure_int_gte(A, B, Message, __FILE__, __LINE__)
#define ENSURE_INT_LT(A, B, Message) varia_validation_ensure_int_lt(A, B, Message, __FILE__, __LINE__)
#define ENSURE_INT_LTE(A, B, Message) varia_validation_ensure_int_lte(A, B, Message, __FILE__, __LINE__)
#define ENSURE_INT_EQUALS(A, B, Message) varia_validation_ensure_int_equals(A, B, Message, __FILE__, __LINE__)
#define ENSURE_INT_IN_RANGE(Value, Lower, Upper, Message) varia_validation_ensure_int_in_range(Value, Lower, Upper, Message, __FILE__, __LINE__)
#define ENSURE_INT_GT_ZERO(Value, Message) varia_validation_ensure_int_gt(Value, 0, Message, __FILE__, __LINE__)
#define ENSURE_INT_GTE_ZERO(Value, Message) varia_validation_ensure_int_gte(Value, 0, Message, __FILE__, __LINE__)

#define ENSURE_UINT_GT(A, B, Message) varia_validation_ensure_uint_gt(A, B, Message, __FILE__, __LINE__)
#define ENSURE_UINT_GTE(A, B, Message) varia_validation_ensure_uint_gte(A, B, Message, __FILE__, __LINE__)
#define ENSURE_UINT_LT(A, B, Message) varia_validation_ensure_uint_lt(A, B, Message, __FILE__, __LINE__)
#define ENSURE_UINT_LTE(A, B, Message) varia_validation_ensure_uint_lte(A, B, Message, __FILE__, __LINE__)
#define ENSURE_UINT_EQUALS(A, B, Message) varia_validation_ensure_uint_equals(A, B, Message, __FILE__, __LINE__)
#define ENSURE_UINT_IN_RANGE(Value, Lower, Upper, Message) varia_validation_ensure_uint_in_range(Value, Lower, Upper, Message,  __FILE__, __LINE__)
#define ENSURE_UINT_GT_ZERO(Value, Message) varia_validation_ensure_uint_gt(Value, 0, Message, __FILE__, __LINE__)
#define ENSURE_UINT_GTE_ZERO(Value, Message) varia_validation_ensure_uint_gte(Value, 0, Message, __FILE__, __LINE__)

#define ENSURE_FLOAT_GT(A, B, Message) varia_validation_ensure_float_gt(A, B, Message, __FILE__, __LINE__)
#define ENSURE_FLOAT_GTE(A, B, Message) varia_validation_ensure_float_gte(A, B, Message, __FILE__, __LINE__)
#define ENSURE_FLOAT_LT(A, B, Message) varia_validation_ensure_float_lt(A, B, Message, __FILE__, __LINE__)
#define ENSURE_FLOAT_LTE(A, B, Message) varia_validation_ensure_float_lte(A, B, Message, __FILE__, __LINE__)
#define ENSURE_FLOAT_EQUALS(A, B, Message) varia_validation_ensure_float_equals(A, B, Message, __FILE__, __LINE__)
#define ENSURE_FLOAT_IN_RANGE(Value, Lower, Upper, Message) varia_validation_ensure_float_in_range(Value, Lower, Upper, Message,  __FILE__, __LINE__)

#define ENSURE_PTR_NOT_NULL(Pointer, Message) varia_validation_ensure_ptr_not_null(Pointer, Message, __FILE__, __LINE__)
#define ENSURE_PTR_IS_NULL(Pointer, Message) varia_validation_ensure_ptr_is_null(Pointer, Message, __FILE__, __LINE__)

//Something must die if you reach this point
#define ENSURE_KILL_NOW(Message) assert(( (Message), 0 ))
//Intended for things like switch cases with unreachable and unwanted default outcomes, for instance
#define ENSURE_UNREACHABLE(Message) assert(( (Message), 0 ))
//Intended for unimplemented functions and features, maybe we should add function name support
#define ENSURE_UNIMPLEMENTED() assert(( "Function is unimplemented.", 0 ))

