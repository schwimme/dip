#include "la_cfg_builder.h"
#include <base/Debugging/debug.h>

#include <base_intf/ScopeGuard/ScopeGuard.h>
#include <vector>


namespace checker
{

// KTTODO move else
using token_id = uint32_t;

void la_cfg_builder::build(const base::string& path, std::shared_ptr<la_cfg>& spCfg, std::shared_ptr<base::fsm_context_factory_intf>& spFsmFactory) const
{
	base::scope_guard commit_point([&] 
	{
		spCfg.reset();
		spFsmFactory.reset();
	});

	spCfg = std::make_shared<la_cfg>();	

	// Fill
	std::vector<std::vector<token_id>> priorityGroup;

	spFsmFactory = std::make_shared<base::fsm_context_factory_intf>(priorityGroup);
	commit_point.dismiss();
}


}
