#pragma once

#include <cstdint>


namespace crossmodule
{
namespace detail {

struct queryable_tag {};

}

template <uint64_t _iid>
struct queryable
{
	using queryable_t = detail::queryable_tag;

	static constexpr uint64_t iid = _iid;

	virtual void * query(uint64_t iid, uint32_t min_version, uint32_t & real_version) = 0;
};

template <typename... Tn>
struct query_impl;

template <typename T, typename U>
T * iid_cast(U * u) noexcept;

template <typename T, typename U>
T & iid_cast(U & u);

namespace detail {

template <typename Intf, typename = void>
struct is_queryable:
	std::false_type
{};

template <typename Intf>
struct is_queryable<Intf, std::enable_if_t<std::is_same<typename Intf::queryable_t, detail::queryable_tag>::value>>:
	std::true_type
{};

}

template <typename Intf >
constexpr bool is_queryable_v = detail::is_queryable<Intf>::value;


template <>
struct query_impl<>
{
	void * query(uint64_t /*iid*/, uint32_t /*min_version*/, uint32_t & /*real_version*/)
	{
		return nullptr;
	}
};

template <typename T0, typename... Tn>
struct query_impl<T0, Tn...>
	: T0, query_impl<Tn...>
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

template <typename T, typename U>
T * iid_cast(U * u) noexcept
{
	uint32_t real_version;
	return static_cast<T *>(u->query(T::iid, T::version, real_version));
}

template <typename T, typename U>
T & iid_cast(U & u)
{
	if (T * p = iid_cast<T>(&u))
		return p;

	throw std::bad_cast();
}

}
