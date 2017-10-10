#include "fsmbuilder.h"
#include "fsm.h"
#include "regexcontextfactory.h"

#include <base/Debugging/debug.h>

namespace Base
{
namespace Fsm
{


void CBuilder::BuildFsmFromRegex(CFsm& fsm, const Base::String& regex, ContextId valid, ContextId invalid) const
{
	// Parse regex to internal structure:
	const CharType* t = regex.c_str();
	Regex parsedRegex = ParseRegex(t);

	// Prepare output fsm:
	StateId fsmStart = fsm.GenerateState(invalid);
	fsm.SetStart(fsmStart);

	StateId fsmEnd = fsm.GenerateState(valid);

	// Prepare mapping from parsed regex to fsm states:
	std::vector<CFsm::StatesStorage::iterator> newStates = { fsm.m_states.find(fsmStart) };

	// Generate:
	size_t regexSize = parsedRegex.size();
	for (size_t it = 0; it < regexSize; ++it)
	{
		const auto& currentParsedRegex = parsedRegex[it];
		switch (currentParsedRegex.type)
		{
		case RegexItem::Type::NORMAL:
		{
			// Generate state:
			StateId s = fsm.GenerateState(invalid);

			// Current end:
			const StateId& b = newStates.at(newStates.size() - 1)->first;

			// Store reference to new states:
			newStates.push_back(fsm.m_states.find(s));

			// Add rule from starting position to newly generated state via character
			fsm.AddRule(b, s, currentParsedRegex.char1);
		}
		break;

		case RegexItem::Type::RANGE:
		{
			// Generate state:
			StateId s = fsm.GenerateState(invalid);

			// Current end:
			const StateId& b = newStates.at(newStates.size() - 1)->first;

			// Store reference to new states:
			newStates.push_back(fsm.m_states.find(s));

			// Add rule from starting position to newly generated state via character
			fsm.AddRule(b, s, currentParsedRegex.char1, currentParsedRegex.char2);
		}
		break;

		case RegexItem::Type::ITERATION:
		{
			// Cannot iterate nothing:
			ASSERT_NO_EVAL(it != 0);

			// Insert last->epsilon->empty to not harm processing:
			StateId s = fsm.GenerateState(invalid);
			fsm.AddRule(newStates.at(newStates.size() - 1)->first, s);
			newStates.push_back(fsm.m_states.find(s));

			// Register loop:
			fsm.AddRule(s, newStates[currentParsedRegex.iteration_begin]->first);

			// Register epsilon rule from begin iteration to here to allow skip all the loop:
			fsm.AddRule(newStates[currentParsedRegex.iteration_begin]->first, s);
		}
		break;

		case RegexItem::Type::POSITIVE_ITERATION:
		{
			// Cannot iterate nothing:
			ASSERT_NO_EVAL(it != 0);

			// Insert last->epsilon->empty to not harm processing:
			StateId s = fsm.GenerateState(invalid);
			fsm.AddRule(newStates.at(newStates.size() - 1)->first, s);
			newStates.push_back(fsm.m_states.find(s));

			// Register loop:
			fsm.AddRule(s, newStates[currentParsedRegex.iteration_begin]->first);
		}
		break;

		case RegexItem::Type::NUMERIC_ITERATION:
		{

		}
		break;

		default:
			break;
		}
	}

	// Add end rule:
	fsm.AddRule(newStates.at(newStates.size() - 1)->first, fsmEnd);

	// This fsm can be fully optimized because all generated states are just internal:
	fsm.Optimize(CFsm::OptimizationLevel::MINIMIZE);
}


CBuilder::Regex CBuilder::ParseRegex(const Base::CharType*& regex) const
{
	CBuilder::Regex outputStack;

	while (*regex)
	{
		Base::CharType c = ParseCharacter(regex, outputStack);

		switch (c)
		{
		case TEXT('['):
			outputStack.push_back(std::move(ParseRange(regex)));
			break;

		case TEXT('('):
		{
			// KTTODO - comment:
			CBuilder::Regex tmp = ParseRegex(++regex);
			size_t oldPosition = outputStack.size();
			outputStack.insert(outputStack.end(), tmp.begin(), tmp.end());
			size_t newPosition = outputStack.size() - 1;
			outputStack[newPosition].iteration_begin = oldPosition;
			break;
		}

		case TEXT(')'):
		{
			++regex;
			CheckEnd(regex);
			
			RegexItem i;
			switch (*regex)
			{
			case TEXT('*'):
				i.type = RegexItem::Type::ITERATION;
				break;

			case TEXT('+'):
				i.type = RegexItem::Type::POSITIVE_ITERATION;
				break;
			
			case TEXT('{'):
				i.type = RegexItem::Type::NUMERIC_ITERATION;

				++regex;
				CheckEnd(regex);
				i.iteration_count = (*regex - TEXT('0')); // KTTODO - more numbers

				++regex;
				CheckIs(regex, TEXT('}'));
				break;

			default:
				ThrowParsingException(regex);
			}
			++regex;

			outputStack.push_back(std::move(i));

			return outputStack;
		}

		default:
		{
			RegexItem i;
			i.type = RegexItem::Type::NORMAL;
			i.char1 = *regex;
			outputStack.push_back(std::move(i));

			++regex;
			break;
		}

		}
	}

	return outputStack;
}


Base::CharType CBuilder::ParseCharacter(const Base::CharType*& rest, CBuilder::Regex& out) const
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
			RegexItem i;
			i.type = RegexItem::Type::NORMAL;
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


CBuilder::RegexItem CBuilder::ParseRange(const Base::CharType*& rest) const
{
	CBuilder::RegexItem item;
	item.type = RegexItem::Type::RANGE;

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
