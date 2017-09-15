#include "state.h"

#include <base/Algorithm/algorithm.h>

#include <base/Debugging/debug.h>

namespace Base
{
namespace Fsm
{

	
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
		auto& v = m_rules[rule.first];
		v = Base::Union(v, rule.second);
	}
	// KTTODO - commit point - do not work with m_rules until here
}


void CState::RemoveRules(AlphabetType ch)
{
	VERIFY(m_rules.find(ch) != m_rules.end());
	m_rules.erase(ch);
}


const std::vector<StateId>& CState::GetRules(AlphabetType ch) const 
{
	auto it = m_rules.find(ch);
	if (it == m_rules.end())
	{
		return m_emptyVector;
	}

	return it->second;
}


const std::unordered_map<AlphabetType, std::vector<StateId>> CState::GetAllRules()
{
	return m_rules;
}


ContextId CState::GetContext()
{
	return m_context;
}


}
}