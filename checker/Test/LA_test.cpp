#include "stdafx.h"
#include "CppUnitTest.h"

#include "../LexicalAnalysis/LexicalAnalysis.h"
#include <base_intf/Types/string.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Checker_test
{		

TEST_CLASS(LA_test)
{
public:
		
TEST_METHOD(KTTODO_ALL_UT)
{
	std::shared_ptr<Checker::ILexicalAnalysis> spLA = Checker::CLexicalAnalysis::Factory().CreateLexicalAnalysis();

	Checker::LexicalAnalysisConfiguration cfg;
	cfg.definition = {
//		{ TEXT("member"), TEXT("m_([a-z])+") },

		{ 1, TEXT("([a-z])+") }, // id
		{ 1, TEXT("([A-Z])+") }, // id
		{ 2, TEXT("+") }, // operator
		{ 2, TEXT("++") }, // operator
		{ 2, TEXT("-") }, // operator
		{ 2, TEXT("--") }, // operator
		{ 3, TEXT(" ") }, // space
		{ 4, TEXT("\t") }, // tab
	};

	spLA->BuildFsm(cfg);

	Base::String data = TEXT("someid--	+ OTHERID");

	std::vector<Checker::Token> tokens = spLA->Parse(data);
}

};
}