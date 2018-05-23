#pragma once


#include <ccc_intf/ccc_checking_module_intf.h>

#include "impl.h"


namespace ccc
{

class checking_module_intf_impl:
	public cross::query_impl<checking_module_intf>
{
public:
	virtual error_t configure(cross::string_ref config_path, checklib::incident_handler_intf* pHandler) override;
	virtual error_t check_files(cross::enumerator<cross::string_ref>* files) override;

private:
	checking_module_impl m_impl;
	std::shared_ptr<sys::registrator_intf> m_reg;
};

}
