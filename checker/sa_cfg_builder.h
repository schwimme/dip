#pragma once


#include "sa_cfg_builder_intf.h"


namespace checker
{


class sa_cfg_builder :
	public sa_cfg_builder_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<sa_cfg_builder_intf> create_sa_cfg_builder() const
		{
			return std::make_shared<sa_cfg_builder>();
		}
	};

public:
	virtual std::shared_ptr<sa_cfg> build(const base::string& path) const override;
};


}
