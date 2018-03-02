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


}
