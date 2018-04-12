#include "sa_cfg_builder.h"
#include <sys/debugging/debug.h>
#include <error/exceptions.h>


namespace checklib
{


std::shared_ptr<sa_cfg> sa_cfg_builder::build(const sys::string& configuration) const
{
	std::shared_ptr<sa_cfg> cfg = std::make_shared<sa_cfg>();

	/*	[sa_items], where sa_item is {token, [stack_items], [stack_items]}.
		token and stack_item are numbers.
		Example of data is:
		[{0,[26,9],[]},{5,[],[1243]}]
	*/
	const sys::char_t* p_cfg = configuration.c_str();

	check_is(p_cfg, TEXT('['));
	++p_cfg;
	
	while (*p_cfg != TEXT(']'))
	{
		sa_rule r = parse_one_rule(p_cfg);
		check_end(p_cfg);
		cfg->m_rules.push_back(std::move(r));
	}

	return cfg;
}


sa_rule sa_cfg_builder::parse_one_rule(const sys::char_t*& p_cfg) const
{
	// {0,[26,9],[]}
	sa_rule rv;

	check_is(p_cfg, TEXT('{'));
	++p_cfg;

	la_cfg::token_id tid = parse_token_id(p_cfg);
	check_is(p_cfg, TEXT(','));
	++p_cfg;

	std::vector<sa_rule::stack_item> stack_top = parse_stack_item_list(p_cfg);
	check_is(p_cfg, TEXT(','));
	++p_cfg;

	std::vector<sa_rule::stack_item> stack_replace = parse_stack_item_list(p_cfg);
	check_is(p_cfg, TEXT('}'));
	++p_cfg;

	rv.m_token = tid;
	rv.m_stackTop = std::move(stack_top);
	rv.m_stackReplace = std::move(stack_replace);

	return rv;
}


la_cfg::token_id sa_cfg_builder::parse_token_id(const sys::char_t*& p_cfg) const
{
	// 0,
	const sys::char_t* p_end = find_next_of(p_cfg, TEXT(','));

	sys::string id(p_cfg, p_end - 1);

	p_cfg = p_end + 1;

	return std::stoul(id);
}


std::vector<sa_rule::stack_item> sa_cfg_builder::parse_stack_item_list(const sys::char_t*& p_cfg) const
{
	// [1,2,24]
	std::vector<sa_rule::stack_item> rv;

	check_is(p_cfg, TEXT('['));
	++p_cfg;

	const sys::char_t* b = p_cfg, *e = p_cfg + 1;

	while (*e != TEXT(']'))
	{
		check_end(b);

		e = find_next_of(b, TEXT(','), TEXT(']'));
		sa_rule::stack_item i = std::stoul(std::string(b, e));

		rv.push_back(i);
		b = e + 1;
	}

	p_cfg = b;
	return rv;
}

}
