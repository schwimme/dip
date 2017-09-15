#pragma once


#include "fsmcontextfactory_intf.h"
#include <base/Debugging/debug.h>


namespace Base
{
namespace Fsm 
{
	

/*!
	\brief		Default context factory used if no other factory is given.
	\warning	Factory is not dealing with context priorities. For more
				contexts asserts and returns first one.
*/
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