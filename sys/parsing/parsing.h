#pragma once


#include <error/error_code_base.h>
#include <error/exceptions.h>


namespace sys
{


template<error_t _ExEnd, error_t _ExCheckIs = _ExEnd>
class parsing_base
{
protected:
	void check_end(const sys::char_t* rest) const
	{
		if (*rest == 0)
		{
			throw exception_t("unexpected end", _ExEnd);
		}
	}

	template<typename ... _Chr>
	void check_is(const sys::char_t* rest, sys::char_t c, _Chr ... cp) const
	{
		if (*rest != c)
		{
			check_is(rest, cp...);
		}
	}

	void check_is(const sys::char_t*) const
	{
		throw exception_t("unexpected character", _ExCheckIs);
	}

	template<typename ... _Chr>
	const char* find_next_of(const sys::char_t* rest, sys::char_t c, _Chr ... cp) const
	{
		const sys::char_t* p = rest;
		while (true)
		{
			check_end(p);
			const sys::char_t* r = find_next_of_impl(p, c, cp...);
			if (r != nullptr)
			{
				return r;
			}
			++p;
		}
	}

private:
	template<typename ... _Chr>
	const char* find_next_of_impl(const sys::char_t* rest, sys::char_t c, _Chr ... cp) const
	{
		if (*rest == c)
		{
			return rest;
		}

		return find_next_of_impl(rest, cp...);
	}

	const char* find_next_of_impl(const char*) const
	{
		return nullptr;
	}
};


}
