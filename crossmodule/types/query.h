#pragma once

#include <cstdint>


namespace crossmodule
{

template <uint64_t _iid>
struct queryable
{
	static constexpr uint64_t iid = _iid;

	virtual void * query(uint64_t iid, uint32_t min_version, uint32_t & real_version) = 0;
};


template <typename... Tn>
struct query_impl;


template <>
struct query_impl<>
{
	void * query(uint64_t /*iid*/, uint32_t /*min_version*/, uint32_t & /*real_version*/)
	{
		return nullptr;
	}
};


template <typename T0, typename... Tn>
struct query_impl<T0, Tn...>:
	T0, 
	query_impl<Tn...>
{
	void * query(uint64_t iid_, uint32_t min_version, uint32_t & real_version) override
	{
		if (T0::iid == iid_ && T0::version >= min_version)
		{
			real_version = T0::version;
			return static_cast<T0 *>(this);
		}

		return query_impl<Tn...>::query(iid_, min_version, real_version);
	}
};


}
