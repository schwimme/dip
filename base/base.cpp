#include "base.h"

#include <error/exceptions.h>

// Implementations:
#include <base/FiniteStateMachine/fsm.h>
#include "ll_validator/ll_validator_impl.h"


namespace base
{


error_t base_impl::create_fsm(std::shared_ptr<fsm_intf>& spFsm) noexcept
{
	try
	{
		spFsm = fsm_impl::factory().create_finite_state_machine();
		return E_NO_ERROR;
	}
	catch (const exception_t& e)
	{
		return e.m_code;
	}
	catch (...)
	{
		return E_COMMON_ERROR;
	}
}


error_t base_impl::create_fsm(std::shared_ptr<fsm_intf>& spFsm, std::shared_ptr<fsm_context_factory_intf> spFactory) noexcept
{
	try
	{
		spFsm = fsm_impl::factory().create_finite_state_machine(spFactory);
		return E_NO_ERROR;
	}
	catch (const exception_t& e)
	{
		return e.m_code;
	}
	catch (...)
	{
		return E_COMMON_ERROR;
	}
}


error_t base_impl::create_ll_validator(std::shared_ptr<ll_validator_intf>& spFsm) noexcept
{
	try
	{
		spFsm = std::make_shared<ll_validator_intf_impl>();
		return E_NO_ERROR;
	}
	catch (const exception_t& e)
	{
		return e.m_code;
	}
	catch (...)
	{
		return E_COMMON_ERROR;
	}
}


}
