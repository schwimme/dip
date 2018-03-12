#include "la_cfg_builder.h"
#include <base/Debugging/debug.h>

#include <base_intf/ScopeGuard/ScopeGuard.h>
#include <vector>


namespace checker
{

// KTTODO move else
using token_id = uint32_t;

std::shared_ptr<la_cfg> la_cfg_builder::build(const base::string& path) const
{
	std::shared_ptr<la_cfg> spCfg = std::make_shared<la_cfg>();

	// KTTODO fill

	return spCfg;
}


}
