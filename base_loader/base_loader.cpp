#include "base_loader.h"

#include <error/exceptions.h>

#ifdef BASE_LIB
#	include <base/base.h>
#endif


namespace base
{


void base_loader::get_base(cross::versioned_intf_ref<base::base_intf> pBase)
{
#ifdef BASE_LIB

	std::unique_ptr<base::base_intf> pIntf = std::make_unique<base::base_impl>();
	if (pBase.attach(pIntf.get()) == false)
	{
		// Error - let object die and throw exception:
		throw exception_t(E_INTF_NOT_FOUND_ERROR);
	}

	// Success:
	pIntf.release();

#elif BASE_DLL

	throw exception_t(E_NOT_IMPLEMENTED_ERROR);

#else

#	error "Not specified linkage"

#endif
}


}
