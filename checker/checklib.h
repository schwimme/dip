#pragma once


#include <checker_intf/checklib_intf.h>


namespace checklib
{


struct checklib_impl:
	cross::query_impl<checklib_intf>
{
	virtual error_t create_checker(cross::versioned_intf_ref<checklib::checker_intf> pChecker) noexcept override;
};


}
