#include "ll_validator_impl.h"

#include <sys/debugging/debug.h>
#include <sys/algorithm/algorithm.h>
#include "sys/scopeguard/scopeguard.h"

namespace base
{

error_t ll_validator_walker_intf_impl::process_step(uint32_t token)
{
	if (m_indent_counter)
	{
		--m_indent_counter;
		return (token == m_indent_token) ? E_NO_ERROR : -5; // broken indent.
	}

	if (m_stack.empty())
	{
		// empty stack - error
		return -2;
	}

	auto stack_top = m_stack.back();

	const std::pair<rule_item, std::vector<rule_item>>* p_rule_pair;

	{
		sys::scope_guard pop_back_guard([&] { m_stack.pop_back(); });

		while (1)
		{
			if (stack_top.is_terminal == true)
			{
				break;
			}

			if (stack_top == -1)
			{
				if (m_indent_depth == 0)
				{
					m_stack.pop_back();
					stack_top = m_stack.back();
				}
				else
				{
					m_indent_counter = m_indent_depth - 1;
					return (token == m_indent_token) ? E_NO_ERROR : -5; // broken indent.
				}
			}
			else if (stack_top == -2)
			{
				++m_indent_depth;
				m_stack.pop_back();
				stack_top = m_stack.back();
			}
			else if (stack_top == -3)
			{
				ASSERT(m_indent_depth != 0);
				--m_indent_depth;
				m_stack.pop_back();
				stack_top = m_stack.back();
			}
			else
			{
				break;
			}
		}

		if (stack_top.is_terminal)
		{
			return (stack_top.value == token) ? E_NO_ERROR : -3; // unexpected token
		}
	
		// Nonterm on the top:
		try
		{
			p_rule_pair = m_p_table->at(stack_top.value).at(token);
		}
		catch (...)
		{
			return -4; // no applicable rule
		}
	}

	m_stack.insert(m_stack.end(), p_rule_pair->second.rbegin(), p_rule_pair->second.rend());

	return process_step(token);
}

error_t ll_validator_intf_impl::add_grammar_rule(const rule_item& parent, const std::vector<rule_item>& successors)
{
	ASSERT(m_optimized == false);
	ASSERT(parent.is_terminal == false);

	m_non_terminals.push_back(parent.value);

	for (const auto& i : successors)
	{
		if (i.is_terminal)
		{
			if (sys::find(m_terminals, i.value) == false)
			{
				m_terminals.push_back(i.value);
			}
		}
		else
		{
			if (sys::find(m_non_terminals, i.value) == false)
			{
				m_non_terminals.push_back(i.value);
			}
		}
	}

	m_rules.push_back(std::make_pair(parent, successors));

	return E_NO_ERROR;
}

error_t ll_validator_intf_impl::create_walker(ll_validator_walker_intf*& p_walker, uint32_t start_nonterm, uint32_t indent_token, uint32_t begin_indent_depth )
{
	if (m_optimized == false)
	{
		RETURN_FAIL(optimize());
		m_optimized = true;
	}

	p_walker = new ll_validator_walker_intf_impl(&m_table, start_nonterm, indent_token, begin_indent_depth);
	return E_NO_ERROR;
}

error_t ll_validator_intf_impl::construct_first_set()
{
	// Check all terms:
	for (const auto& i : m_terminals)
	{
		m_first[{ true, i }] = { i };
	}

	bool change = false;
	do
	{
		change = false;
		for (const auto& active_rule: m_rules)
		{
			auto first_non_meta = active_rule.second.begin();
			while (first_non_meta->is_terminal == false && (first_non_meta->value == -1 || first_non_meta->value == -2 || first_non_meta->value == -3)) first_non_meta++;

			const auto first_of_active_rule_replace = m_first.find(*first_non_meta);
			if (first_of_active_rule_replace != m_first.end())
			{
				auto& ref = m_first[active_rule.first];
				auto size_before = ref.size();
				ref = sys::make_union(first_of_active_rule_replace->second, ref);
				change |= (size_before != ref.size());
			}
		}
	} while (change);

	return E_NO_ERROR;
}

error_t ll_validator_intf_impl::construct_step_table()
{
	for (const auto& rule : m_rules)
	{
		const auto& current_first_set = m_first[rule.second[0]];
		auto& table_col = m_table[rule.first.value];

		for (const auto& terminal : current_first_set)
		{
			if (table_col.find(terminal) != table_col.end())
			{
				ASSERT(false);
				return -1; // KTTODO multiple rules.
			}

			table_col[terminal] = &rule;
		}
	}

	return E_NO_ERROR;
}

error_t ll_validator_intf_impl::optimize()
{
	RETURN_FAIL(construct_first_set());
	return construct_step_table();
}

}
