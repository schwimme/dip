#include "ccc_impl.h"

#include <error/exceptions.h>

#include "checking_module/intf_impl.h"


namespace ccc
{

error_t ccc_impl::get_checking_module(cross::versioned_intf_ref<checking_module_intf> pModule)
{
	try
	{
		std::unique_ptr<checking_module_intf_impl> up = std::make_unique<checking_module_intf_impl>();
		if (pModule.attach(up.get()))
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
