#pragma once


namespace crossmodule
{


template<typename intf_base_t>
struct versioned_intf_ref
{
	template<typename intf_requested_t>
	versioned_intf_ref(intf_requested_t*& pIntf);

	bool attach(intf_base_t* pIntf);

protected:
	void**   m_pIntf;
	uint32_t m_requestedVersion;
};


}
