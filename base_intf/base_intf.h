#pragma once

#include <crossmodule/types/pointer.h>
#include <crossmodule/types/query.h>

#include <error/error_code_base.h>


// Interfaces:
#include <base_intf/FiniteStateMachine/fsm_intf.h>
#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>
#include <base_intf/PushDownAutomata/pda_intf.h>
#include "ll_validator/ll_validator_intf.h"


namespace base
{


struct base_intf:
	cross::queryable<0x3296ce53>
{
	static constexpr uint32_t version = 1;

	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm) noexcept = 0;
	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm, std::shared_ptr<fsm_context_factory_intf> spFactory) noexcept = 0;
	virtual error_t create_pda(cross::versioned_intf_ref<base::pda_intf> pPda) noexcept = 0;
	virtual error_t create_ll_validator(std::shared_ptr<ll_validator_intf>& pPda) noexcept = 0;
};


}
