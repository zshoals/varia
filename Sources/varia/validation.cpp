#include "varia/validation.hpp"

#include "kinc/log.h"
#include <math.h>

#define VARIA_VALIDATION_NO_OP() ((void)0)
#define VARIA_VALIDATION_FLOAT_EPSILON 0.00000001

//======================================
//=========INT CHECKS===================
//======================================

void varia_validation_ensure_int_gt(long long a, long long b, char const * message, char const * file, int line)
{
	if (a > b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{
		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %lld greater than %lld", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a > b, 0) );
	}
}
void varia_validation_ensure_int_gte(long long a, long long b, char const * message, char const * file, int line)
{
	if (a >= b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %lld greater than or equal to %lld", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a >= b, 0) );
	}
}
void varia_validation_ensure_int_lt(long long a, long long b, char const * message, char const * file, int line)
{
	if (a < b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %lld less than %lld", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a < b, 0) );
	}
}
void varia_validation_ensure_int_lte(long long a, long long b, char const * message, char const * file, int line)
{
	if (a <= b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %lld less than or equal to %lld", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a <= b, 0) );
	}
}
void varia_validation_ensure_int_equals(long long a, long long b, char const * message, char const * file, int line)
{
	if (a == b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %lld equals %lld", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a == b, 0) );
	}
}
void varia_validation_ensure_int_in_range(long long value, long long lower, long long upper, char const * message, char const * file, int line)
{
	if (lower <= value && value < upper)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{
		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %lld to be in range of lower range %lld and upper range %lld.", value, lower, upper);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, lower <= value && value < upper, 0) );
	}
}


//================================
//=====UNSIGNED INT CHECKS========
//================================

void varia_validation_ensure_uint_gt(unsigned long long a, unsigned long long b, char const * message, char const * file, int line)
{
	if (a > b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %llu greater than %llu", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a > b, 0) );
	}
}
void varia_validation_ensure_uint_gte(unsigned long long a, unsigned long long b, char const * message, char const * file, int line)
{
	if (a >= b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %llu greater than or equal to %llu", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a >= b, 0) );
	}
}
void varia_validation_ensure_uint_lt(unsigned long long a, unsigned long long b, char const * message, char const * file, int line)
{
	if (a < b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %llu less than %llu", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a < b, 0) );
	}
}
void varia_validation_ensure_uint_lte(unsigned long long a, unsigned long long b, char const * message, char const * file, int line)
{
	if (a <= b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %llu less than or equal to %llu", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a <= b, 0) );
	}
}
void varia_validation_ensure_uint_equals(unsigned long long a, unsigned long long b, char const * message, char const * file, int line)
{
	if (a == b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %llu equals %llu", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a == b, 0) );
	}
}
void varia_validation_ensure_uint_in_range(unsigned long long value, unsigned long long lower, unsigned long long upper, char const * message, char const * file, int line)
{
	if (lower <= value && value < upper)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected value %llu to be in range of lower range %llu and upper range %llu", value, lower, upper);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, lower <= value && value < upper, 0) );
	}
}

//======================================
//=====UNSIGNED FLOAT/DOUBLE CHECKS=====
//======================================

void varia_validation_ensure_float_gt(long double a, long double b, char const * message, char const * file, int line)
{
	if (a > b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %Lf greater than %Lf", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a > b, 0) );
	}
}
void varia_validation_ensure_float_gte(long double a, long double b, char const * message, char const * file, int line)
{
	if (a >= b || fabs(a - b) < VARIA_VALIDATION_FLOAT_EPSILON)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %Lf greater than or equal to %Lf", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a >= b || fabs(a - b) < VARIA_VALIDATION_FLOAT_EPSILON, 0) );
	}
}
void varia_validation_ensure_float_lt(long double a, long double b, char const * message, char const * file, int line)
{
	if (a < b)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %Lf less than %Lf", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a < b, 0) );
	}
}
void varia_validation_ensure_float_lte(long double a, long double b, char const * message, char const * file, int line)
{
	if (a <= b || fabs(a - b) < VARIA_VALIDATION_FLOAT_EPSILON)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %Lf less than or equal to %Lf", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, a <= b || fabs(a - b) < VARIA_VALIDATION_FLOAT_EPSILON, 0) );
	}
}
void varia_validation_ensure_float_equals(long double a, long double b, char const * message, char const * file, int line)
{
	if (fabs(a - b) < VARIA_VALIDATION_FLOAT_EPSILON)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %Lf equals %Lf", a, b);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, fabs(a - b) < VARIA_VALIDATION_FLOAT_EPSILON, 0) );
	}
}
void varia_validation_ensure_float_in_range(long double value, long double lower, long double upper, char const * message, char const * file, int line)
{
	if ( (lower <= value || fabs(lower - value) < VARIA_VALIDATION_FLOAT_EPSILON) && value < upper)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Expected %Lf in the range of lower range %Lf and upper range %Lf", value, lower, upper);
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, ((lower <= value || fabs(lower - value) < VARIA_VALIDATION_FLOAT_EPSILON) && value < upper), 0) );
	}
}

//======================================
//=====POINTER CHECKS===================
//======================================

void varia_validation_ensure_ptr_not_null(void * ptr, char const * message, char const * file, int line)
{
	if (ptr)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Pointer was null!");
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, ptr, 0) );
	}
}
void varia_validation_ensure_ptr_is_null(void * ptr, char const * message, char const * file, int line)
{
	if (!ptr)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{
		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Pointer was not null!");
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, !ptr, 0) );
	}
}
void varia_validation_ensure_const_ptr_not_null(void const * ptr, char const * message, char const * file, int line)
{
	if (ptr)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{

		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Const Pointer was null!");
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, ptr, 0) );
	}
}
void varia_validation_ensure_const_ptr_is_null(void const * ptr, char const * message, char const * file, int line)
{
	if (!ptr)
	{
		VARIA_VALIDATION_NO_OP();
	}
	else
	{
		kinc_log(KINC_LOG_LEVEL_ERROR, "\nASSERTION FAILED::[FILE: %s]::[LINE: %d]", file, line);
		kinc_log(KINC_LOG_LEVEL_ERROR, "Const Pointer was not null!");
		kinc_log(KINC_LOG_LEVEL_ERROR, "User Message: %s", message);
		assert( (message, !ptr, 0) );
	}
}