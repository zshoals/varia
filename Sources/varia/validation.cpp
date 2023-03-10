#include "varia/validation.hpp"

#include "kinc/log.h"
#include "log.hpp"
#include <math.h>

#define VARIA_VALIDATION_NO_OP() ((void)0)
#define VARIA_VALIDATION_FLOAT_EPSILON 0.00000001


void varia_custom_assert(bool condition, char const * expr, char const * message, char const * file, int line)
{
	if (!condition)
	{
		Glog_print(); //Write any global unlogged info out
		Glog_clear_buffer();

		kinc_log(KINC_LOG_LEVEL_ERROR, "| Assertion failed.\n|   %s \n|   Message: %s \n|   File: %s \n|   Line: %d \n", expr, message, file, line);

		#if defined(_MSC_VER)
			__debugbreak();
		#elif defined(__clang__)
			__builtin_debugtrap();
		#else
		#if defined(__aarch64__)
			__asm__ volatile(".inst 0xd4200000");
		#elif defined(__x86_64__)
			__asm__ volatile("int $0x03");
		#else
			kinc_log(KINC_LOG_LEVEL_WARNING, "Oh no, kinc_debug_break is not implemented for the current compiler and CPU.");
			assert(false && "No debugger available: aborting...");
		#endif
		#endif
	}
}


void varia_custom_assert_release(bool condition, char const * expr, char const * message, char const * file, int line)
{
	if (!condition)
	{
		Glog_print();
		Glog_clear_buffer();

		kinc_log(KINC_LOG_LEVEL_ERROR, "| Assertion failed.\n|   %s \n|   Message: %s \n|   File: %s \n|   Line: %d \n", expr, message, file, line);

		assert(false && "A critical runtime error has occurred. Please contact the developer.");
	}
}
