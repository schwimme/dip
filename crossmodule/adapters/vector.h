#pragma once


#include <crossmodule/types/enumerator.h>
#include <crossmodule/types/backinserter.h>

#include <vector>


namespace crossmodule
{

template<typename T>
struct std_vector_on_enumerator:
	enumerator<T>
{
	std_vector_on_enumerator(const std::vector<T>& vector):
		m_vector(vector),
		m_pos(vector.begin())
	{}

	const T* const get() override
	{
		if (m_pos != m_vector.end())
		{
			return &*m_pos;
		}
		else
		{
			return nullptr;
		}
	}

	void next() override
	{
		m_pos++;
	}

	void reset() override
	{
		m_pos = m_vector.begin();
	}

private:
	const std::vector<T>&					m_vector;
	typename std::vector<T>::const_iterator	m_pos;
};

template<typename T>
struct std_vector_on_back_inserter:
	crossmodule::back_inserter<T>
{
	virtual error_t push_back(const T& o) override
	{
		try
		{
			m_vector.push_back(o);
		}
		catch (...)
		{
			return 1; // KTTODO - error
		}

		return 0; // KTTODO - no error
	}

	virtual error_t push_back(T&& o) override
	{
		try
		{
			m_vector.push_back(std::move(o));
		}
		catch (...)
		{
			return 1; // KTTODO - error
		}

		return 0; // KTTODO - no error
	}

	virtual error_t reserve(size_t count) override
	{
		try
		{
			m_vector.reserve(count);
		}
		catch (...)
		{
			return 1; // KTTODO - error
		}

		return 0; // KTTODO - no error
	}

private:
	std::vector<T>& m_vector;
};


}
