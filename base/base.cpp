#include "base.h"

// Implementations:
#include <base/FiniteStateMachine/fsm.h>


namespace base
{


error_t base_impl::create_fsm(std::shared_ptr<fsm_intf>& spFsm) const noexcept
{
	try
	{
		spFsm = fsm_impl::factory().create_finite_state_machine();
		return 0; // KTTODO - no error
	}
	catch (...)
	{
		return 1; // KTTODO - failed
	}
}


error_t base_impl::create_fsm(std::shared_ptr<fsm_intf>& spFsm, std::shared_ptr<fsm_context_factory_intf> spFactory) const noexcept
{
	try
	{
		spFsm = fsm_impl::factory().create_finite_state_machine(spFactory);
		return 0; // KTTODO - no error
	}
	catch (...)
	{
		return 1; // KTTODO - failed
	}
}


}
