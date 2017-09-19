#include "state.h"

#include <base/Algorithm/algorithm.h>

#include <base/Debugging/debug.h>


namespace Base
{
namespace Fsm
{

// Empty vector for returning in case, rule does not exists.
static const std::vector<StateId> g_emptyVector;


void CState::AddRule(const StateId& to, AlphabetType ch)
{
	std::vector<StateId>& nextStates = m_rules[ch];
	if (Base::Find(nextStates, to) == false)
	{
		nextStates.push_back(to);
	}
}


void CState::AddOptimizedEpsilonRule(const CState& to)
{
	for (const auto& rule : to.m_rules)
	{
		if (rule.first != Fsm::detail::EPSILON)
		{
			auto& v = m_rules[rule.first];
			v = Base::Union(v, rule.second);
		}
	}
}


void CState::RemoveRules(AlphabetType ch)
{
	m_rules.erase(ch);
}


const std::vector<StateId>& CState::GetRules(AlphabetType ch) const 
{
	auto it = m_rules.find(ch);
	if (it == m_rules.end())
	{
		return g_emptyVector;
	}

	return it->second;
}


}
}