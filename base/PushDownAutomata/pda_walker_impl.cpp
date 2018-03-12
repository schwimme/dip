#include "pda_walker_impl.h"


namespace base
{
namespace pda
{


bool walker_impl::process_step(const pda::token_id& input)
{
	const auto& rules = m_pWalkable->get_rules(input);

	std::list<std::vector<stack_item>> newCfgs;
	for (auto& cfg: m_configurations)
	{
		auto l = process_step_in_config(cfg, rules);
		newCfgs.splice(newCfgs.end(), std::move(l));
	}

	m_non_commited_configurations = std::move(newCfgs);
	return (m_non_commited_configurations.empty() == false);
}


void walker_impl::reset()
{
	m_configurations = { {} };
}


void walker_impl::commit()
{
	m_configurations = std::move(m_non_commited_configurations);
}


bool walker_impl::is_accepted() const
{
	for (const auto& i : m_configurations)
	{
		if (i.empty())
		{
			return true;
		}
	}

	return false;
}


std::list<std::vector<stack_item>> walker_impl::process_step_in_config(const std::vector<stack_item>& cfg, const std::map<std::vector<stack_item>, std::list<std::vector<stack_item>>>& rules)
{
	std::list<std::vector<stack_item>> ret;

	auto it = cfg.begin();
	bool end = false;
	do
	{
		std::vector<stack_item> stackTop(cfg.begin(), it);
		std::vector<stack_item> stackRest(it, cfg.end());
		if (it != cfg.end())
		{
			++it;
		}
		else
		{
			end = true;
		}

		auto currentStackTopRules = rules.find(stackTop);
		if (currentStackTopRules != rules.end())
		{
			auto l = expand_by_one_stack_top(stackRest, currentStackTopRules->second);
			ret.splice(ret.end(), std::move(l));
		}
	} while (end == false);

	return ret;
}


std::list<std::vector<stack_item>> walker_impl::expand_by_one_stack_top(const std::vector<stack_item>& stackRest, const std::list<std::vector<stack_item>>& rules)
{
	std::list<std::vector<stack_item>> ret;

	for (const auto& r: rules)
	{
		std::vector<stack_item> concatenate = stackRest;
		concatenate.insert(concatenate.end(), r.begin(), r.end());
		ret.push_back(std::move(concatenate));
	}

	return ret;
}


}
}
