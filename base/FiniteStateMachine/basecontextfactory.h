#pragma once


#include "fsmcontextfactory_intf.h"

#include <base/Debugging/debug.h>

namespace Base
{
namespace Fsm 
{
	

class CBaseContextFactory:
	public IFsmContextFactory
{
public:
	virtual Fsm::ContextId SelectContext(const std::vector<Fsm::ContextId>& allContexts) const override
	{
		VERIFY(allContexts.size() == 1);
		return allContexts[0];
	}
};


}
}