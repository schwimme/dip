#pragma once


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


namespace base
{


using char_t = CHAR_TYPE;


}