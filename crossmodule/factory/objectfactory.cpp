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
		return E_NOT_IMPLEMETED_ERROR;
#elif defined BASE_LIB
		base::base_intf* pIntf = new (std::nothrow) base::base_impl();
		if (pIntf == nullptr)
		{
			return E_MEMORY_ERROR;
		}

		*ppObject = (void*)pIntf;
		return E_NO_ERROR;
#else /*BASE_NONE*/
		return E_NOT_IMPLEMENTED_ERROR;
#endif
	}

	return E_NOT_IMPLEMENTED_ERROR;
}


}