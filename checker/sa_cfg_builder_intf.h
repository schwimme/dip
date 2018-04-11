#pragma once


#include "la_cfg_builder_intf.h"
#include <sys/types/string.h>
#include <memory>


namespace checklib
{


struct sa_rule
{
	using stack_item = uint32_t;
	la_cfg::token_id m_token;
	std::vector<stack_item> m_stackTop;
	std::vector<stack_item> m_stackReplace;
};


struct sa_cfg
{
	std::vector<sa_rule> m_rules;
};


struct sa_cfg_builder_intf
{
	virtual std::shared_ptr<sa_cfg> build(const sys::string& path) const = 0;
};


}
