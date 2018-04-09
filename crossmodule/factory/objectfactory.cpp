#include "objectfactory.h"
#include "detail/build_checker.h"


// Exported interfaces:
#include <base/base_intf.h>

// Exported implementations for static linkage
#ifdef BASE_LIB
#	include <base/base.h>
#endif


namespace cross
{


error_t object_factory::get_object(const guid_t& id, void ** ppObject) const
{
	if (id == GUID_BASE_V1)
	{
#if defined BASE_DLL
		// KTTODO
#elif defined BASE_LIB
		base::base_intf* pIntf = new (std::nothrow) base::base_impl();
		if (pIntf == nullptr)
		{
			return 2; // KTTODO - no memory
		}

		*ppObject = (void*)pIntf;
		return 0; // KTTODO - no error
#else /*BASE_NONE*/
		return 3; // KTTODO - not implemented
#endif
	}

	return 1; // KTTODO - unknown guid
}


}