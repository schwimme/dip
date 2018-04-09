#pragma once


#include <cstdint>

#include <crossmodule/types/pointer.h>


// Interfaces:
#include <base_intf/FiniteStateMachine/fsm_intf.h>
#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>
#include <base_intf/PushDownAutomata/pda_intf.h>


// KTTODO - move else.
using error_t = int32_t;


namespace base
{


struct base_intf
{
	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm) noexcept = 0;
	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm, std::shared_ptr<fsm_context_factory_intf> spFactory) noexcept = 0;
	virtual error_t create_pda(cross::versioned_intf_ref<base::pda_intf> pPda) noexcept = 0;
};


}
