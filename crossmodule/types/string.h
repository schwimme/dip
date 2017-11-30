#pragma once


#include <types/chartype.h> // base::char_t
#include <cstring>			// size_t


namespace crossmodule
{


struct string_ref
{
	template<size_t N>
	string_ref(const base::char_t (&data)[N]):
		m_data(data),
		m_size(N)
	{}

	string_ref(const base::char_t * const data, size_t size):
		m_data(data),
		m_size(size)
	{}

	const base::char_t* const m_data;
	size_t m_size;
};


struct settable_string_ref
{
	virtual bool set(base::char_t const* data, size_t size) noexcept = 0;
};


}
