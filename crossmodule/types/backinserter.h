#pragma once

#include <cstdint>

namespace crossmodule
{


// KTTODO - move else.
using error_t = int32_t;

template<typename T>
struct back_inserter
{
	virtual error_t push_back(const T&) = 0;
	virtual error_t push_back(T&&) = 0;
	virtual error_t reserve(size_t count) = 0;
};


}
