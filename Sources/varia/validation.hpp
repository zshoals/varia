#pragma once

#ifndef NDEBUG
	#include <assert.h>
#else //Assert was disabled, but we want our assertion kit to always be enabled
	#undef NDEBUG
	#include <assert.h>
	#define NDEBUG
#endif



void varia_custom_assert(bool condition, char const * expr, char const * message, char const * file, int line);
void varia_custom_assert_release(bool condition, char const * expr, char const * message, char const * file, int line);

#ifndef VARIA_DISABLE_UNFORCED_ASSERTS
	#define DEBUG_ENSURE(Expression, Message) varia_custom_assert(Expression, #Expression, Message, __FILE__, __LINE__)
	#define DEBUG_ENSURE_KILL_NOW(Message) varia_custom_assert(false, "DEBUG ERROR", Message, __FILE__, __LINE__)
#else
	#define DEBUG_ENSURE(Expression, Message) ((void)0)
	#define DEBUG_ENSURE_KILL_NOW(Message) ((void)0)
#endif


#define ENSURE(Expression, Message) varia_custom_assert_release(Expression, #Expression, Message, __FILE__, __LINE__)
//Something must die if you reach this point
#define ENSURE_KILL_NOW(Message) assert(false && "RUNTIME ERROR: Emergency Kill Now in file: " __FILE__)
//Intended for things like switch cases with unreachable and unwanted default outcomes, for instance
#define ENSURE_UNREACHABLE(Message) assert(false && "RUNTIME ERROR: Unreachable was reached in file: " __FILE__)
//Intended for unimplemented functions and features, maybe we should add function name support
#define ENSURE_UNIMPLEMENTED() assert(false && "RUNTIME ERROR: Function is unimplemented in file: " __FILE__)

