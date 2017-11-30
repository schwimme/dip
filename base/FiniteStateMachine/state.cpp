#include "state.h"

#include <base_intf/Algorithm/algorithm.h>

#include <base/Debugging/debug.h>


namespace base
{
namespace fsm
{

// Empty vector for returning in case, rule does not exists.
static const std::vector<state_id> g_emptyVector;


void state::add_rule(const state_id& to, base::char_t ch)
{
	std::vector<state_id>& nextStates = m_rules[ch];
	if (base::find(nextStates, to) == false)
	{
		nextStates.push_back(to);
	}
}


void state::add_optimized_epsilon_rule(const state& to)
{
	for (const auto& rule : to.m_rules)
	{
		if (rule.first != fsm::detail::EPSILON)
		{
			auto& v = m_rules[rule.first];
			v = base::make_union(v, rule.second);
		}
	}
}


void state::remove_rules(base::char_t ch)
{
	m_rules.erase(ch);
}


const std::vector<state_id>& state::get_rules(base::char_t ch) const
{
	auto it = m_rules.find(ch);
	if (it == m_rules.end())
	{
		return g_emptyVector;
	}

	return it->second;
}


}
}