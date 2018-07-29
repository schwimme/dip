#pragma once

#include <base_intf/ll_validator/ll_validator_intf.h>
#include <map>

namespace base
{

struct ll_validator_walker_intf_impl:
	public ll_validator_walker_intf
{
	ll_validator_walker_intf_impl(const std::map<uint32_t, std::map<uint32_t, const std::pair<rule_item, std::vector<rule_item>>*>>* const p_table, uint32_t start_nonterm, uint32_t indent_token, uint32_t begin_indent_depth = 0):
		m_p_table(p_table),
		m_indent_depth(begin_indent_depth),
		m_indent_token(indent_token)
	{
		m_stack.push_back({ false, start_nonterm });
	}
	
	virtual error_t process_step(uint32_t token) override;
	virtual bool is_accepted() const override { return m_stack.empty(); }

private:
	const std::map<uint32_t, std::map<uint32_t, const std::pair<rule_item, std::vector<rule_item>>*>>* const m_p_table;
	std::vector<rule_item> m_stack;
	uint32_t m_indent_depth = 0;
	uint32_t m_indent_token = 0;
	uint32_t m_indent_counter = 0;
};


struct ll_validator_intf_impl:
	public ll_validator_intf
{
	virtual error_t add_grammar_rule(const rule_item& parent, const std::vector<rule_item>& successors) override;
	virtual error_t create_walker(ll_validator_walker_intf*& p_walker, uint32_t start_nonterm, uint32_t indent_token, uint32_t begin_indent_depth = 0) override;

protected:
	error_t optimize();
	error_t construct_first_set();
	error_t construct_step_table();

private:
	std::vector<uint32_t>                                     m_terminals;
	std::vector<uint32_t>                                     m_non_terminals;

	std::map<rule_item, std::vector<uint32_t>>   m_first;

	std::vector<std::pair<rule_item, std::vector<rule_item>>> m_rules;

	std::map<uint32_t, std::map<uint32_t, const std::pair<rule_item, std::vector<rule_item>>*>> m_table;

	bool m_optimized = false;
};

}
