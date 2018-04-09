#pragma once


#include <sys/types/string.h>
#include <crossmodule/types/string.h>


namespace cross
{


struct base_string_on_string_ref:
	string_ref
{
	base_string_on_string_ref(const sys::string& s):
		string_ref(s.data(), s.size())
	{}
};


struct base_string_settable_string_ref:
	settable_string_ref
{
	base_string_settable_string_ref(sys::string& s):
		m_string(&s)
	{}

	virtual bool set(sys::char_t const* data, size_t size) noexcept override
	{
		*m_string = sys::string(data, size);
	}
	
private:
	sys::string* m_string;
};


}
