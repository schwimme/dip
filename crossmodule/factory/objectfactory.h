#pragma once


#include "object_factory_intf.h"
#include <memory>


namespace cross
{


// KTTODO this must have interface because of di.
struct object_factory:
	public object_factory_intf
{
public:
	struct factory
	{
		virtual std::shared_ptr<object_factory_intf> create_object_factory() const
		{
			return std::make_shared<object_factory>();
		}
	};

	virtual error_t get_object(const guid_t& id, void ** ppObject) const override;
};


}
