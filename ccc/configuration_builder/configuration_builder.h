#pragma once

#include "configuration_builder_intf.h"
#include <memory>

#include "token_specifier.h"


namespace ccc
{

class configuration_builder:
	public configuration_builder_intf,
	protected token_specifier::factory
{
public:
	struct factory
	{
		virtual std::shared_ptr<configuration_builder_intf> create_configuration_builder() const
		{
			return std::make_shared<configuration_builder>();
		}
	};

public:
	virtual void build_configuration(sys::string& la_cfg, sys::string& sa_cfg, const sys::string& input_config) const override;
};

}
