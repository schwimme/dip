#pragma once


#include "fsmdefs.h"
#include "fsmwalker_intf.h"
#include <memory>
#include <crossmodule/types/string.h>

namespace base
{


struct fsm_intf
{
	//! \brief 'from'->epsilon->'to'.
	virtual void add_rule(const fsm::state_id& from, const fsm::state_id& to) = 0;

	//! \brief 'from'->'ch'->'to'.
	virtual void add_rule(const fsm::state_id& from, const fsm::state_id& to, base::char_t ch) = 0;

	//! \brief 'from'->[a, b]->'to'.
	virtual void add_rule(const fsm::state_id& from, const fsm::state_id& to, base::char_t a, base::char_t b) = 0;

	//! \brief 'from'->epsilon->regex.
	virtual void add_regex(const fsm::state_id& from, crossmodule::string_ref regex, fsm::context_id valid, fsm::context_id invalid) = 0;

	//! \brief Mark state as starting state.
	virtual void set_start(const fsm::state_id& state) = 0;

	//! \brief Generate new state id.
	virtual fsm::state_id generate_state(fsm::context_id ctx) = 0;

	/*!
		\brief		Create walker for current fsm.
		\warning	FSM cannot changed after optimization (add_rule, SetIdle and GenerateState are forbidden).
		\note		Walker does not change fsm structure so more instances can be used at the time. 
	*/
	virtual std::shared_ptr<fsm_walker_intf> create_walker() = 0;
};


}