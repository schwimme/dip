#include "stdafx.h"
#include "CppUnitTest.h"

#include "../configuration_builder/token_specifier.h"
#include "../../sys/types/chartype.h"
#include "../checking_module/impl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ccc_test
{		

TEST_CLASS(UnitTest1)
{
public:
	struct fake_handler: ccc::accident_handler_intf {};

TEST_METHOD(ccc_KTTODO)
{
	fake_handler fh;
	ccc::checking_module_impl ccc_module;
	ccc_module.configure(TEXT(""), fh);
}

};
}