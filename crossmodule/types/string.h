#pragma once


// KTTODO - there must be used cross::char_t
#include <sys/types/chartype.h>
#include <cstring>


namespace cross
{


struct string_ref
{
	template<size_t N>
	string_ref(const sys::char_t (&data)[N]):
		m_data(data),
		m_size(N)
	{}

	string_ref(const sys::char_t * const data, size_t size):
		m_data(data),
		m_size(size)
	{}

	const sys::char_t* const m_data;
	size_t m_size;
};


struct settable_string_ref
{
	virtual bool set(sys::char_t const* data, size_t size) noexcept = 0;
};


}
