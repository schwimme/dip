#pragma once


#include "fsmdefs.h"
#include <vector>


namespace Base
{
	
//! \brief	Interface for injected ctx factory to fsm.
struct IFsmContextFactory
{
	//!	\brief	Select most priorited context from all given contexts.
	virtual Fsm::ContextId SelectContext(const std::vector<Fsm::ContextId>& allContexts) const = 0;
};


}
