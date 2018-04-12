#pragma once


#ifdef _DEBUG

#include "debug_impl.h"


#define ASSERT(condition) \
	do { \
		if ((condition) == false) \
		{\
			sys::debug::detail::AssertImpl(__FILE__, __LINE__, "[ASSERT FAILED] ("#condition ")");\
		}\
	} while (0)


#define ASSERT_NO_EVAL(condition) ASSERT(condition)

#define BREAK_POINT() \
	sys::debug::detail::BreakPointImpl();

#else // WITH_DEBUG


#define ASSERT(c) do { (c); } while (0)
#define ASSERT_NO_EVAL(c) 
#define BREAK_POINT()


#endif

