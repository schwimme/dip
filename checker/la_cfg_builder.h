#pragma once


#include "la_cfg_builder_intf.h"


namespace checklib
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
	virtual std::shared_ptr<la_cfg> build(const sys::string& path) const override;
};


}
