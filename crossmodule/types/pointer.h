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


template<typename intf_base_t>
template<typename intf_requested_t>
versioned_intf_ref<intf_base_t>::versioned_intf_ref(intf_requested_t*& pIntf) :
	m_requestedVersion(intf_requested_t::version),
	m_pIntf(reinterpret_cast<void**>(&pIntf))
{
	static_assert(intf_base_t::iid == intf_requested_t::iid, "Interface iid mismatch");
}

template<typename intf_base_t>
bool versioned_intf_ref<intf_base_t>::attach(intf_base_t* pIntf)
{
	uint32_t real_version = 0;
	*m_pIntf = pIntf->query(intf_base_t::iid, m_requestedVersion, real_version);
	return *m_pIntf != nullptr;
}


}
