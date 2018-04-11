#pragma once


#include <checker_intf/checker_intf.h>
#include "checker.h"


namespace checklib
{

class intf_impl:
	public cross::query_impl<checker_intf>
{
public:
	virtual error_t configure(cross::string_ref la_config, cross::string_ref sa_config, bool path) override;
	virtual error_t check(cross::enumerator<cross::string_ref>* pFiles) override;

private:
	checker_impl m_impl;
};

}
