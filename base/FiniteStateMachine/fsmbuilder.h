#pragma once


#include <vector>
#include <base_intf/FiniteStateMachine/fsmdefs.h>
#include <sys/types/string.h>

namespace base
{


class fsm_impl;


namespace fsm
{

/*
	KTTODO - for start only simple regex is supported:
	e.g.
	[a-z]{3}(ab)*
	Completly adding unix regex has to be implemented in future.
*/

class builder
{
protected:
	struct regex_item
	{
		enum class type_t
		{
			NORMAL,				// For standart character
			RANGE,				// For [a-z] type
			ITERATION,			// For (x)*
			POSITIVE_ITERATION,	// For (x)+
			NUMERIC_ITERATION,	// For x{2}
		};

		sys::char_t char1 = 0;
		sys::char_t char2 = 0;
		size_t      iteration_begin	= 0;
		size_t      iteration_count	= 0;
		type_t      type			= type_t::NORMAL;
	};

	using regex = std::vector<regex_item>;

public:
	void build_fsm_from_regex(fsm_impl& f, const sys::string& rgx, context_id valid, context_id invalid) const;

protected:
	// KTTODO - other class and remove virtual
	virtual regex parse_regex(const sys::char_t*& rgx) const;

	void CheckEnd(const sys::char_t* rest) const
	{
		if (*rest == 0)
		{
			throw "KTTODO";
		}
	}

	void CheckIs(const sys::char_t* rest, sys::char_t expected) const
	{
		if (*rest != expected)
		{
			throw "KTTODO";
		}
	}

	void ThrowParsingException(const sys::char_t* rest) const
	{
		throw "KTTODO";
	}

	sys::char_t parse_character(const sys::char_t*& rest, builder::regex& out) const;

	regex_item parse_range(const sys::char_t*& rest) const;

};


}
}
