#pragma once


#include "fsmcontextfactory_intf.h"
#include <base/Algorithm/algorithm.h>


namespace Base
{
namespace Fsm
{


struct CRegexContextFactory:
	public IFsmContextFactory
{

	CRegexContextFactory(ContextId valid, ContextId invalid):
		m_valid(valid),
		m_invalid(invalid)
	{}


	virtual Fsm::ContextId SelectContext(const std::vector<Fsm::ContextId>& allContexts) const override
	{
		if (Base::Find(allContexts, m_valid))
		{
			return m_valid;
		}

		return m_invalid;
	}


private:
	ContextId m_valid;
	ContextId m_invalid;
};


}
}
