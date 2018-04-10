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
public:
	void build_fsm_from_regex(fsm_impl& f, const sys::string& rgx, context_id valid, context_id invalid) const;

protected:
	void check_end(const sys::char_t* rest) const
	{
		if (*rest == 0)
		{
			throw "KTTODO";
		}
	}

	void check_is(const sys::char_t* rest, sys::char_t expected) const
	{
		if (*rest != expected)
		{
			throw "KTTODO";
		}
	}

	void throw_parse_error(const sys::char_t* rest) const
	{
		throw "KTTODO";
	}

	bool get_character(const sys::char_t*& ch) const;
	void add_regex(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx) const;
	void add_character(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx) const;
	void add_range(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx) const;
	void add_selection(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx) const;
	
	void add_sequence(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx, bool iteration, bool optional) const;
};


}
}
