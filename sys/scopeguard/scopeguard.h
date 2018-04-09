#pragma once


#include <functional>


namespace sys
{

struct scope_guard
{
	explicit scope_guard(std::function<void()> guard):
		m_guard(guard),
		m_dismissed(false)
	{}


	~scope_guard()
	{
		if (!m_dismissed)
		{
			try
			{
				m_guard();
			}
			catch (...) {} // Ignore any exception.
		}
	}


	void dismiss()
	{
		m_dismissed = true;
	}

private:
	std::function<void()>	m_guard;
	bool					m_dismissed;
};


}
