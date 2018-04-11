#include "checklib.h"

#include <error/exceptions.h>

// Implementations:
#include <checker/intf_impl.h>


namespace checklib
{

error_t checklib_impl::create_checker(cross::versioned_intf_ref<checklib::checker_intf> pChecker) noexcept
{
	try
	{
		std::unique_ptr<checklib::intf_impl> up = std::make_unique<checklib::intf_impl>();
		if (pChecker.attach(up.get()))
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
