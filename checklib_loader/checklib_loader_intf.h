#pragma once


#include <checker_intf/checklib_intf.h>
#include <crossmodule/types/pointer.h>


namespace checklib
{

struct checklib_loader_intf
{
	virtual void get_checklib(cross::versioned_intf_ref<checklib::checklib_intf> pBase) = 0;
};


}
