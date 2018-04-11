#include "base.h"

#include <error/exceptions.h>

// Implementations:
#include <base/FiniteStateMachine/fsm.h>
#include <base/PushDownAutomata/pda_intf_impl.h>


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


error_t base_impl::create_pda(cross::versioned_intf_ref<base::pda_intf> pPda) noexcept
{
	try
	{
		std::unique_ptr<base::pda::intf_impl> up = std::make_unique<base::pda::intf_impl>();
		if (pPda.attach(up.get()))
		{
			// Successfully attached, do not delete object:
			up.release();
			return E_NO_ERROR;
		}

		return E_NOT_IMPLEMENTED_ERROR;
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
