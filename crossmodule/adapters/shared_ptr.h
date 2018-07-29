#pragma once


#include <crossmodule/types/pointer.h>


namespace cross
{

template<typename base_intf_t>
struct std_shared_ptr_on_versioned_intf_ref:
	versioned_intf_ref<base_intf_t>
{
	template<typename requested_intf_t>
	std_shared_ptr_on_versioned_intf_ref(std::shared_ptr<requested_intf_t>& sp):
		versioned_intf_ref(),
		m_sp(sp)
	{
		m_pIntf = &m_ptr;
		m_requestedVersion = requested_intf_t::version;
	}

	~std_shared_ptr_on_versioned_intf_ref()
	{
		m_sp.reset(m_ptr);
	}

private:
	std::shared_ptr<base_intf_t>	m_sp;
	base_intf_t*					m_ptr;
};


}
