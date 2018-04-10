#pragma once

#include <cstdint>
#include <error/error_code_base.h>


namespace cross
{

// KTTODO - move else and make as string.
using guid_t = uint32_t;
static const guid_t GUID_BASE_V1 = 0x1;


struct object_factory_intf
{
	virtual error_t get_object(const guid_t& id, void ** ppObject) const = 0;
};


}
