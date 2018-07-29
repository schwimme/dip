#include "fsmwalker.h"
#include <sys/algorithm/algorithm.h>
#include <crossmodule/adapters/vector.h>
#include <sys/types/string.h>


namespace base
{
namespace fsm
{


walker::walker(fsm::walkable_intf& walkable, std::shared_ptr<fsm_context_factory_intf>& spCtxFactory):
	m_spCtxFactory(spCtxFactory),
	m_pWalkable(&walkable)
{
	reset();
}


bool walker::process_step(sys::char_t ch)
{
	std::vector<fsm::state_id> newState;
	for (const auto& s : m_actualState)
	{
		std::vector<fsm::state_id> tmp = m_pWalkable->get_next_states(s, ch);
		newState = sys::make_union(newState, tmp);
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


bool walker::verify_literal(cross::string_ref literal)
{
	sys::string literal_no_crossmodule = sys::string(literal.m_data, literal.m_size);

	for (sys::char_t c : literal_no_crossmodule)
	{
		if (process_step(c) == false)
		{
			// Literal was not readed:
			return false;
		}
	}

	return true;
}


void walker::reset()
{
	// Actual state is fsm start.
	m_actualState = { m_pWalkable->get_start() };
}


fsm::context_id walker::get_context() const
{
	// Get context of all current states:
	std::vector<fsm::context_id> ctxs;
	for (const auto& s : m_actualState)
	{
		fsm::context_id c = m_pWalkable->get_context(s);
		if (sys::find(ctxs, c) == false)
		{
			ctxs.push_back(c);
		}
	}

	// Return most priorited context by factory:
	return m_spCtxFactory->select_context(&cross::std_vector_on_enumerator<fsm::context_id>(ctxs));
}


}
}
