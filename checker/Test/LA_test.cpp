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
		
enum TokenEnum
{
	IDENTIFIER = 0,
	SMPTR,
	MUTEX,
	M_IDENTIFIER,
	M_SMPTR,
	M_MUTEX,
	COMMA
};


TEST_METHOD(KTTODO_ALL_UT)
{
	std::shared_ptr<Checker::ILexicalAnalysis> spLA = Checker::CLexicalAnalysis::Factory().CreateLexicalAnalysis();

	Checker::LexicalAnalysisConfiguration cfg;


	cfg.definition = {
		{ IDENTIFIER,	TEXT("([a-z])+") },
		{ SMPTR,		TEXT("sp([a-z])+") },
		{ MUTEX,		TEXT("([a-z])*mutex") },
		{ M_IDENTIFIER,	TEXT("m([a-z])+") },
		{ M_SMPTR,		TEXT("msp([a-z])+") },
		{ M_MUTEX,		TEXT("m([a-z])*mutex") },
		{ COMMA,		TEXT(", ") }
	};

	cfg.priority = {
		{M_MUTEX, M_IDENTIFIER ,MUTEX, IDENTIFIER},
		{M_SMPTR, M_IDENTIFIER ,SMPTR, IDENTIFIER}
	};


	spLA->BuildFsm(cfg);

	Base::String data = TEXT("id, spid, idmutex, mid, mspid, midmutex");

	std::vector<Checker::Token> tokens = spLA->Parse(data);
}

};
}