#include "la_cfg_builder.h"
#include <sys/debugging/debug.h>

#include <sys/scopeguard/scopeguard.h>
#include <vector>


namespace checker
{

// KTTODO move else
using token_id = uint32_t;

std::shared_ptr<la_cfg> la_cfg_builder::build(const sys::string& path) const
{
	std::shared_ptr<la_cfg> spCfg = std::make_shared<la_cfg>();

	// KTTODO fill

	return spCfg;
}


}
