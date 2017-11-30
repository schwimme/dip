#include "objectfactory.h"


// Exported interfaces:
#include <base/base_intf.h>

#ifndef DYNAMIC_LINK_BASE
#	include <base/base.h>
#endif


namespace crossmodule
{


error_t object_factory::get_object(const guid_t& id, void ** ppObject) const
{
	if (id == GUID_BASE_V1)
	{
#ifdef DYNAMIC_LINK_BASE
		// KTTODO
#else
		base::base_intf* pIntf = new (std::nothrow) base::base_impl();
		if (pIntf == nullptr)
		{
			return 2; // KTTODO - no memory
		}

		*ppObject = (void*)pIntf;
		return 0; // KTTODO - no error
#endif
	}

	return 1; // KTTODO - unknown guid
}


}