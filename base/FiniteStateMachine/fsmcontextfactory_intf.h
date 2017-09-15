#pragma once


#include "defs.h"
#include <vector>


namespace Base
{
	

struct IFsmContextFactory
{
	virtual Fsm::ContextId SelectContext(const std::vector<Fsm::ContextId>& allContexts) const = 0;
};


}
