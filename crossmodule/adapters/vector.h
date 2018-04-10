#pragma once


#include <crossmodule/types/enumerator.h>
#include <crossmodule/types/backinserter.h>

#include <error/base_error_code.h>

#include <vector>


namespace cross
{


template<typename T>
struct std_vector_on_enumerator:
	enumerator<T>
{
	std_vector_on_enumerator(const std::vector<T>& vector):
		m_vector(vector),
		m_pos(vector.begin())
	{}

	const T* const get() const override
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
	back_inserter<T>
{
	virtual error_t push_back(const T& o) override
	{
		try
		{
			m_vector.push_back(o);
		}
		catch (...)
		{
			return E_COMMON;
		}

		return E_NO_ERROR;
	}

	virtual error_t push_back(T&& o) override
	{
		try
		{
			m_vector.push_back(std::move(o));
		}
		catch (...)
		{
			return E_COMMON;
		}

		return E_NO_ERROR;
	}

	virtual error_t reserve(size_t count) override
	{
		try
		{
			m_vector.reserve(count);
		}
		catch (...)
		{
			return E_COMMON;
		}

		return E_NO_ERROR;
	}

private:
	std::vector<T>& m_vector;
};


template<typename T>
std::vector<T> enumerator_to_vector(const enumerator<T>& e)
{
	std::vector<T> ret;
	enumerator<T>& localE = const_cast<enumerator<T>&>(e);

	const T* ptr = nullptr;
	while ((ptr = localE.get()) != nullptr)
	{
		ret.push_back(*ptr);
		localE.next();
	}

	return ret;
}

}
