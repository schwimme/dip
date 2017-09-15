#pragma once


#include <string>

#ifdef TEXT
#	undef TEXT
#endif

#ifdef CHAR_TYPE
#	undef CHAR_TYPE
#endif

#ifdef USING_UNICODE
#	define CHAR_TYPE wchar_t
#	define TEXT(txt) L##txt
#else
#	define CHAR_TYPE char
#	define TEXT(txt) txt
#endif

namespace Base
{
	using CharType = CHAR_TYPE;
	using String = std::basic_string<CharType>;

	template<typename T>
	String ToString(const T& e)
	{
#ifdef USING_UNICODE
		return std::to_wstring(e);
#else
		return std::to_string(e);
#endif

	}
}