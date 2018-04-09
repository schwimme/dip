#pragma once


#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>
#include <sys/algorithm/algorithm.h>


namespace base
{
namespace fsm
{


struct regex_context_factory:
	public fsm_context_factory_intf
{

	regex_context_factory(context_id valid, context_id invalid):
		m_valid(valid),
		m_invalid(invalid)
	{}


	virtual fsm::context_id select_context(cross::enumerator<fsm::context_id>* const allContexts) const override
	{
		fsm::context_id const* id = nullptr;
		while (id = allContexts->get())
		{
			if (*id == m_valid)
			{
				return m_valid;
			}
			allContexts->next();
		}

		return m_invalid;
	}


private:
	context_id m_valid;
	context_id m_invalid;
};


}
}
