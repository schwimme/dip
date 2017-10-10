#include "BaseFactory.h"

#include <base/FiniteStateMachine/fsm.h>

namespace Base
{


std::shared_ptr<IFsm> BaseFactory::CreateFsm(std::shared_ptr<IFsmContextFactory>& spFsmFactory) const
{
	return CFsm::Factory().CreateFiniteStateMachine(spFsmFactory);
}

std::shared_ptr<IFsm> BaseFactory::CreateFsm() const
{
	return CFsm::Factory().CreateFiniteStateMachine();
}


}
