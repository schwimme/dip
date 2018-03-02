#include "pda_impl.h"
#include <base_intf/Algorithm/algorithm.h>


namespace base
{
namespace pda
{
namespace
{
	const std::map<std::vector<stack_item>, std::list<std::vector<stack_item>>> g_emptyRule;
}

void impl::register_rule(const pda::token_id& input, const std::vector<stack_item>& stackTop, const std::vector<stack_item>& stackRewrite)
{
	auto& pos = m_rules[input][stackTop];
	if (base::find(pos, stackRewrite) == false)
	{
		// Multiple rule registration:
		return;
	}

	pos.push_back(stackRewrite);
}


std::shared_ptr<pda::walker_impl> impl::create_walker() const
{
	return std::make_shared<pda::walker_impl>();
}


const std::map<std::vector<stack_item>, std::list<std::vector<stack_item>>>& impl::get_rules(const pda::token_id& input) const
{
	auto it = m_rules.find(input);
	if (it == m_rules.end())
	{
		return g_emptyRule;
	}

	return it->second;
}


}
}