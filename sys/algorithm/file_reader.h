#pragma once


#include "../types/string.h"
#include <locale>
#include <codecvt>


namespace sys
{

sys::string read_file(const sys::string& file);

template<typename _Dst, typename _Src>
static std::basic_string<_Dst> convert(const std::basic_string<_Src>& src)
{
	return src;
}

template<>
static std::basic_string<wchar_t> convert(const std::basic_string<char>& src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(src);
}


template<>
static std::basic_string<char> convert(const std::basic_string<wchar_t>& src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(src);
}

}