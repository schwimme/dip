#pragma once


#include "error_code_base.h"

#include <sys/types/string.h>


struct exception_t
// : std::exception  KTTODO - this inheritance is probably not needed
{
	exception_t(const sys::string& data, error_t code = E_COMMON_ERROR):
		m_data(data),
		m_code(code)
	{}

	exception_t(sys::string&& data, error_t code = E_COMMON_ERROR) :
		m_data(std::move(data)),
		m_code(code)
	{}

	exception_t(error_t code):
		m_code(code)
	{}

	sys::string m_data;
	error_t m_code;
};
