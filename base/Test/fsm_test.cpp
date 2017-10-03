#include "stdafx.h"
#include "CppUnitTest.h"

#include <base/FiniteStateMachine/fsm.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Base_Test
{		

TEST_CLASS(Fsm_test)
{
public:

	struct FsmDumper:
		public Base::CFsm
	{
		FsmDumper(std::shared_ptr<Base::IFsmContextFactory>& spCtxFactory):
			Base::CFsm(spCtxFactory)
		{}


		Base::String Dump() const
		{
			return __super::Dump();
		}
	};

TEST_METHOD(KTTODO_All_UT)
{
	std::shared_ptr<Base::IFsmContextFactory> f = std::make_shared<Base::Fsm::CBaseContextFactory>();
	FsmDumper fsm(f);
	Base::Fsm::StateId a = fsm.GenerateState(0);
	fsm.SetStart(a);

	fsm.AddRegex(a, TEXT("(c)*(a)+c"), 1, 0);

	std::shared_ptr<Base::IFsmWalker> w = fsm.CreateWalker();

	Base::String dump = fsm.Dump();

	Assert::IsTrue(w->VerifyLiteral(TEXT("c")));
	Assert::IsTrue(w->GetContext() == 0);
	w->Reset();

	Assert::IsTrue(w->VerifyLiteral(TEXT("a")));
	Assert::IsTrue(w->GetContext() == 0);
	w->Reset();

	Assert::IsTrue(w->VerifyLiteral(TEXT("ac")));
	Assert::IsTrue(w->GetContext() == 1);
}


};
}