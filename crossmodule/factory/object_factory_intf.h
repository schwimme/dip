#pragma once

#include <cstdint>
#include <error/error_code_base.h>


namespace cross
{

// KTTODO - move else and make as string.
using guid_t = uint32_t;

namespace detail
{
	static constexpr guid_t GUID_BASE = 0x82f93200;
	static constexpr guid_t GUID_CHECKER = 0x570a1600;
}


static constexpr guid_t GUID_BASE_V1 = detail::GUID_BASE + 1;
static constexpr guid_t GUID_CHECKER_V1 = detail::GUID_CHECKER + 1;


struct object_factory_intf
{
	virtual error_t get_object(const guid_t& id, void ** ppObject) = 0;
};


}
