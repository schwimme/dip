#pragma once


#include <base_intf/Types/string.h>


namespace Base
{
namespace Fsm
{


using StateId = Base::String;
using ContextId = uint32_t;

static const ContextId INVALID	= 0;
static const ContextId VALID	= 1;

namespace detail
{
	static const Base::CharType EPSILON = 0;
}


}	
}