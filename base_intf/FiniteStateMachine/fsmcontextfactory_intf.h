#pragma once


#include "fsmdefs.h"
#include <crossmodule/types/enumerator.h>


namespace base
{
	
//! \brief	Interface for injected ctx factory to fsm.
struct fsm_context_factory_intf
{
	//!	\brief	Select most priorited context from all given contexts.
	virtual fsm::context_id select_context(cross::enumerator<fsm::context_id>* const allContexts) const = 0;
};


}
