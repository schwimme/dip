#pragma once

#include <base_intf/FiniteStateMachine/fsmdefs.h>
#include <vector>


namespace base
{
namespace fsm 
{
	

struct walkable_intf
{
	virtual const std::vector<fsm::state_id>& get_next_states(const fsm::state_id& currentState, sys::char_t ch) const = 0;
	virtual fsm::context_id get_context(const fsm::state_id& state) const = 0;
	virtual const fsm::state_id& get_start() const = 0;

protected:
	walkable_intf() {}
};


}
}