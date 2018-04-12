#pragma once


#include <sys/types/string.h>
#include <crossmodule/types/string.h>


namespace cross
{


struct sys_string_on_string_ref:
	string_ref
{
	sys_string_on_string_ref(const sys::string& s):
		string_ref(s.data(), s.size())
	{}
};


struct sys_string_settable_string_ref:
	settable_string_ref
{
	sys_string_settable_string_ref(sys::string& s):
		m_string(&s)
	{}

	virtual bool set(sys::char_t const* data, size_t size) noexcept override
	{
		*m_string = sys::string(data, size);
	}
	
private:
	sys::string* m_string;
};


sys::string to_sys_string(string_ref r)
{
	return sys::string(r.m_data, r.m_size);
}

}
