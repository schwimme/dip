#include "objectfactory.h"

// #include "detail/build_checker.h"


// Exported interfaces:
#include <base_intf/base_intf.h>
#include <checker_intf/checklib_intf.h>

// Exported implementations for static linkage
#ifdef BASE_LIB
#	include <base/base.h>
#endif

#ifdef CHECKER_LIB
#	include <checker/checklib.h>
#endif

namespace
{
	template<class _Intf, class _Class>
	error_t get_static(void ** ppObject, uint32_t version)
	{
		_Intf* pIntf = new (std::nothrow) _Class();
		if (pIntf == nullptr)
		{
			return E_MEMORY_ERROR;
		}

		uint32_t ov;
		*ppObject = pIntf->query(_Intf::iid, version, ov);
		return *ppObject ? E_NO_ERROR, E_INTF_NOT_FOUND_ERROR;
	}
}


namespace cross
{


error_t object_factory::get_object(const guid_t& id, void ** ppObject)
{
	guid_t id_without_version = id & 0xffffff00;
	guid_t id_version = id & 0x000000ff;

	if (id_without_version == detail::GUID_BASE)
	{
		return get_base(ppObject, (uint32_t)id_version);
	}
	
	if (id_without_version == detail::GUID_CHECKER)
	{
		return get_checker(ppObject, (uint32_t)id_version);
	}

	return E_NOT_IMPLEMENTED_ERROR;
}


error_t object_factory::get_base(void** ppObject, uint32_t version)
{
#if defined BASE_DLL

	return E_NOT_IMPLEMETED_ERROR;

#elif defined BASE_LIB

	return get_static<base::base_intf, base::base_impl>(ppObject, version);

#else /*BASE_NONE*/

	return E_NOT_IMPLEMENTED_ERROR;

#endif
}


error_t object_factory::get_checker(void** ppObject, uint32_t version)
{
#if defined CHECKER_DLL

	return E_NOT_IMPLEMETED_ERROR;

#elif defined CHECKER_LIB

	return get_static<checklib::checklib_intf, checklib::checklib_impl>(ppObject, version);

#else /*CHECKER_NONE*/

	return E_NOT_IMPLEMENTED_ERROR;

#endif
}


}