#include "checklib_loader.h"

#include <error/exceptions.h>

#ifdef CHECKLIB_LIB
#	include <checker/checklib.h>
#endif


namespace checklib
{


void checklib_loader::get_checklib(cross::versioned_intf_ref<checklib::checklib_intf> pChecklib)
{
#ifdef CHECKLIB_LIB

	std::unique_ptr<checklib_intf> pIntf = std::make_unique<checklib_impl>();
	if (pChecklib.attach(pIntf.get()) == false)
	{
		// Error - let object die and throw exception:
		throw exception_t(E_INTF_NOT_FOUND_ERROR);
	}

	// Success:
	pIntf.release();

#elif CHECKLIB_DLL

	throw exception_t(E_NOT_IMPLEMENTED_ERROR);

#else

#	error "Not specified linkage"

#endif
}


}
