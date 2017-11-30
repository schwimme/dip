#pragma once


#include <vector>
#include <utility>
#include <types/string.h>

namespace checker
{

//KTTODO - move to defs
using token_id = uint32_t;

using token = std::pair<token_id, base::string>;

struct lexical_analysis_configuration
{
	using priority_group = std::vector<token_id>;

	std::vector<std::pair<token_id, base::string>> definition;
	std::vector<priority_group> priority;
};



struct lexical_analysis_intf
{
	virtual void configure(const lexical_analysis_configuration& configuration) = 0;

	virtual std::vector<token> parse(const base::string& input) = 0;
};


}
