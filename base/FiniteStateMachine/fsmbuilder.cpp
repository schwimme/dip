#include "fsmbuilder.h"
#include "fsm.h"


namespace Base
{
namespace Fsm
{


void CBuilder::MergeRegex(CFsm& fsm, const Base::String& regex, ContextId valid, ContextId invalid) const
{
	// Parse regex to internal structure
	Regex parsedRegex = ParseRegex(regex);

	size_t regexSize = parsedRegex.size();

	CFsm::StatesStorage	newStates;
	for (size_t it = 0; it < regexSize; ++it)
	{
		switch (parsedRegex[it].type)
		{
		case RegexItem::Type::NORMAL:
		case RegexItem::Type::RANGE:
		case RegexItem::Type::ITERATION:
		case RegexItem::Type::POSITIVE_ITERATION:
		case RegexItem::Type::NUMERIC_ITERATION:
		default:
		}
	}
	
}


CBuilder::Regex CBuilder::ParseRegex(const Base::String& regex) const
{

}


}
}
