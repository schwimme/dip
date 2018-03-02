#pragma once


#include "pda_walker_impl.h"
#include "walkable_intf.h"
#include <base_intf/PushDownAutomata/pda_defs.h>
#include <unordered_map>
#include <map>
#include <memory>


namespace base
{
namespace pda
{


class impl:
	walkable_intf
{
public:
	void register_rule(const pda::token_id& input, const std::vector<stack_item>& stackTop, const std::vector<stack_item>& stackRewrite);
	std::shared_ptr<pda::walker_impl> create_walker() const;

public:
	virtual const std::map<std::vector<stack_item>, std::list<std::vector<stack_item>>>& get_rules(const pda::token_id& input) const override;

private:
	std::unordered_map<token_id, std::map<std::vector<stack_item>, std::list<std::vector<stack_item>>>> m_rules;
};


}
}
