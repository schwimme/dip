#include "stdafx.h"
#include "CppUnitTest.h"

#include "../checker.h"
#include <sys/types/string.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Checker_test
{		

TEST_CLASS(LA_test)
{

TEST_METHOD(KTTODO_ALL_UT)
{
	sys::string la = TEXT(R"({[{1,"r1"},{2,"r2"}],[[1,2],[3,4,5,6]]})");
	sys::string sa = TEXT(R"([{0,[26,9],[]},{5,[],[1243]}])");

//	test_checker c;
//	c.configure(la, sa);
//
//	c.check({ TEXT("") });
}

};
}