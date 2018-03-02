#pragma once


#include <base_intf/PushDownAutomata/pda_defs.h>
#include <vector>
#include <map>
#include <list>


namespace base
{
namespace pda
{


struct walkable_intf
{
	virtual const std::map<std::vector<stack_item>, std::list<std::vector<stack_item>>>& get_rules(const pda::token_id& input) const = 0;
};


}
}
