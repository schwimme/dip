#include "intf_impl.h"

#include <sys/types/string.h>
#include <error/exceptions.h>

#include <list>

#include <crossmodule/adapters/basestring.h>

namespace checklib
{

// KTTODO - path param is ignored and behavior is as set to true
error_t intf_impl::configure(incident_handler_intf* pHandler, cross::string_ref la_config, cross::string_ref sa_config)
{
	try
	{
		if (pHandler == nullptr)
		{
			return E_INVALID_PARAMETER;
		}

		m_impl.configure(*pHandler, cross::to_sys_string(la_config), cross::to_sys_string(sa_config));
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


error_t intf_impl::check(cross::enumerator<cross::string_ref>* pFiles)
{
	try
	{
		std::list<sys::string> v;

		const cross::string_ref* p_str;
		while ((p_str = pFiles->get()) != nullptr)
		{
			sys::string s(p_str->m_data, p_str->m_size);
			v.push_back(std::move(s));

			pFiles->next();
		}

		m_impl.check(v);
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

