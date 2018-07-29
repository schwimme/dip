#include "intf_impl.h"
#include <error/exceptions.h>

#include <crossmodule/adapters/vector.h>


namespace ccc
{

error_t checking_module_intf_impl::configure(cross::string_ref config_path, checklib::incident_handler_intf* pHandler)
{
	try
	{
		if (pHandler == nullptr)
		{
			return E_CCC_UNSPECIFIED_HANDLER_WARN;
		}

		m_impl.configure(sys::string(config_path.m_data, config_path.m_size), *pHandler);
		
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


error_t checking_module_intf_impl::check_files(cross::enumerator<cross::string_ref>* files)
{
	try
	{
		std::vector<sys::string> v;
		const cross::string_ref* p_str = nullptr;
		while ((p_str = files->get()) != nullptr)
		{
			v.push_back(std::move(sys::string(p_str->m_data, p_str->m_size)));
			files->next();
		}

		m_impl.check_files(v);
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