#pragma once


#include "defs.h"
#include <unordered_map>
#include <vector>
#include <memory>


namespace Base
{
namespace Fsm
{


class CState
{
public:
	using Holder = std::unique_ptr<CState>;

public:
	CState(ContextId ctx):
		m_context(ctx)
	{}

public:
	void AddRule(const StateId& to, AlphabetType ch);
	void AddOptimizedEpsilonRule(const CState& to);
	void RemoveRules(AlphabetType ch);
	const std::vector<StateId>& GetRules(AlphabetType ch) const;
	const std::unordered_map<AlphabetType, std::vector<StateId>> GetAllRules();
	ContextId GetContext();

private:
	std::vector<StateId> m_emptyVector;
	std::unordered_map<AlphabetType, std::vector<StateId>>	m_rules;
	ContextId	m_context;
};

}
}