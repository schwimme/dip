#pragma once


#include <types/string.h>				// base::string
#include <crossmodule/types/string.h>	// string_ref, settable_string_ref


namespace crossmodule
{


struct base_string_on_string_ref:
	string_ref
{
	base_string_on_string_ref(const base::string& s):
		string_ref(s.data(), s.size())
	{}
};


struct base_string_settable_string_ref:
	settable_string_ref
{
	base_string_settable_string_ref(base::string& s):
		m_string(&s)
	{}

	virtual bool set(base::char_t const* data, size_t size) noexcept override
	{
		*m_string = base::string(data, size);
	}
	
private:
	base::string* m_string;
};


}
