#pragma once


#include "fsmcontextfactory_intf.h"
#include <base/Algorithm/algorithm.h>
#include <base/Debugging/debug.h>


namespace Base
{
namespace Fsm 
{
	

/*!
	\brief		Default context factory used if no other factory is given.
	\detail		Factory behaves as standard fsm so if at least one VALID
				context given, result is VALID. Otherwise INVALID.
*/
class CBaseContextFactory:
	public IFsmContextFactory
{
public:
	virtual Fsm::ContextId SelectContext(const std::vector<Fsm::ContextId>& allContexts) const override
	{
		if (Base::Find(allContexts, VALID))
		{
			return VALID;
		}

		return INVALID;
	}
};


}
}