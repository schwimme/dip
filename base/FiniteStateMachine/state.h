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
struct CState
{
	//! Holder used in fsm for simple memory managment.
	using Holder = std::unique_ptr<CState>;


	CState(ContextId ctx):
		m_context(ctx)
	{}

	//! \brief	Add simple rule this->'ch'->'to'
	void AddRule(const StateId& to, Base::CharType ch);

	/*!
		\brief	Convert this->epsilon->to->character->other
				to		this->character->other
	*/
	void AddOptimizedEpsilonRule(const CState& to);

	//! \brief	Remove all rules using given character.
	void RemoveRules(Base::CharType ch);

	//! \brief	Get all rules using given character.
	const std::vector<StateId>& GetRules(Base::CharType ch) const;

	std::unordered_map<Base::CharType, std::vector<StateId>>	m_rules;
	ContextId	m_context;
	bool		m_epsVisited = false;
};


}
}
