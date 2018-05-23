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
	struct fake_handler: checklib::incident_handler_intf {
		virtual void on_incident(incident_info info, action* action_to_take, cross::settable_string_ref* replacement) noexcept {}
		virtual void on_finish(cross::string_ref file, error_t err) {}
	};

TEST_METHOD(ccc_KTTODO)
{
	fake_handler fh{};
	ccc::checking_module_impl ccc_module;
	ccc_module.configure(TEXT(""), fh);
	ccc_module.check_files({ TEXT("a") });
}

};
}