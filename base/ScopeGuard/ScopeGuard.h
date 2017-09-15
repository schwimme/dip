#pragma once


#include <functional>


namespace Base
{

struct CScopeGuard
{
	CScopeGuard(std::function<void()> guard):
		m_guard(guard),
		m_dismissed(false)
	{}


	~CScopeGuard()
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


	void Dismiss()
	{
		m_dismissed = true;
	}

private:
	std::function<void()>	m_guard;
	bool					m_dismissed;
};


}
