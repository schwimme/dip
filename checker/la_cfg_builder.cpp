#include "la_cfg_builder.h"
#include <sys/debugging/debug.h>

#include <sys/scopeguard/scopeguard.h>
#include <vector>


namespace checklib
{

std::shared_ptr<la_cfg> la_cfg_builder::build(const sys::string& configuration) const
{
	// {[{1,"r1"},{2,"r2"}],[[1,2],[3,4,5,6]]}
	std::shared_ptr<la_cfg> spCfg = std::make_shared<la_cfg>();

	const sys::char_t* p_cfg = configuration.c_str();

	check_is(p_cfg, TEXT('{'));
	++p_cfg;
	check_is(p_cfg, TEXT('['));
	++p_cfg;
	check_end(p_cfg);

	std::vector<std::pair<token_id, sys::string>> tokens;
	while (*p_cfg != TEXT(']'))
	{
		std::pair<token_id, sys::string> p = parse_token_pair(p_cfg);
		check_is(p_cfg, TEXT(','), TEXT(']'));
		tokens.push_back(std::move(p));
	}

	++p_cfg;
	check_is(p_cfg, TEXT(','));
	++p_cfg;
	check_is(p_cfg, TEXT('['));
	++p_cfg;
	check_end(p_cfg);

	std::vector<std::vector<token_id>> priorities;
	while (*p_cfg != TEXT(']'))
	{
		std::vector<token_id> p = parse_priority_group(p_cfg);
		check_is(p_cfg, TEXT(','), TEXT(']'));
		priorities.push_back(std::move(p));
	}

	check_is(p_cfg, TEXT('}'));

	spCfg->m_tokens = std::move(tokens);
	spCfg->m_priorityGroups = std::move(priorities);

	return spCfg;
}


std::pair<token_id, sys::string> la_cfg_builder::parse_token_pair(const sys::char_t*& p_cfg) const
{
	// {1,"r1"},
	check_is(p_cfg, TEXT('{'));
	++p_cfg;
	check_end(p_cfg);

	token_id tid = parse_token_id(p_cfg);
	sys::string rgx = parse_token_regex(p_cfg);

	check_is(p_cfg, TEXT('}'));
	++p_cfg;

	return std::make_pair(tid, std::move(rgx));
}


token_id la_cfg_builder::parse_token_id(const sys::char_t*& p_cfg) const
{
	// 1,
	const sys::char_t* p_end = find_next_of(p_cfg, TEXT(','));

	sys::string id(p_cfg, p_end - 1);

	p_cfg = p_end + 1;

	return std::stoul(id);
}


sys::string la_cfg_builder::parse_token_regex(const sys::char_t*& p_cfg) const
{
	// "r1"
	const sys::char_t* b = p_cfg;
	check_is(b, TEXT('\"'));
	++b;
	check_end(b);

	const sys::char_t* e = b;
	while (*e != TEXT('\"'))
	{
		if (*e == TEXT('\\'))
		{
			// Next is special character, skip it:
			++e;
			check_end(e);
		}

		++e;
		check_end(e);
	}

	p_cfg = e + 1;

	return std::string(b, e);
}


std::vector<token_id> la_cfg_builder::parse_priority_group(const sys::char_t*& p_cfg) const
{
	// [1,2,43]
	std::vector<token_id> rv;

	check_is(p_cfg, TEXT('['));
	++p_cfg;
	check_end(p_cfg);

	while (*p_cfg != TEXT(']'))
	{
		token_id tid = parse_token_id(p_cfg);
		rv.push_back(tid);
	}

	return rv;
}


}
