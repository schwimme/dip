#pragma once


#include <base/Types/string.h>


namespace Base
{
namespace Fsm
{


using StateId = Base::String;
using AlphabetType = Base::CharType;
using ContextId = uint32_t;

static const ContextId NON_FINITE	= 0;
static const ContextId FINITE		= 1;

namespace detail
{
	static const AlphabetType EPSILON = 0;
}

}	
}
