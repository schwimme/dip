#include "intf_impl.h"

#include <sys/types/string.h>
#include <error/exceptions.h>

#include <list>


namespace checklib
{

// KTTODO - path param is ignored and behavior is as set to true
error_t intf_impl::configure(cross::string_ref la_config, cross::string_ref sa_config, bool path)
{
	try
	{
		m_impl.configure(sys::string(la_config.m_data, la_config.m_size), sys::string(sa_config.m_data, sa_config.m_size));
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

