#pragma once

// KTTODO remove:
#include <iostream>

#ifdef WITH_ASSERTS
#	define ASSETS_DISABLED true
#else
#	define ASSETS_DISABLED false
#endif


namespace Base
{
namespace detail
{


template<bool IsAssertsDisabled>
static void AssertImpl(const char* file, int line, const char* msg)
{
	std::cout << "Not assert";
}


template<>
static void AssertImpl<false>(const char* file, int line, const char* msg)
{
	std::cout << "[Assert] - " << file << ':' << line << '-' << msg << std::endl;
}


}
}


#define VERIFY(condition) \
	do { \
		if ((condition) == false) \
		{\
			Base::detail::AssertImpl<ASSETS_DISABLED>(__FILE__, __LINE__, #condition " failed");\
		}\
	} while (0)
