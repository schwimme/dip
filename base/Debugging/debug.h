#pragma once

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
static void AssertImpl(const char*, int, const char*) {}


template<>
static void AssertImpl<false>(const char* file, int line, const char* msg)
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


#define VERIFY(condition) \
	do { \
		if ((condition) == false) \
		{\
			Base::Debugging::AssertImpl<ASSERTS_DISABLED>(__FILE__, __LINE__, "[ASSERT FAILED] ("#condition ")");\
		}\
	} while (0)


#ifdef WITH_ASSERTS
#	define VERIFY_NO_EVAL(condition) VERIFY(condition)
#else
#	define VERIFY_NO_EVAL(condition) (void)(0)
#endif
