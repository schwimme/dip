#include "fsmwalker.h"
#include <base/Algorithm/algorithm.h>


namespace Base
{


CFsmWalker::CFsmWalker(Fsm::IWalkable& walkable):
	m_pWalkable(&walkable)
{
	Reset();
}


bool CFsmWalker::ProcessStep(Fsm::AlphabetType ch)
{
	// Process states by character:
	std::vector<Fsm::StateId> newState;
	for (const auto& s : m_actualState)
	{
		std::vector<Fsm::StateId> tmp = m_pWalkable->GetNextStates(s, ch);
		newState = Union(newState, tmp);
	}

	// Process states by epsilon until no change:
	size_t newStateSize = 0;
	do
	{
		std::vector<Fsm::StateId> closure;
		for (const auto& s : newState)
		{
			std::vector<Fsm::StateId> tmp = m_pWalkable->GetNextStates(s, Fsm::EPSILON);
			closure = Union(closure, tmp);
		}
		newStateSize = newState.size();
		newState = Union(newState, closure);
	} while (newStateSize != newState.size());

	if (newStateSize == 0)
	{
		return false;
	}

	// Process success step:
	m_actualState = std::move(newState);
	return true;
}


bool CFsmWalker::VerifyLiteral(const String& literal)
{
	for (CharType c : literal)
	{
		if (ProcessStep(c) == false)
		{
			return false;
		}
	}

	return IsInFiniteState();
}


void CFsmWalker::Reset()
{
	const Fsm::StateId& idle = m_pWalkable->GetIdle();
	VERIFY(idle.empty() == false);
	m_actualState = { idle };
}


bool CFsmWalker::IsInFiniteState()
{
	for (const auto& s : m_actualState)
	{
		if (m_pWalkable->IsStateFinite(s))
		{
			return true;
		}
	}

	return false;
}


}
