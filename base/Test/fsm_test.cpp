#include "stdafx.h"
#include "CppUnitTest.h"

#include <base/FiniteStateMachine/fsm.h>

#include <crossmodule/types/string.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace base_test
{		

TEST_CLASS(fsm_test)
{
public:

	struct FsmDumper:
		public base::fsm_impl
	{
		FsmDumper(std::shared_ptr<base::fsm_context_factory_intf>& spCtxFactory):
			base::fsm_impl(spCtxFactory)
		{}


		base::string dump() const
		{
			return __super::dump();
		}
	};


TEST_METHOD(KTTODO_All_UT)
{
	std::shared_ptr<base::fsm_context_factory_intf> f = std::make_shared<base::fsm::base_context_factory>();
	FsmDumper fsm(f);
	base::fsm::state_id a = fsm.generate_state(0);
	fsm.set_start(a);

	fsm.add_regex(a, TEXT("(c)*(a)+c"), 1, 0);

	std::shared_ptr<base::fsm_walker_intf> w = fsm.create_walker();

	base::string dump = fsm.dump();

	Assert::IsTrue(w->verify_literal(TEXT("c")));
	Assert::IsTrue(w->get_context() == 0);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("a")));
	Assert::IsTrue(w->get_context() == 0);
	w->reset();

	Assert::IsTrue(w->verify_literal(TEXT("ac")));
	Assert::IsTrue(w->get_context() == 1);
}


};
}