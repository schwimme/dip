#pragma once


#include <vector>
#include <utility>
#include <base_intf/Types/string.h>


//!	\brief Mapping where first param of pair is token name and second is his regex.

struct LexicalAnalysisConfiguration
{
	using PriorityGroup = std::vector<uint32_t>;

	std::vector<std::pair<Base::String, Base::String>> definition;
	std::vector<PriorityGroup> priority;
};

//!	\brief Mapping where first param of pair is token name and second is his matched value.
using Token = std::pair<Base::String, Base::String>;


struct ILexicalAnalysis
{
	virtual void BuildFsm(const LexicalAnalysisConfiguration& configuration) = 0;

	virtual std::vector<Token> Parse(const Base::String& input) = 0;
};
