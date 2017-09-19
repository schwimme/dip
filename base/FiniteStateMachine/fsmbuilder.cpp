#include "fsmbuilder.h"
#include "fsm.h"
#include "regexcontextfactory.h"


namespace Base
{
namespace Fsm
{


void CBuilder::MergeRegex(CFsm& fsm, const Base::String& regex, ContextId valid, ContextId invalid) const
{
	// Parse regex to internal structure:
	Regex parsedRegex = ParseRegex(regex);

	// Prepare output fsm:
	std::shared_ptr<IFsmContextFactory> spCtxF = std::make_shared<CRegexContextFactory>(valid, invalid);
	CFsm regexFsm(spCtxF);
	regexFsm.Initialize();

	StateId fsmStart = regexFsm.GenerateState(invalid);
	regexFsm.SetStart(fsmStart);

	StateId fsmEnd = regexFsm.GenerateState(valid);

	// Prepare mapping from parsed regex to fsm states:
	std::vector<CFsm::StatesStorage::value_type*> newStates;

	// Generate:
	size_t regexSize = parsedRegex.size();
	for (size_t it = 0; it < regexSize; ++it)
	{
		switch (parsedRegex[it].type)
		{
		case RegexItem::Type::NORMAL:
		{
			// Generate state:
			StateId s = regexFsm.GenerateState(invalid);

			// Is there previous state?
			const StateId& b = (it == 0) ? fsmStart : newStates.at(newStates.size() - 1)->first;

			// Store reference to new states:
			newStates.push_back(&(*(regexFsm.m_states.find(s))));

			// Add rule from starting position to newly generated state via character
			regexFsm.AddRule(b, s, parsedRegex[it].char1);
		}
		break;

		case RegexItem::Type::RANGE:
		{
			// Generate state:
			StateId s = regexFsm.GenerateState(invalid);

			// Is there previous state?
			const StateId& b = (it == 0) ? fsmStart : newStates.at(newStates.size() - 1)->first;

			// Store reference to new states:
			newStates.push_back(&(*(regexFsm.m_states.find(s))));

			// Add rule from starting position to newly generated state via character
			regexFsm.AddRule(b, s, parsedRegex[it].char1, parsedRegex[it].char2);
		}
		break;

		case RegexItem::Type::ITERATION:
		{
			// Cannot iterate nothing:
			VERIFY_NO_EVAL(it != 0);

			// Insert last->epsilon->empty to not harm processing:
			StateId s = regexFsm.GenerateState(invalid);
			regexFsm.AddRule(newStates.at(newStates.size() - 1)->first, s);
			newStates.push_back(&(*(regexFsm.m_states.find(s))));

			// Register loop:
			regexFsm.AddRule(s, newStates[parsedRegex[it].iteration_begin]->first);

			// Register epsilon rule from begin iteration to here to allow skip all the loop:
			regexFsm.AddRule(newStates[parsedRegex[it].iteration_begin]->first, s);
		}
		break;

		case RegexItem::Type::POSITIVE_ITERATION:
		{
			// Cannot iterate nothing:
			VERIFY_NO_EVAL(it != 0);

			// Insert last->epsilon->empty to not harm processing:
			StateId s = regexFsm.GenerateState(invalid);
			regexFsm.AddRule(newStates.at(newStates.size() - 1)->first, s);
			newStates.push_back(&(*(regexFsm.m_states.find(s))));

			// Register loop:
			regexFsm.AddRule(s, newStates[parsedRegex[it].iteration_begin]->first);
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
	regexFsm.AddRule(newStates.at(newStates.size() - 1)->first, fsmEnd);

	// This fsm can be determined because all generated states are just internal:
	regexFsm.Optimize(CFsm::OptimizationLevel::DETERMINE);

	// KTTODO optimize - remove not endable states:

	// Merge fsm:
	// - Copy new states:
	for (auto& it : regexFsm.m_states)
	{
		fsm.m_states.insert(std::move(it));
	}

	// - Merge:
	fsm.AddRule(fsm.m_start, fsmStart);
}


CBuilder::Regex CBuilder::ParseRegex(const Base::String& regex) const
{
	return CBuilder::Regex();
}


}
}
