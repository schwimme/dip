#pragma once


#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>


namespace base
{
namespace fsm
{
	

/*!
	\brief		Default context factory used if no other factory is given.
	\detail		Factory behaves as standard fsm so if at least one VALID
				context given, result is VALID. Otherwise INVALID.
*/
class base_context_factory:
	public fsm_context_factory_intf
{
public:
	virtual fsm::context_id select_context(cross::enumerator<fsm::context_id>* const allContexts) const override
	{
		fsm::context_id const* pCtx = nullptr;
		while ((pCtx = allContexts->get()) != nullptr)
		{
			if (*pCtx == VALID)
			{
				return VALID;
			}

			allContexts->next();
		}

		return INVALID;
	}
};


}
}