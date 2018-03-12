#pragma once

#include <base_intf/FiniteStateMachine/fsmcontextfactory_intf.h>

#include <types/string.h>
#include <memory>


namespace checker
{


struct la_cfg
{

};


struct la_cfg_builder_intf
{
	virtual void build(const base::string& path, std::shared_ptr<la_cfg>& cfg, std::shared_ptr<base::fsm_context_factory_intf>& fsmFactory) const = 0;
};


}
