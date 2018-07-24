#pragma once


#include <base_intf/base_intf.h>


namespace base
{


struct base_impl:
	public cross::query_impl<base_intf>
{
	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm) noexcept override;
	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm, std::shared_ptr<fsm_context_factory_intf> spFactory) noexcept override;
	virtual error_t create_pda(cross::versioned_intf_ref<base::pda_intf> pPda) noexcept override;
	virtual error_t create_ll_validator(std::shared_ptr<ll_validator_intf>& pPda) noexcept override;

};


}

