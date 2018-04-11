#pragma once


#include <sys/types/string.h>
#include <memory>
#include "configurable_fsm_factory.h"


namespace checklib
{


struct la_cfg
{
	using token_id = uint32_t;
	std::vector<std::pair<token_id, sys::string>> m_tokens; // token id mapped to their regular expression
	std::vector<std::vector<token_id>>  m_priorityGroups;
};


struct la_cfg_builder_intf
{
	virtual std::shared_ptr<la_cfg> build(const sys::string& path) const = 0;
};


}
