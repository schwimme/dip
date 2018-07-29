#pragma once

#include <sys/types/string.h>
#include <vector>

namespace ccc
{


struct configuration_builder_intf
{
	virtual void build_configuration(sys::string& la_cfg, sys::string& sa_cfg, std::vector<uint32_t>& ignored_tokens, const sys::string& input_config) const = 0;
};

}
