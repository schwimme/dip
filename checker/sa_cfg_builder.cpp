#include "sa_cfg_builder.h"
#include <sys/debugging/debug.h>
#include <error/exceptions.h>
#include <cctype>


namespace checklib
{


std::shared_ptr<sa_cfg> sa_cfg_builder::build(const sys::string& configuration) const
{
	std::shared_ptr<sa_cfg> cfg = std::make_shared<sa_cfg>();

	/*	[sa_items], where sa_item is {token,[stack_items]}.
		token and stack_item are numbers.
		Example of data is:
		[{0,[t26,n9]},{5,[t6]}]
	*/
	const sys::char_t* p_cfg = configuration.c_str();

	check_is(p_cfg, TEXT('['));
	
	while (*p_cfg != TEXT(']'))
	{
		++p_cfg;
		check_end(p_cfg);

		sa_cfg::rule r = parse_one_rule(p_cfg);
		cfg->m_rules.push_back(std::move(r));

		check_is(p_cfg, TEXT(','), TEXT(']'));
	}

	return cfg;
}


sa_cfg::rule sa_cfg_builder::parse_one_rule(const sys::char_t*& p_cfg) const
{
	// {0,[t26,n9]}
	sa_cfg::rule rv;

	check_is(p_cfg, TEXT('{'));
	++p_cfg;

	rv.parent = base::rule_item_non_terminal(parse_token_id(p_cfg));
	check_is(p_cfg, TEXT(','));
	++p_cfg;

	rv.successors = parse_stack_item_list(p_cfg);
	check_is(p_cfg, TEXT('}'));
	++p_cfg;

	return rv;
}


la_cfg::token_id sa_cfg_builder::parse_token_id(const sys::char_t*& p_cfg) const
{
	// 132
	const sys::char_t* p_end = p_cfg;
	while (std::isdigit(*p_end))
	{
		++p_end;
	}

	sys::string id(p_cfg, p_end);

	p_cfg = p_end;

	return std::stoul(id);
}


std::vector<base::rule_item> sa_cfg_builder::parse_stack_item_list(const sys::char_t*& p_cfg) const
{
	// [1,2,24]
	std::vector<base::rule_item> rv;

	check_is(p_cfg, TEXT('['));
	++p_cfg;

	const sys::char_t* b = p_cfg, *e = p_cfg;

	while (*e != TEXT(']'))
	{
		check_end(b);

		bool terminal = (*b == TEXT('t'));
		++b;
		e = find_next_of(b, TEXT(','), TEXT(']'));
		uint32_t i = std::stoul(std::string(b, e));

		rv.push_back({terminal, i});
		b = e + 1;
	}

	p_cfg = e + 1;
	return rv;
}

}
