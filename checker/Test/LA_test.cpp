#include "stdafx.h"
#include "CppUnitTest.h"

#include "../LexicalAnalysis/LexicalAnalysis.h"
#include <types/string.h>

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
	std::shared_ptr<checker::lexical_analysis_intf> spLA = checker::lexical_analysis::factory().create_lexical_analysis();

	checker::lexical_analysis_configuration cfg;


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
		{ M_MUTEX, M_IDENTIFIER ,MUTEX, IDENTIFIER },
		{ M_SMPTR, M_IDENTIFIER ,SMPTR, IDENTIFIER }
	};


	spLA->configure(cfg);

	base::string data = TEXT("id, spid, idmutex, mid, mspid, midmutex");

	std::vector<checker::token> tokens = spLA->parse(data);
}

};
}