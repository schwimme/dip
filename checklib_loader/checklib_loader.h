#pragma once


#include "checklib_loader_intf.h"
#include <memory>


namespace checklib
{

class checklib_loader:
	public checklib_loader_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<checklib_loader_intf> create_checklib_loader() const
		{
			return std::make_shared<checklib_loader>();
		}
	};

public:
	virtual void get_checklib(cross::versioned_intf_ref<checklib::checklib_intf> pBase) override;
};


}
