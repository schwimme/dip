#include "file_reader.h"
#include <fstream>
#include <error/exceptions.h>
#include <locale>
#include <codecvt>

namespace sys
{

template<typename _Dst, typename _Src>
std::basic_string<_Dst> convert(const std::basic_string<_Src>& src)
{
	return src;
}

template<>
std::basic_string<wchar_t> convert(const std::basic_string<char>& src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(src);
}


template<>
std::basic_string<char> convert(const std::basic_string<wchar_t>& src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(src);
}


sys::string read_file(const sys::string& file)
{
	std::string cvtfile = convert<char, sys::char_t>(file);

	std::ifstream file_stream(cvtfile);
	if (file_stream.is_open() == false)
	{
		throw exception_t(E_COULD_NOT_OPEN_FILE);
	}

	std::string output;
	output.assign((std::istreambuf_iterator<char>(file_stream)),
		(std::istreambuf_iterator<char>()));

	return convert<sys::char_t, char>(output);
}

}