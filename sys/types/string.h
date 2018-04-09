#pragma once


#include <string>
#include "chartype.h"


namespace sys
{


using string = std::basic_string<char_t>;

template<typename T>
string to_string(const T& e)
{
#ifdef USING_UNICODE
	return std::to_wstring(e);
#else
	return std::to_string(e);
#endif
}


}