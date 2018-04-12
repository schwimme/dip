#pragma once

#include <crossmodule/types/pointer.h>
#include <crossmodule/types/query.h>

#include "checker_errors.h"

// Interfaces:
#include <checker_intf/checker_intf.h>


namespace checklib
{

struct checklib_intf:
	cross::queryable<0x14a7d539>
{
	static constexpr uint32_t version = 1;

	virtual error_t create_checker(cross::versioned_intf_ref<checklib::checker_intf> pChecker) noexcept = 0;
};

}
