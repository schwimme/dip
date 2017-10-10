#pragma once


#include <vector>
#include <utility>
#include <base_intf/Types/string.h>

namespace Checker
{

//KTTODO - move to defs
using TokenId = uint32_t;

using Token = std::pair<TokenId, Base::String>;

struct LexicalAnalysisConfiguration
{
	using PriorityGroup = std::vector<TokenId>;

	std::vector<std::pair<TokenId, Base::String>> definition;
	std::vector<PriorityGroup> priority;
};



struct ILexicalAnalysis
{
	virtual void BuildFsm(const LexicalAnalysisConfiguration& configuration) = 0;

	virtual std::vector<Token> Parse(const Base::String& input) = 0;
};


}
