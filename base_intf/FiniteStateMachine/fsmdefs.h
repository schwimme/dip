#pragma once


#include <sys/types/chartype.h>
#include <cstdint>


namespace base
{
namespace fsm
{


using state_id = uint32_t;
using context_id = uint32_t;

static const context_id INVALID	= 0;
static const context_id VALID	= 1;


namespace detail
{
	static const sys::char_t EPSILON = 0;
}


}	
}
