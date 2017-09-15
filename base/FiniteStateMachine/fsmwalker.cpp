#include "fsmwalker.h"
#include <base/Algorithm/algorithm.h>


namespace Base
{


CFsmWalker::CFsmWalker(Fsm::IWalkable& walkable, std::shared_ptr<IFsmContextFactory>& spCtxFactory):
	m_spCtxFactory(spCtxFactory),
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

	if (newState.size() == 0)
	{
		// For given char is no next states:
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

	return true;
}


void CFsmWalker::Reset()
{
	m_actualState = { m_pWalkable->GetStart() };
}


Fsm::ContextId CFsmWalker::GetContext() const
{
	std::vector<Fsm::ContextId> ctxs;
	for (const auto& s : m_actualState)
	{
		Fsm::ContextId c = m_pWalkable->GetContext(s);
		if (Base::Find(ctxs, c) == false)
		{
			ctxs.push_back(c);
		}
	}

	return m_spCtxFactory->SelectContext(ctxs);
}


}
