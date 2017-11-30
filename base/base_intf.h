#pragma once


#include <cstdint>


// Interfaces:
#include <base_intf/FiniteStateMachine/fsm_intf.h>
#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>


// KTTODO - move else.
using error_t = int32_t;


namespace base
{


struct base_intf
{
	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm) const noexcept = 0;
	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm, std::shared_ptr<fsm_context_factory_intf> spFactory) const noexcept = 0;
};


}
