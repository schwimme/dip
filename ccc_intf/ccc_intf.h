#pragma once

#include <crossmodule/types/pointer.h>
#include <crossmodule/types/query.h>

#include "ccc_errors.h"

// Interfaces:
#include "ccc_checking_module_intf.h"


namespace ccc
{

struct ccc_intf:
	cross::queryable<0x7b62d191>
{
	static constexpr uint32_t version = 1;

	virtual error_t get_checking_module(cross::versioned_intf_ref<checking_module_intf> pModule) = 0;
};

}
