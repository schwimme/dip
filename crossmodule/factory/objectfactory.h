#pragma once


#include <cstdint>


// KTTODO - move else and make as string.
using guid_t = uint32_t;

// KTTODO - move else.
using error_t = int32_t;


static const guid_t GUID_BASE_V1 = 0x1;


namespace crossmodule
{


struct object_factory
{
	error_t get_object(const guid_t& id, void ** ppObject) const;
};


}
