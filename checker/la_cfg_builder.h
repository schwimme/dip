#pragma once


#include "la_cfg_builder_intf.h"


namespace checker
{


class la_cfg_builder:
	public la_cfg_builder_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<la_cfg_builder_intf> create_la_cfg_builder() const
		{
			return std::make_shared<la_cfg_builder>();
		}
	};

public:
	virtual void build(const base::string& path, std::shared_ptr<la_cfg>& cfg, std::shared_ptr<base::fsm_context_factory_intf>& fsmFactory) const override;
};


}
