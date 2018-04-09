#pragma once


#include <cstdint>



// KTTODO - move else.
using error_t = int32_t;



namespace cross
{

// KTTODO - move else and make as string.
using guid_t = uint32_t;

static const guid_t GUID_BASE_V1 = 0x1;



// KTTODO this must have interface because of di.
struct object_factory
{
	error_t get_object(const guid_t& id, void ** ppObject) const;
};


}
