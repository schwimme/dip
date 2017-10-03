#pragma once


#include <vector>
#include <base/Types/string.h>
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

		CharType char1 = 0;
		CharType char2 = 0;
		uint32_t iteration_begin	= 0;
		uint32_t iteration_count	= 0;
		Type type					= Type::NORMAL;
	};

	using Regex = std::vector<RegexItem>;

public:
	void BuildFsmFromRegex(CFsm& fsm, const Base::String& regex, ContextId valid, ContextId invalid) const;

protected:
	// KTTODO - other class and remove virtual
	virtual Regex ParseRegex(const Base::CharType*& regex) const;

	void CheckEnd(const Base::CharType* rest) const
	{
		if (*rest == 0)
		{
			throw "KTTODO";
		}
	}

	void CheckIs(const Base::CharType* rest, Base::CharType expected) const
	{
		if (*rest != expected)
		{
			throw "KTTODO";
		}
	}

	void ThrowParsingException(const Base::CharType* rest) const
	{
		throw "KTTODO";
	}

	Base::CharType ParseCharacter(const Base::CharType*& rest, CBuilder::Regex& out) const;

	RegexItem ParseRange(const Base::CharType*& rest) const;

};


}
}
