#pragma once


#include <base_intf/FiniteStateMachine/fsmdefs.h>
#include <unordered_map>
#include <vector>
#include <memory>


namespace base
{
namespace fsm
{


//! \brief	One fsm state.
struct state
{
	//! Holder used in fsm for simple memory managment.
	using holder = std::unique_ptr<state>;


	state(context_id ctx):
		m_context(ctx)
	{}

	//! \brief	Add simple rule this->'ch'->'to'
	void add_rule(const state_id& to, sys::char_t ch);

	/*!
		\brief	Convert this->epsilon->to->character->other
				to		this->character->other
	*/
	void add_optimized_epsilon_rule(const state& to);

	//! \brief	Remove all rules using given character.
	void remove_rules(sys::char_t ch);

	//! \brief	Get all rules using given character.
	const std::vector<state_id>& get_rules(sys::char_t ch) const;

	std::unordered_map<sys::char_t, std::vector<state_id>>	m_rules;
	context_id	m_context;
	bool		m_epsVisited = false;
};


}
}
