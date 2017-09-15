#pragma once


#include "defs.h"
#include <unordered_map>
#include <vector>
#include <memory>


namespace Base
{
namespace Fsm
{


//! \brief	One fsm state.
class CState
{
public:
	//! Holder used in fsm for simple memory managment.
	using Holder = std::unique_ptr<CState>;

public:
	CState(ContextId ctx):
		m_context(ctx)
	{}

public:
	//! \brief	Add simple rule this->'ch'->'to'
	void AddRule(const StateId& to, AlphabetType ch);

	/*!
		\brief	Convert this->epsilon->to->character->other
				to		this->character->other
	*/
	void AddOptimizedEpsilonRule(const CState& to);

	//! \brief	Remove all rules using given character.
	void RemoveRules(AlphabetType ch);

	//! \brief	Get all rules using given character.
	const std::vector<StateId>& GetRules(AlphabetType ch) const;

	//! \brief	Get all rules for given state.
	const std::unordered_map<AlphabetType, std::vector<StateId>> GetAllRules();

	//! \brief	Get state context.
	ContextId GetContext();

private:
	std::unordered_map<AlphabetType, std::vector<StateId>>	m_rules;

	ContextId	m_context;
};

}
}