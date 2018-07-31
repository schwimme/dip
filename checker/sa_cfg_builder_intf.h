#pragma once


#include "la_cfg_builder_intf.h"

#include <sys/types/string.h>
#include <memory>
#include "base_intf/ll_validator/ll_validator_intf.h"


namespace checklib
{


struct sa_cfg
{
	struct rule
	{
		base::rule_item parent;
		std::vector<base::rule_item> successors;
	};
	std::vector<rule> m_rules;

	base::rule_item m_indent;
	base::rule_item m_start;
};


struct sa_cfg_builder_intf
{
	virtual std::shared_ptr<sa_cfg> build(const sys::string& configuration) const = 0;
};


}
