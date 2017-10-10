#pragma once


#include <memory>

// All exported interfaces:
#include <base_intf/FiniteStateMachine/fsm_intf.h>
#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>


namespace Base
{


struct BaseFactory
{
	virtual std::shared_ptr<IFsm> CreateFsm(std::shared_ptr<IFsmContextFactory>& spFsmFactory) const;
	virtual std::shared_ptr<IFsm> CreateFsm() const;
};


}