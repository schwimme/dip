#pragma once


#include "base_loader_intf.h"
#include <memory>


namespace base
{

class base_loader:
	public base_loader_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<base_loader_intf> create_base_loader() const
		{
			return std::make_shared<base_loader>();
		}
	};

public:
	virtual void get_base(cross::versioned_intf_ref<base::base_intf> pBase) override;
};


}
