#pragma once


#include <base_intf/base_intf.h>
#include <crossmodule/types/pointer.h>


namespace base
{

struct base_loader_intf
{
	virtual void get_base(cross::versioned_intf_ref<base::base_intf> pBase) = 0;
};


}
