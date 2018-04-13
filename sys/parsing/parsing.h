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

	void check_is(const sys::char_t* rest, sys::char_t e1) const
	{
		if (*rest != e1)
		{
			throw exception_t("unexpected character", _ExCheckIs);
		}
	}

	void check_is(const sys::char_t* rest, sys::char_t e1, sys::char_t e2) const
	{
		if (*rest != e1 && *rest != e2)
		{
			throw exception_t("unexpected character", _ExCheckIs);
		}
	}

	const sys::char_t* find_next_of(const sys::char_t* rest, sys::char_t ch1) const
	{
		const sys::char_t* p = rest;
		while (*p != ch1)
		{
			check_end(p);
			++p;
		}

		return p;
	}

	const sys::char_t* find_next_of(const sys::char_t* rest, sys::char_t ch1, sys::char_t ch2) const
	{
		const sys::char_t* p = rest;
		while (*p != ch1 && *p != ch2)
		{
			check_end(p);
			++p;
		}

		return p;
	}

};


}
