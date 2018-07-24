#pragma once

#include<error/error_code_base.h>
#include <vector>

namespace base
{

struct rule_item
{
	rule_item():
		rule_item(false, 0)
	{}

	rule_item(bool is_terminal, uint32_t value) :
		is_terminal(is_terminal),
		value(value)
	{}

	bool is_terminal;
	uint32_t value;
	bool operator<(const rule_item& r) const
	{
		return (is_terminal == r.is_terminal) ? (value < r.value) : is_terminal;
	}

	operator uint32_t()
	{
		return value;
	}
};

struct rule_item_terminal:
	public rule_item
{
	rule_item_terminal(uint32_t value):
		rule_item(true, value)
	{}
};

struct rule_item_non_terminal:
	public rule_item
{
	rule_item_non_terminal(uint32_t value):
		rule_item(false, value)
	{}
};

namespace meta
{
struct indent_check:
	public rule_item_non_terminal
{
	indent_check() :
		rule_item_non_terminal(-1)
	{}
};


struct indent_inc:
	public rule_item_non_terminal
{
	indent_inc():
		rule_item_non_terminal(-2)
	{}
};

struct indent_dec:
	public rule_item_non_terminal
{
	indent_dec():
		rule_item_non_terminal(-3)
	{}
};

}

struct ll_validator_walker_intf
{
	virtual error_t process_step(uint32_t token) = 0;
	virtual bool is_accepted() const = 0;
};

struct ll_validator_intf
{
	virtual error_t add_grammar_rule(const rule_item& parent, const std::vector<rule_item>& successors) = 0;
	virtual error_t create_walker(ll_validator_walker_intf*& p_walker, uint32_t start_nonterm, uint32_t indent_token, uint32_t begin_indent_depth = 0) = 0;
};

}
