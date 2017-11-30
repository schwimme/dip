#include "fsmbuilder.h"
#include "fsm.h"
#include "regexcontextfactory.h"

#include <base/Debugging/debug.h>


namespace base
{
namespace fsm
{


void builder::build_fsm_from_regex(fsm_impl& fsm, const base::string& rgx, context_id valid, context_id invalid) const
{
	// Parse regex to internal structure:
	const base::char_t* t = rgx.c_str();
	regex parsedRegex = parse_regex(t);

	// Prepare output fsm:
	state_id fsmStart = fsm.generate_state(invalid);
	fsm.set_start(fsmStart);

	state_id fsmEnd = fsm.generate_state(valid);

	// Prepare mapping from parsed regex to fsm states:
	std::vector<fsm_impl::states_storage_t::iterator> newStates = { fsm.m_states.find(fsmStart) };

	// Generate:
	size_t regexSize = parsedRegex.size();
	for (size_t it = 0; it < regexSize; ++it)
	{
		const auto& currentParsedRegex = parsedRegex[it];
		switch (currentParsedRegex.type)
		{
		case regex_item::type_t::NORMAL:
		{
			// Generate state:
			state_id s = fsm.generate_state(invalid);

			// Current end:
			const state_id& b = newStates.at(newStates.size() - 1)->first;

			// Store reference to new states:
			newStates.push_back(fsm.m_states.find(s));

			// Add rule from starting position to newly generated state via character
			fsm.add_rule(b, s, currentParsedRegex.char1);
		}
		break;

		case regex_item::type_t::RANGE:
		{
			// Generate state:
			state_id s = fsm.generate_state(invalid);

			// Current end:
			const state_id& b = newStates.at(newStates.size() - 1)->first;

			// Store reference to new states:
			newStates.push_back(fsm.m_states.find(s));

			// Add rule from starting position to newly generated state via character
			fsm.add_rule(b, s, currentParsedRegex.char1, currentParsedRegex.char2);
		}
		break;

		case regex_item::type_t::ITERATION:
		{
			// Cannot iterate nothing:
			ASSERT_NO_EVAL(it != 0);

			// Insert last->epsilon->empty to not harm processing:
			state_id s = fsm.generate_state(invalid);
			fsm.add_rule(newStates.at(newStates.size() - 1)->first, s);
			newStates.push_back(fsm.m_states.find(s));

			// Register loop:
			fsm.add_rule(s, newStates[currentParsedRegex.iteration_begin]->first);

			// Register epsilon rule from begin iteration to here to allow skip all the loop:
			fsm.add_rule(newStates[currentParsedRegex.iteration_begin]->first, s);
		}
		break;

		case regex_item::type_t::POSITIVE_ITERATION:
		{
			// Cannot iterate nothing:
			ASSERT_NO_EVAL(it != 0);

			// Insert last->epsilon->empty to not harm processing:
			state_id s = fsm.generate_state(invalid);
			fsm.add_rule(newStates.at(newStates.size() - 1)->first, s);
			newStates.push_back(fsm.m_states.find(s));

			// Register loop:
			fsm.add_rule(s, newStates[currentParsedRegex.iteration_begin]->first);
		}
		break;

		case regex_item::type_t::NUMERIC_ITERATION:
		{

		}
		break;

		default:
			break;
		}
	}

	// Add end rule:
	fsm.add_rule(newStates.at(newStates.size() - 1)->first, fsmEnd);

	// This fsm can be fully optimized because all generated states are just internal:
	fsm.optimize(fsm_impl::optimization_level_t::MINIMIZE);
}


builder::regex builder::parse_regex(const base::char_t*& rgx) const
{
	regex outputStack;

	while (*rgx)
	{
		base::char_t c = parse_character(rgx, outputStack);

		switch (c)
		{
		case TEXT('['):
			outputStack.push_back(std::move(parse_range(rgx)));
			break;

		case TEXT('('):
		{
			// KTTODO - comment:
			regex tmp = parse_regex(++rgx);
			size_t oldPosition = outputStack.size();
			outputStack.insert(outputStack.end(), tmp.begin(), tmp.end());
			size_t newPosition = outputStack.size() - 1;
			outputStack[newPosition].iteration_begin = oldPosition;
			break;
		}

		case TEXT(')'):
		{
			++rgx;
			CheckEnd(rgx);
			
			regex_item i;
			switch (*rgx)
			{
			case TEXT('*'):
				i.type = regex_item::type_t::ITERATION;
				break;

			case TEXT('+'):
				i.type = regex_item::type_t::POSITIVE_ITERATION;
				break;
			
			case TEXT('{'):
				i.type = regex_item::type_t::NUMERIC_ITERATION;

				++rgx;
				CheckEnd(rgx);
				i.iteration_count = (*rgx - TEXT('0')); // KTTODO - more numbers

				++rgx;
				CheckIs(rgx, TEXT('}'));
				break;

			default:
				ThrowParsingException(rgx);
			}
			++rgx;

			outputStack.push_back(std::move(i));

			return outputStack;
		}

		default:
		{
			regex_item i;
			i.type = regex_item::type_t::NORMAL;
			i.char1 = *rgx;
			outputStack.push_back(std::move(i));

			++rgx;
			break;
		}

		}
	}

	return outputStack;
}


base::char_t builder::parse_character(const base::char_t*& rest, builder::regex& out) const
{
	while (*rest == TEXT('\\'))
	{
		++rest;
		CheckEnd(rest);

		// Special character:
		switch (*rest)
		{
		case TEXT('['):
		case TEXT(']'):
		case TEXT('('):
		case TEXT(')'):
		case TEXT('{'):
		case TEXT('}'):
		case TEXT('*'):
		{
			regex_item i;
			i.type = regex_item::type_t::NORMAL;
			i.char1 = *rest;
			out.push_back(std::move(i));
			++rest;
		}
		break;

		default:
			// Wrong special character:
			ThrowParsingException(rest);
		}
	}

	return *rest;
}


builder::regex_item builder::parse_range(const base::char_t*& rest) const
{
	builder::regex_item item;
	item.type = regex_item::type_t::RANGE;

	// [a-b]
	ASSERT_NO_EVAL(*rest == TEXT('['));

	++rest;
	// a-b]
	CheckEnd(rest);
	item.char1 = *rest;

	++rest;
	// -b]
	CheckIs(rest, TEXT('-'));

	++rest;
	// b]
	CheckEnd(rest);
	item.char2 = *rest;

	++rest;
	// ]
	CheckIs(rest, TEXT(']'));

	// 
	++rest;

	return item;
}


}
}
