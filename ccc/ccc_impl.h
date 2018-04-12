#pragma once


#include <ccc_intf/ccc_intf.h>


namespace ccc
{

class ccc_impl:
	public ccc_intf
{
	virtual error_t get_checking_module(cross::versioned_intf_ref<checking_module_intf> pModule) override;
};

}
