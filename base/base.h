#pragma once


#include "base_intf.h"


namespace base
{


struct base_impl:
	public base_intf
{
	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm) const noexcept override;
	virtual error_t create_fsm(std::shared_ptr<fsm_intf>& spFsm, std::shared_ptr<fsm_context_factory_intf> spFactory) const noexcept override;
};


}

