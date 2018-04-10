#pragma once

#include <cstdint>
#include <error/error_code_base.h>


namespace cross
{


template<typename T>
struct back_inserter
{
	virtual error_t push_back(const T&) = 0;
	virtual error_t push_back(T&&) = 0;
	virtual error_t reserve(size_t count) = 0;
};


}
