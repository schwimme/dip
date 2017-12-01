#pragma once


#include "query.h"


namespace crossmodule
{


template<typename intf_base_t>
struct versioned_intf_ref
{
	// Empty preparation for fill later using derived ctor:
	versioned_intf_ref():
		m_requestedVersion(0),
		m_pIntf(0)
	{}

	template<typename intf_requested_t>
	versioned_intf_ref(intf_requested_t*& pIntf) :
		m_requestedVersion(intf_requested_t::version),
		m_pIntf(reinterpret_cast<intf_base_t**>(&pIntf))
	{}

	void attach(intf_base_t* pIntf)
	{
		*m_pIntf = reinterpret_cast<intf_base_t*>(pIntf->query(intf_base_t::iid, m_requestedVersion));
	}

protected:
	intf_base_t**	m_pIntf;
	uint32_t		m_requestedVersion;
};


}
