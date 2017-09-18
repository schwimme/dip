#pragma once


#include <vector>
#include <Base/Types/string.h>
#include "defs.h"

namespace Base
{

class CFsm;

namespace Fsm
{

/*
	KTTODO - for start only simple regex is supported:
	e.g.
	[a-z]{3}(ab)*
	Completly adding unix regex has to be implemented in future.
*/

class CBuilder
{
protected:
	struct RegexItem
	{
		enum class Type
		{
			NORMAL,				// For standart character
			RANGE,				// For [a-z] type
			ITERATION,			// For (x)*
			POSITIVE_ITERATION,	// For (x)+
			NUMERIC_ITERATION,	// For x{2}
		};

		CharType normal				= 0;
		CharType range_begin		= 0;
		CharType range_end			= 0;
		uint32_t iteration_begin	= 0;
		uint32_t iteration_count	= 0;
		Type type					= Type::NORMAL;
		bool isFinite				= false;
	};

	using Regex = std::vector<RegexItem>;

public:
	void MergeRegex(CFsm& fsm, const Base::String& regex, ContextId valid, ContextId invalid) const;

protected:
	// KTTODO - other class
	Regex ParseRegex(const Base::String& regex) const;
};


}
}
