#include "stdafx.h"
#include "CppUnitTest.h"

#include <base/PushDownAutomata/pda_impl.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace base_test
{

TEST_CLASS(pda_test)
{
public:

TEST_METHOD(KTTODO_All_UT)
{
	std::shared_ptr<base::pda::impl> spImpl = std::make_shared<base::pda::impl>();
	spImpl->add_rule(1, {}, { 2 });
	spImpl->add_rule(2, { 2 }, {});

	auto w = spImpl->create_walker();

	Assert::IsTrue(w->process_step(1));
	Assert::IsTrue(w->process_step(1));
	Assert::IsTrue(w->process_step(1));
	Assert::IsTrue(w->process_step(2));
	Assert::IsTrue(w->process_step(2));

	Assert::IsFalse(w->accepted());
	Assert::IsTrue(w->process_step(2));
	Assert::IsTrue(w->accepted());

	Assert::IsFalse(w->process_step(2));
}


};
}