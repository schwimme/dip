#pragma once


#include <base_intf/PushDownAutomata/pda_defs.h>
#include "walkable_intf.h"
#include <vector>


namespace base
{
namespace pda
{


class walker_impl
{
public:
	walker_impl(const walkable_intf& walkable):
		m_pWalkable(&walkable)
	{
		reset();
	}

public:
	virtual bool process_step(const pda::token_id& input);
	virtual void reset();
	virtual void commit();
	virtual bool is_accepted() const;

protected:
	std::list<std::vector<stack_item>> process_step_in_config(const std::vector<stack_item>& cfg, const std::map<std::vector<stack_item>, std::list<std::vector<stack_item>>>& rules);
	std::list<std::vector<stack_item>> expand_by_one_stack_top(const std::vector<stack_item>& stackRest, const std::list<std::vector<stack_item>>& rules);

private:
	const walkable_intf*               m_pWalkable = nullptr;
	std::list<std::vector<stack_item>> m_configurations;
	std::list<std::vector<stack_item>> m_non_commited_configurations;
};


}
}
