#pragma once

#include <sys/types/string.h>


namespace ccc
{


struct configuration_builder_intf
{
	virtual void build_configuration(sys::string& la_cfg, sys::string& sa_cfg, const sys::string& input) const = 0;
};

}
