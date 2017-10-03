#pragma once


#ifdef WITH_DEBUG

#include "debug_impl.h"


#ifdef WITH_ASSERTS
#	define ASSERTS_DISABLED true
#else
#	define ASSERTS_DISABLED false
#endif


#ifdef WITH_BREAKPOINTS
#	define BREAK_POINTS_DISABLED true
#else
#	define BREAK_POINTS_DISABLED false
#endif

namespace Base
{
namespace Debugging
{


template<bool IsAssertsDisabled>
static void Assert(const char*, int, const char*) {}


template<>
static void Assert<false>(const char* file, int line, const char* msg)
{
	detail::AssertImpl(file, line, msg);
}


template<bool IsBreakPointsDisabled>
static void BreakPointImpl() {}


template<>
static void BreakPointImpl<false>()
{
	detail::BreakPointImpl();
}


}
}


#define ASSERT(condition) \
	do { \
		if ((condition) == false) \
		{\
			Base::Debugging::Assert<ASSERTS_DISABLED>(__FILE__, __LINE__, "[ASSERT FAILED] ("#condition ")");\
		}\
	} while (0)


#ifdef WITH_ASSERTS
#	define ASSERT_NO_EVAL(condition) ASSERT(condition)
#else
#	define ASSERT_NO_EVAL(condition) (void)(0)
#endif


#else // WITH_DEBUG


#define ASSERT(c) do { (c); } while (0)
#define ASSERT_NO_EVAL(c) 

#endif

