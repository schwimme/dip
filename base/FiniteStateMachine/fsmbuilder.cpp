#include "fsmbuilder.h"
#include "fsm.h"
#include "regexcontextfactory.h"

#include <sys/debugging/debug.h>


namespace base
{
namespace fsm
{


void builder::add_character(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx) const
{
	// - Generate new state and make rule from actual to new one:
	state_id new_state = fsm.generate_state(invalid);
	fsm.add_rule(actual_position, new_state, *p_rgx);

	// - update actual_state
	actual_position = new_state;

	// - Continue with next character:
	++p_rgx;
}


void builder::add_sequence(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx, bool iteration, bool optional) const
{
	// Check opening brace:
	check_is(p_rgx, TEXT('('));
	++p_rgx;
	check_end(p_rgx);

	// Mark significant states:
	state_id it_begin = actual_position;
	state_id it_end = fsm.generate_state(invalid);

	// process everithing until closing brace
	while (*p_rgx != TEXT(')'))
	{
		add_regex(fsm, actual_position, invalid, p_rgx);
	}

	++p_rgx;

	fsm.add_rule(actual_position, it_end);

	if (iteration)
	{
		fsm.add_rule(it_end, it_begin);
	}

	if (optional)
	{
		fsm.add_rule(it_begin, it_end);
	}

	actual_position = it_end;
}


void builder::add_range(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx) const
{
	check_is(p_rgx, TEXT('('));

	++p_rgx;
	check_end(p_rgx);
	sys::char_t a = *p_rgx;

	++p_rgx;
	check_end(p_rgx);
	sys::char_t b = *p_rgx;

	++p_rgx;
	check_is(p_rgx, TEXT(')'));

	++p_rgx;

	state_id dest = fsm.generate_state(invalid);
	fsm.add_rule(actual_position, dest, a, b);

	actual_position = dest;
}


void builder::add_selection(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx) const
{
	check_is(p_rgx, TEXT('('));
	++p_rgx;
	check_end(p_rgx);

	state_id destination_state = fsm.generate_state(invalid);

	while (*p_rgx != TEXT(')'))
	{
		state_id new_state = fsm.generate_state(invalid);

		fsm.add_rule(actual_position, new_state);
		add_regex(fsm, new_state, invalid, p_rgx);
		fsm.add_rule(new_state, destination_state);
	}

	++p_rgx;
	actual_position = destination_state;
}


void builder::add_regex(fsm_impl& fsm, state_id& actual_position, context_id invalid, const sys::char_t*& p_rgx) const
{
	if (get_character(p_rgx))
	{
		// Escaped character:
		add_character(fsm, actual_position, invalid, p_rgx);
		return;
	}

	// Handle non escaped character:
	switch (*p_rgx)
	{
	case TEXT('*'): // Iteration
	{
		++p_rgx;
		add_sequence(fsm, actual_position, invalid, p_rgx, /*iteration*/ true, /*optional*/ true);
		break;
	}

	case TEXT('+'): // Positive iteration
	{
		++p_rgx;
		add_sequence(fsm, actual_position, invalid, p_rgx, /*iteration*/ true, /*optional*/ false);
		break;
	}

	case TEXT('!'): // Optional sequence
	{
		++p_rgx;
		add_sequence(fsm, actual_position, invalid, p_rgx, /*iteration*/ false, /*optional*/ true);
		break;
	}

	case TEXT('.'): // Sequence
	{
		++p_rgx;
		add_sequence(fsm, actual_position, invalid, p_rgx, /*iteration*/ false, /*optional*/ false);

		break;
	}

	case TEXT('?'): // Selection
	{
		++p_rgx;
		add_selection(fsm, actual_position, invalid, p_rgx);
		break;
	}

	case TEXT('-'): // Range
	{
		++p_rgx;
		add_range(fsm, actual_position, invalid, p_rgx);
		break;
	}

	case TEXT(')'): // Return from recursion:
	{
		return;
	}

	default: // standard character:
	{
		add_character(fsm, actual_position, invalid, p_rgx);
		break;
	}

	}
}


void builder::build_fsm_from_regex(fsm_impl& fsm, const sys::string& rgx, context_id valid, context_id invalid) const
{
	// Prepare output fsm:
	state_id fsm_start = fsm.generate_state(invalid);
	state_id fsm_end = fsm.generate_state(valid);
	fsm.set_start(fsm_start);

	// Helpers:
	state_id actual_position = fsm_start;

	// C-string:
	const sys::char_t* p_rgx = rgx.c_str();

	while (*p_rgx)
	{
		add_regex(fsm, actual_position, invalid, p_rgx);
	}

	fsm.add_rule(actual_position, fsm_end);

	// This fsm can be fully optimized because all generated states are just internal:
	fsm.optimize(fsm_impl::optimization_level_t::MINIMIZE);
}


bool builder::get_character(const sys::char_t*& ch) const
{
	if (*ch != TEXT('\\'))
	{
		// Simple char:
		return false;
	}

	// Escaped character preceded by '\'.
	++ch;
	check_end(ch);

	return true;
}


}
}
